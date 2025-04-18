#include "ansi_fp.h"
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <cwctype>
#include "stdio_api.h"
#include <wchar_t.h>
#include "wmem.h"
#include "wstring.h"
#include "va_list.h"
#pragma exceptions on

#define LDBL_MANT_DIG          24
#define LDBL_MAX_EXP          128
#define TARGET_FLOAT_BITS 32
#define TARGET_FLOAT_BYTES	(TARGET_FLOAT_BITS/8)
#define TARGET_FLOAT_MAX_EXP	LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG	LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS		(TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS		(TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

typedef long long intmax_t;

#define PTRDIFF __typeof__((char*)0-(char*)0)
typedef PTRDIFF ptrdiff_t;

wchar_t* wcscpy(wchar_t *pDest, const wchar_t *pSrc);
size_t strlen(const char *pStr);
void* memchr(const void *, int, size_t);
int mbtowc(wchar_t *pDest, const char *s, size_t num);
int vswprintf(wchar_t *s, size_t n, const wchar_t *format, va_list arg);

enum justification_options {
	left_justification,
	right_justification,
	zero_fill
};

enum sign_options {
	only_minus,
	sign_always,
	space_holder
};

enum argument_options {
	normal_argument,
	char_argument,
	short_argument,
	long_argument,
	long_long_argument,
	wchar_argument,
	intmax_argument,
	size_t_argument,
	ptrdiff_argument,
	long_double_argument
};

typedef struct {
	unsigned char	justification_options;		// 0x0
	unsigned char 	sign_options;				// 0x1
	unsigned char 	precision_specified;		// 0x2
	unsigned char	alternate_form;				// 0x3
	unsigned char 	argument_options;			// 0x4
	wchar_t	conversion_char;			// 0x5
	int			  	field_width;				// 0x8
	int				precision;					// 0xC
} print_format;

typedef struct {
	char* CharStr;
	size_t MaxCharCount;
	size_t CharsWritten;
} __OutStrCtrl;

/*static*/ const wchar_t* parse_format(const wchar_t *format_string, va_list *arg, print_format *format) {
	print_format f;
	const wchar_t* s = format_string;
	wchar_t c;
	int flag_found;
	f.justification_options = right_justification;
	f.sign_options = only_minus;
	f.precision_specified = 0;
	f.alternate_form = 0;
	f.argument_options = normal_argument;
	f.field_width = 0;
	f.precision = 0;
	
	if ((c = *++s) == L'%') {
		f.conversion_char = c;
		*format = f;
		return ((const wchar_t*)s + 1);
	}

	for (;;) {
		flag_found = 1;

		switch (c) {
			case L'-':
				f.justification_options = left_justification;
				break;
			case L'+':
				f.sign_options = sign_always;
				break;
			case L' ':
				if (f.sign_options != sign_always) {
					f.sign_options = space_holder;
				}
				break;
			case L'#':
				f.alternate_form = 1;
				break;
			case L'0':
				if (f.justification_options != left_justification) {
					f.justification_options = zero_fill;
				}
				break;
			default:
				flag_found = 0;
				break;
		}

		if (flag_found) {
			c = *++s;
		}
		else {
			break;
		}
	}

	if (c == L'*') {
		if ((f.field_width = va_arg(*arg, int)) < 0) {
			f.justification_options = left_justification;
			f.field_width = -f.field_width;
		}

		c = *++s;
	}
	else {
		while (iswdigit(c)) {
			f.field_width = (f.field_width * 10) + (c - L'0');
			c = *++s;
		}
	}

	if (f.field_width > 509) {
		f.conversion_char = 0xFFFF;
		*format = f;
		return ((const wchar_t*)s + 1);
	}

	if (c == L'.') {
		f.precision_specified = 1;

		if ((c = *++s) == L'*') {
			if ((f.precision = va_arg(*arg, int)) < 0) {
				f.precision_specified = 0;
			}

			c = *++s;
		}
		else {
			while (iswdigit(c)) {
				f.precision = (f.precision * 10) + (c - L'0');
				c = *++s;
			}
		}
	}

	flag_found = 1;

	switch (c) {
		case L'h':
			f.argument_options = short_argument;

			if (s[1] == L'h') {
				f.argument_options = char_argument;
				c = *++s;
			}

			break;

		case L'l':
			f.argument_options = long_argument;

			if (s[1] == L'l') {
				f.argument_options = long_long_argument;
				c = *++s;
			}

			break;

		case L'L':
			f.argument_options = long_double_argument;
			break;
		case L'j':
			f.argument_options = intmax_argument;
			break;
		case L't':
			f.argument_options = ptrdiff_argument;
			break;
		case L'z':
			f.argument_options = size_t_argument;
			break;
		default:
			flag_found = 0;
			break;
	}

	if (flag_found) {
		c = *++s;
	}

	f.conversion_char = c;

	switch (c) {
		case L'd':
		case L'i':
		case L'u':
		case L'o':
		case L'x':
		case L'X':
			if (f.argument_options == long_double_argument) {
				f.conversion_char = long_long_argument;
			}

			if (!f.precision_specified) {
				f.precision = 1;
			}
			else if (f.justification_options == zero_fill) {
				f.justification_options = right_justification;
			}
			break;

		case L'f':
		case L'F':
			if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
				f.conversion_char = 0xFFFF;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 6;
			}
			break;

		case L'a':
		case L'A':
			if (!f.precision_specified) {
				f.precision = 0xD;
			}

			if (f.argument_options == short_argument || f.argument_options == intmax_argument || 
				f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || 
				f.argument_options == long_long_argument || f.argument_options == char_argument) {
				f.conversion_char = 0xFFFF;
			}

			break;

		case L'g':
		case L'G':
			if (!f.precision) {
				f.precision = 1;
			}

		case L'e':
		case L'E':
			if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument || f.argument_options == char_argument) {
				f.conversion_char = 0xFFFF;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 6;
			}
			break;

		case L'p':
            f.argument_options = long_argument;
            f.alternate_form = 1;
			f.conversion_char = L'x';
			f.precision = 8;
			break;

		case L'c':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			}
			else {
				if (f.precision_specified || f.argument_options != normal_argument) {
					f.conversion_char = 0xFFFF;
				}
			}

			break;

		case L's':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			}
			else {
				if (f.argument_options != normal_argument) {
					f.conversion_char = 0xFFFF;
				}
			}

			break;

		case L'n':
			if (f.argument_options == long_double_argument) {
				f.conversion_char = long_long_argument;
			}
			
			break;

		default:
			f.conversion_char = 0xFFFF;
			break;
	}

	*format = f;
	return ((const wchar_t*)s + 1);
}

wchar_t * long2str(long num, wchar_t * buff, print_format format) {
	unsigned long unsigned_num, base;
	wchar_t* p;
	int n, digits;
	int minus = 0;
	unsigned_num = num;
	minus = 0;
	
	p = buff;	
	*--p = 0;
	digits = 0;
	
	if (!num && !format.precision && !(format.alternate_form && format.conversion_char == L'o')) {
		return p;
    }
	
	switch (format.conversion_char) {
		case L'd':
		case L'i':
			base = 10;
			
			if (num < 0) {
				if (num != 0x80000000) {
					unsigned_num = -num;
                }

				minus = 1;
			}
			break;
		
		case L'o':
			base = 8;
			format.sign_options = only_minus;
			break;
		
		case L'u':
			base = 10;
			format.sign_options = only_minus;
			break;
		
		case L'x':
		case L'X':
			base = 16;
			format.sign_options = only_minus;
			break;
	}
	
	do {
		n = unsigned_num % base;
		unsigned_num /= base;
		
		if (n < 10) {
			n += L'0';
        }
		else {
			n -= 10;
			
			if (format.conversion_char == L'x') {
				n += L'a';
            }
			else {
				n += L'A';
            }
		}
		
		*--p = n;
		++digits;
	}
	while (unsigned_num != 0);
	
	if (base == 8 && format.alternate_form && *p != L'0') {
		*--p = L'0';
		++digits;
	}
	
	if (format.justification_options == zero_fill) {
		format.precision = format.field_width;
		
		if (minus || format.sign_options != only_minus)
			--format.precision;
		
		if (base == 16 && format.alternate_form)
			format.precision -= 2;
	}
	
	if (buff - p + format.precision > 509)
		return 0;
	
	while (digits < format.precision) {
		*--p = L'0';
		++digits;
	}
	
	if (base == 16 && format.alternate_form) {
		*--p = format.conversion_char;
		*--p = L'0';
	}
	
	if (minus) {
		*--p = L'-';
    }
	else if (format.sign_options == sign_always) {
		*--p = L'+';
    }
	else if (format.sign_options == space_holder) {
		*--p = L' ';
    }
	
	return p;
}

wchar_t* longlong2str(long long num, wchar_t *pBuf, print_format fmt) {
	unsigned long long unsigned_num, base;
	wchar_t* p;
	int n, digits;
	int minus = 0;
	unsigned_num = num;
	minus = 0;
	p = pBuf;
	*--p = 0;
	digits = 0;

	if (!num && !fmt.precision && !(fmt.alternate_form && fmt.conversion_char == L'o')) {
		return p;
	}

	switch (fmt.conversion_char) {
		case L'd':
		case L'i':
			base = 10;

			if (num < 0) {
				if (num != 0x8000000000000000) {
					unsigned_num = -num;
				}

				minus = 1;
			}
			break;
		case L'o':
			base = 8;
			fmt.sign_options = only_minus;
			break;
		case L'u':
			base = 10;
			fmt.sign_options = only_minus;
			break;
		case L'x':
		case L'X':
			base = 16;
			fmt.sign_options = only_minus;
			break;
	}

	do {
		n = unsigned_num % base;
		unsigned_num /= base;

		if (n < 10) {
			n += L'0';
		}
		else {
			n -= 10;
			if (fmt.conversion_char == L'x') {
				n += L'a';
			}
			else {
				n += L'A';
			}
		}

		*--p = n;
		++digits;
	} while (unsigned_num != 0);

	if (base == 8 && fmt.alternate_form && *p != L'0') {
		*--p = L'0';
		++digits;
	}

	if (fmt.justification_options == zero_fill) {
		fmt.precision = fmt.field_width;

		if (minus || fmt.sign_options != only_minus) {
			--fmt.precision;
		}

		if (base == 16 && fmt.alternate_form) {
			fmt.precision -= 2;
		}
	}

	if (pBuf - p + fmt.precision > 509) {
		return 0;
	}

	while (digits < fmt.precision) {
		*--p = L'0';
		++digits;
	}

	if (base == 16 && fmt.alternate_form) {
		*--p = fmt.conversion_char;
		*--p = L'0';
	}

	if (minus) {
		*--p = L'-';
	}
	else if (fmt.sign_options == sign_always) {
		*--p = L'+';
	}
	else if (fmt.sign_options == space_holder) {
		*--p = L' ';
	}

	return p;
}

wchar_t * double2hex(long double num, wchar_t * buff, print_format format)  {
	wchar_t *p;	
	unsigned char *q;
	unsigned char working_byte;
	long double ld;
	int expbits, expmask;
	unsigned snum;
	long exp;
	print_format exp_format;
	int hex_precision; 
	int mantissa_bit;
	decform form;		
	decimal dec;	
	int radix_marker;
	
    radix_marker = 	* (unsigned char *)(__lconv).decimal_point;
	p = buff;
	ld = num;
	
	if (format.precision > 509) {
		return 0;
	}

	form.style  = (char)0;
	form.digits = 0x20;           
	__num2dec(&form, num, &dec);

	if (fabsl(num) == 0.0) {
		p = buff - 6;
		wcscpy(p, L"0x0p0");	
		return p;
	}
	
	if (*dec.sig.text == 'I') {
		if (dec.sign) {
			p = buff - 5;
			if (format.conversion_char == L'A')	
				wcscpy(p, L"-INF");
			else					
				wcscpy(p, L"-inf"); 
		}                              
		else {
			p = buff - 4;
			if (format.conversion_char == L'A')	
				wcscpy(p, L"INF");						
			else
				wcscpy(p, L"inf");     
		}                              
		
		return p;
	}
	else if (*dec.sig.text == 'N') {
		if (dec.sign) {
			p = buff - 5;              
			if (format.conversion_char == L'A')	
				wcscpy(p, L"-NAN");
			else					
				wcscpy(p, L"-nan"); 
		}                              
		else {
			p = buff - 4;              
			if (format.conversion_char == L'A')	
				wcscpy(p, L"NAN");						
			else										
				wcscpy(p, L"nan");     
		}                              
		 return p;
	}

	exp_format.justification_options = 1;
	exp_format.sign_options = 1;
	exp_format.precision_specified = 0;
	exp_format.alternate_form = 0;
	exp_format.argument_options = 0;
	exp_format.field_width = 0;
	exp_format.precision = 1;
	exp_format.conversion_char = 'd';

	expbits = 11;
	expmask = 0x7FF;

	snum = ((unsigned char *)&num)[0] << 25;
	if (TARGET_FLOAT_EXP_BITS > 7)
		snum |= ((unsigned char *)&num)[1] << 17;
	if (TARGET_FLOAT_EXP_BITS > 15)
		snum |= ((unsigned char *)&num)[2] << 9;
	if (TARGET_FLOAT_EXP_BITS > 23)
		snum |= ((unsigned char *)&num)[3] << 1;

	snum	= (snum >> (32 - expbits)) & expmask;
	exp		= snum - 0x3FF;

	p = long2str(exp, buff, exp_format);
	if (format.conversion_char == L'a')
		*--p = L'p';
	else
		*--p = L'P';
	q = (unsigned char *)&num;

	if (TARGET_FLOAT_IMPLICIT_J_BIT) {
		mantissa_bit = (1 + expbits + format.precision * 4) - 1;
	}
	else {
		mantissa_bit = (1 + expbits + format.precision * 4) - 4;
	}
	
	for (hex_precision = format.precision; hex_precision >= 1; hex_precision--) {
		if (mantissa_bit < 64) {
			int mantissa_byte;
			
			mantissa_byte = mantissa_bit >> 3;
			working_byte = (*(q + mantissa_byte)) >> (7 - (mantissa_bit & 7));

			if ((mantissa_bit & ~7) != ((mantissa_bit - 4) & ~7)) {
				working_byte |= ((*(q + (mantissa_byte - 1))) << 8) >> (7 - ((mantissa_bit) & 7));
			}

			if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
				if (mantissa_bit == 1 + expbits) {
					*--p = radix_marker;							
					working_byte &= 0x1;
				}
			}
			
			if ((working_byte &= 0xF) < 10) {
				working_byte += L'0';
			}
			else
				if (format.conversion_char == L'a') {
					working_byte += L'a' - 10;
				}
				else {
					working_byte += L'A' - 10;
				}
		}
		else {
			working_byte = L'0';
		} 

		*--p = working_byte;
		mantissa_bit -= 4;
	}
	
	if (TARGET_FLOAT_IMPLICIT_J_BIT){
		if (format.precision || format.alternate_form) {
			*--p = radix_marker;
		}

		*--p = L'1'; 
	}

	if (format.conversion_char == L'a') {
		*--p = L'x';
	}
	else {
		*--p = L'X';
	}

	*--p = L'0';

	if (dec.sign) {
		*--p = L'-';
	}
	else if (format.sign_options == 1) {
		*--p = L'+';
	}
	else if (format.sign_options == 2) {
		*--p = L' ';
	}

	return p;
}

void round_decimal(decimal *dec, int new_length) {
	char c;
	char* p;
	int carry;

	if (new_length < 0) {
return_zero:
		dec->exponent = 0;
		dec->sig.length = 1;
		*dec->sig.text = '0';
		return;
	}

	if (new_length >= dec->sig.length) {
		return;
	}

	p = (char*)dec->sig.text + new_length + 1;
	c = *--p - L'0';

	if (c == 5) {
		char* q = &((char*)dec->sig.text)[dec->sig.length];

		while (--q > p && *q == '0');
		carry = (q == p) ? p[-1] & 1 : 1;
	}
	else {
		carry = (c > 5);
	}

	while (new_length != 0) {
		c = *--p - L'0' + carry;

		if ((carry = (c > 9)) != 0 || c == 0) {
			--new_length;
		}
		else {
			*p  = c + L'0';
			break;
		}
	}

	if (carry != 0) {
		dec->exponent += 1;
		dec->sig.length = 1;
		*dec->sig.text = '1';
		return;
	}
	else if (new_length == 0) {
		goto return_zero;
	}

	dec->sig.length = new_length;
}

wchar_t* float2str(long double num, wchar_t *wbuff, print_format format) {
	decimal dec;
	decform form;
    wchar_t* pw;
	char* p;
    char buff[512];	
	char* q;
	int n, digits, sign;
	int int_digits, frac_digits;
	int radix_marker;

	radix_marker = *(unsigned char*)__lconv.decimal_point;

	if (format.precision > 509) {
		return 0;
	}

	form.style = 0;
	form.digits = 0x20;
	__num2dec(&form, num, &dec);
	p = (char*)dec.sig.text + dec.sig.length;

	while (dec.sig.length > 1 && *--p == '0') {
		--dec.sig.length;
		++dec.exponent;
	}

	switch (*dec.sig.text) {
		case '0':
			dec.exponent = 0;
			break;
		case 'I':
			if (num < 0) {
				pw = wbuff - 5;

				if (iswupper(format.conversion_char)) {
					wcscpy(pw, L"-INF");
				}
				else {
					wcscpy(pw, L"-inf");
				}
			}
			else {
				pw = wbuff - 4;
				if (iswupper(format.conversion_char)) {
					wcscpy(pw, L"INF");
				}
				else {
					wcscpy(pw, L"inf");
				}
			}

			return pw;

		case 'N':
			if (dec.sign) {
				pw = wbuff - 5;

				if (iswupper(format.conversion_char)) {
					wcscpy(pw, L"-NAN");
				}
				else {
					wcscpy(pw, L"-nan");
				}
			}
			else {
				pw = wbuff - 4;
				if (iswupper(format.conversion_char)) {
					wcscpy(pw, L"NAN");
				}
				else {
					wcscpy(pw, L"nan");
				}
			}

			return pw;
	}

	dec.exponent += dec.sig.length - 1;
	p = buff + 512;
	*--p = 0;

	switch (format.conversion_char)
	{
		case L'g':
		case L'G':
			
			if (dec.sig.length > format.precision) {
				round_decimal(&dec, format.precision);
			}
			
			if (dec.exponent < -4 || dec.exponent >= format.precision)
			{
				if (format.alternate_form) {
					--format.precision;
				}
				else {
					format.precision = dec.sig.length - 1;
				}
				
				if (format.conversion_char == L'g') {
					format.conversion_char = L'e';
				}
				else {
					format.conversion_char = L'E';
				}
				
				goto e_format;
			}
			
			if (format.alternate_form) {
				format.precision -= dec.exponent + 1;
			}
			else {
				if ((format.precision = dec.sig.length - (dec.exponent + 1)) < 0) {
					format.precision = 0;
				}
			}
			
			goto f_format;
		
		case L'e':
		case L'E':
		e_format:
			
			if (dec.sig.length > format.precision + 1) {
				round_decimal(&dec, format.precision + 1);
			}
			
			n    = dec.exponent;
			sign = '+';
			
			if (n < 0) {
				n    = -n;
				sign = '-';
			}
			
			for (digits = 0; n || digits < 2; ++digits) {
				*--p = n % 10 + '0';
				n /= 10;
			}
			
			*--p = sign;
			*--p = format.conversion_char;
			
			if (buff - p + format.precision > 509) {
				return 0;
			}
			
			if (dec.sig.length < format.precision + 1) {
				for (n = format.precision + 1 - dec.sig.length + 1; --n;) {
					*--p = '0';
				}
			}
			
			for (n = dec.sig.length, q = (char*)dec.sig.text + dec.sig.length; --n;) {
				*--p = *--q;
			}
			
			if (format.precision || format.alternate_form) {
				*--p = radix_marker;
			}
			
			*--p = *dec.sig.text;
			
			if (dec.sign)
				*--p = '-';
			else if (format.sign_options == sign_always)
				*--p = '+';
			else if (format.sign_options == space_holder)
				*--p = ' ';
			
			break;

		case L'f':
		case L'F':
		f_format:
			
			if ((frac_digits = -dec.exponent + dec.sig.length - 1) < 0)
				frac_digits = 0;
			
			if (frac_digits > format.precision) {
				round_decimal(&dec, dec.sig.length - (frac_digits - format.precision));
				
				if ((frac_digits = -dec.exponent + dec.sig.length - 1) < 0)
					frac_digits = 0;
			}
			
			if ((int_digits = dec.exponent + 1) < 0)
				int_digits = 0;
			
			if (int_digits + frac_digits > 509)
				return 0;
			
			q = (char *) dec.sig.text + dec.sig.length;
			
			for (digits = 0; digits < (format.precision - frac_digits); ++digits)
				*--p = '0';
			
			for (digits = 0; digits < frac_digits && digits < dec.sig.length; ++digits)
				*--p = *--q;
			
			for (; digits < frac_digits; ++digits)
				*--p = '0';
			
			if (format.precision || format.alternate_form)
				*--p = radix_marker;
			
			if (int_digits) {
				for (digits = 0; digits < int_digits - dec.sig.length; ++digits) {
					*--p = '0';
				}

				for (; digits < int_digits; ++digits) {
					*--p = *--q;
				}
			}
			else {
				*--p = '0';
			}
			
			if (dec.sign) {
				*--p = '-';
			}
			else if (format.sign_options == sign_always) {
				*--p = '+';
			}
			else if (format.sign_options == space_holder) {
				*--p = ' ';
			}
			
			break;
	}

    pw = wbuff - strlen(p) - 1;
    mbstowcs(pw, p, strlen(p));
	return pw;
}

int __wpformatter(void *(*WriteProc)(void *, const wchar_t *, size_t), void *WriteProcArg, const wchar_t * format_str, va_list arg) {
	int num_chars, chars_written, field_width;
	const wchar_t* format_ptr;
	const wchar_t* curr_format;
	print_format format;
	long long_num;
	long long long_long_num;
	long double long_double_num;
	wchar_t buff[512];
	wchar_t* buff_ptr;
	wchar_t* string_end;
    char* cstring_end;
	wchar_t fill_char = L' ';
    char* strptr;

	format_ptr = format_str;
	chars_written = 0;

	while (*format_ptr) {
		if (!(curr_format = wcschr(format_ptr, L'%'))) {
			num_chars = wcslen(format_ptr);
			chars_written += num_chars;

			if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
				return -1;
			}

			break;
		}

		num_chars = curr_format - format_ptr;
		chars_written += num_chars;

		if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
			return -1;
		}

		format_ptr = curr_format;
		format_ptr = parse_format(format_ptr, (va_list*)arg, &format);

		switch (format.conversion_char) {
			case L'd':
			case L'i':
				if (format.argument_options == long_argument) {
					long_num = va_arg(arg, long);
				}
				else if (format.argument_options == long_long_argument) {
					long_long_num = va_arg(arg, long long);
				}
				else if (format.argument_options == intmax_argument) {
					long_long_num = va_arg(arg, intmax_t);
				}
				else if (format.argument_options == size_t_argument) {
					long_num = va_arg(arg, size_t);
				}
				else if (format.argument_options == ptrdiff_argument) {
					long_num = va_arg(arg, ptrdiff_t);
				}
				else {
					long_num = va_arg(arg, int);
				}

				if (format.argument_options == short_argument) {
					long_num = (short)long_num;
				}

				if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument)) {
					if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
						goto conversion_error;
					}
				}
				else {
					if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
						goto conversion_error;
					}
				}

				num_chars = buff + 512 - 1 - buff_ptr;
				break;

			case L'o':
			case L'u':
			case L'x':
			case L'X':
				if (format.argument_options == long_argument) {
					long_num = va_arg(arg, unsigned long);
				}
				else if (format.argument_options == long_long_argument) {
					long_long_num = va_arg(arg, long long);
				}
				else if (format.argument_options == intmax_argument) {
					long_long_num = va_arg(arg, intmax_t);
				}
				else if (format.argument_options == size_t_argument) {
					long_num = va_arg(arg, size_t);
				}
				else if (format.argument_options == ptrdiff_argument) {
					long_num = va_arg(arg, ptrdiff_t);
				}
				else {
					long_num = va_arg(arg, unsigned int);
				}

				if (format.argument_options == short_argument) {
					long_num = (unsigned short)long_num;
				}

				if ((format.argument_options == long_long_argument) || (format.argument_options == intmax_argument)) {
					if (!(buff_ptr = longlong2str(long_long_num, buff + 512, format))) {
						goto conversion_error;
					}
				}
				else {
					if (!(buff_ptr = long2str(long_num, buff + 512, format))) {
						goto conversion_error;
					}
				}

				num_chars = buff + 512 - 1 - buff_ptr;
				break;

			case L'f':
			case L'F':
			case L'e':
			case L'E':
			case L'g':
			case L'G':
				if (format.argument_options == long_double_argument) {
					long_double_num = va_arg(arg, long double);
				}
				else {
					long_double_num = va_arg(arg, double);
				}

				if (!(buff_ptr = float2str(long_double_num, buff + 512, format))) {
					goto conversion_error;
				}

				num_chars = buff + 512 - 1 - buff_ptr;
				break;

			case 'a':
			case 'A':
				if (format.argument_options == long_double_argument) {
					long_double_num = va_arg(arg, long double);
				}
				else {
					long_double_num = va_arg(arg, double);
				}

				if (!(buff_ptr = double2hex(long_double_num, buff + 512, format))) {
					goto conversion_error;
				}

				num_chars = buff + 512 - 1 - buff_ptr;
				break;

			case L's':
				if (format.argument_options == wchar_argument) {
					buff_ptr = va_arg(arg, wchar_t *);

					if (buff_ptr == 0) {
						buff_ptr = L"";
					}

					if (format.alternate_form)
					{
						num_chars = (unsigned char) *buff_ptr++;
						
						if (format.precision_specified && num_chars > format.precision)
							num_chars = format.precision;
					}
					else if (format.precision_specified)
					{
						num_chars = format.precision;
						
						if ((string_end = (wchar_t *)wmemchr(buff_ptr, 0, num_chars)) != 0) {
							num_chars = string_end - buff_ptr;
                        }
					}
					else
						num_chars = wcslen(buff_ptr);
				}
				else {
					strptr = va_arg(arg, char *);
					if (strptr == 0) {
	  					strptr = "";
                    }

					if (format.alternate_form) {
						num_chars = (unsigned char) *buff_ptr++;
						
						if (format.precision_specified && num_chars > format.precision) {
							num_chars = format.precision;
                        }
					}
					else if (format.precision_specified) {
						num_chars = format.precision;
						
						if ((cstring_end = (char *) memchr(strptr, 0, num_chars)) != 0) {
							num_chars = cstring_end - strptr;
                        }
					}
					else
						num_chars = strlen(strptr);
	  				if ((num_chars = mbstowcs(buff, strptr, num_chars)) < 0)
						goto conversion_error;
	  				
	  				buff_ptr = &buff[0];
				}

				break;

			case 'n':
				buff_ptr = va_arg(arg, wchar_t *);

				switch (format.argument_options) {
					case normal_argument:
						*(int*)buff_ptr = chars_written;
						break;
					case short_argument:
						*(short*)buff_ptr = chars_written;
						break;
					case long_argument:
						*(long*)buff_ptr = chars_written;
						break;
					case intmax_argument:
						*(intmax_t*)buff_ptr = chars_written;
						break;
					case size_t_argument:
						*(size_t*)buff_ptr = chars_written;
						break;
					case ptrdiff_argument:
						*(ptrdiff_t*)buff_ptr = chars_written;
						break;
					case long_long_argument:
						*(long long*)buff_ptr = chars_written;
						break;
				}

				continue;

			case L'c':
				buff_ptr = buff;
				if (format.argument_options == wchar_argument) {
					*buff_ptr = va_arg(arg, int);
					num_chars = 1;
				}
				else {
					char chint = va_arg(arg, int);
					num_chars = mbtowc(buff, &chint, 1);
				}
				
				break;

			case L'%':
				buff_ptr = buff;
				*buff_ptr = '%';
				num_chars = 1;
				break;
			
			case 0xFFFF:
			default:
				conversion_error:
					num_chars = wcslen(curr_format);
					chars_written += num_chars;

					if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, num_chars)) {
						return -1;
					}

					return chars_written;
					break;
		}

		field_width = num_chars;

		if (format.justification_options != left_justification) {
			fill_char = (format.justification_options == zero_fill) ? L'0' : L' ';

			if (((*buff_ptr == L'+') || (*buff_ptr == L'-') || (*buff_ptr == L' ')) && (fill_char == L'0')) {
				if ((*WriteProc)(WriteProcArg, buff_ptr, 1) == 0) {
					return -1;
				}

				++buff_ptr;
				num_chars--;
			}

			while (field_width < format.field_width) {
				if ((*WriteProc)(WriteProcArg, &fill_char, 1) == 0) {
					return -1;
				}

				++field_width;
			}
		}

		if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, num_chars)) {
			return -1;
		}

		if (format.justification_options == left_justification) {
			while (field_width < format.field_width) {
				wchar_t blank = L' ';

				if ((*WriteProc)(WriteProcArg, &blank, 1) == 0) {
					return -1;
				}

				++field_width;
			}
		}

		chars_written += field_width;
	}

	return chars_written;
}

void* __wStringWrite(void *wosc, const wchar_t * buffer, size_t numchars) {
    size_t tobewritten;
    void* res;
    __wOutStrCtrl * wOscp = (__wOutStrCtrl*)wosc;
    tobewritten = ((wOscp->CharsWritten + numchars) <= wOscp->MaxCharCount) ? numchars : wOscp->MaxCharCount - wOscp->CharsWritten;
    res = (void*)wmemcpy(wOscp->wCharStr + wOscp->CharsWritten, buffer, tobewritten);
    wOscp->CharsWritten += tobewritten;
    return res;
}

int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...) {
    va_list args;
    va_start(args, format);
    return vswprintf(s, n, format, args);
}

int vswprintf(wchar_t *s, size_t n, const wchar_t *format, va_list arg) {
    int count;
    __wOutStrCtrl wosc;
    wosc.wCharStr = s;
    wosc.MaxCharCount = n;
    wosc.CharsWritten = 0;

    count = __wpformatter(&__wStringWrite, &wosc, format, arg);

    if (count >= 0) {
        if (count < n) {
            s[count] = 0;
        }
        else {
            s[n - 1] = 0;
            count = -1;
        }
    }

    return count;
}
