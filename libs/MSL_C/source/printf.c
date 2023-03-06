#include <cctype>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include "ansi_fp.h"
#include "wchar_io.h"

#define LDBL_MANT_DIG          24
#define LDBL_MAX_EXP          128
#define TARGET_FLOAT_BITS 32
#define TARGET_FLOAT_BYTES	(TARGET_FLOAT_BITS/8)
#define TARGET_FLOAT_MAX_EXP	LDBL_MAX_EXP
#define TARGET_FLOAT_MANT_DIG	LDBL_MANT_DIG
#define TARGET_FLOAT_IMPLICIT_J_BIT 1
#define TARGET_FLOAT_MANT_BITS		(TARGET_FLOAT_MANT_DIG - TARGET_FLOAT_IMPLICIT_J_BIT)
#define TARGET_FLOAT_EXP_BITS		(TARGET_FLOAT_BITS - TARGET_FLOAT_MANT_BITS - 1)

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
	unsigned char	justification_options;		// _0
	unsigned char 	sign_options;				// _1
	unsigned char 	precision_specified;		// _2
	unsigned char	alternate_form;				// _3
	unsigned char 	argument_options;			// _4
	unsigned char	conversion_char;			// _5
	int			  	field_width;				// _8
	int				precision;					// _C
} print_format;

typedef struct {
	char* CharStr;
	size_t MaxCharCount;
	size_t CharsWritten;
} __OutStrCtrl;

/*static*/ const char* parse_format(const char *format_string, va_list *arg, print_format *format) {
	print_format f;
	const char* s = format_string;
	int c;
	int flag_found;
	f.justification_options = right_justification;
	f.sign_options = only_minus;
	f.precision_specified = 0;
	f.alternate_form = 0;
	f.argument_options = normal_argument;
	f.field_width = 0;
	f.precision = 0;
	
	if ((c = *++s) == '%') {
		f.conversion_char = c;
		*format = f;
		return ((const char*)s + 1);
	}

	for (;;) {
		flag_found = 1;

		switch (c) {
			case '-':
				f.justification_options = left_justification;
				break;
			case '+':
				f.sign_options = sign_always;
				break;
			case ' ':
				if (f.sign_options != sign_always) {
					f.sign_options = space_holder;
				}
				break;
			case '#':
				f.alternate_form = 1;
				break;
			case '0':
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

	if (c == '*') {
		if ((f.field_width = va_arg(*arg, int)) < 0) {
			f.justification_options = left_justification;
			f.field_width = -f.field_width;
		}

		c = *++s;
	}
	else {
		while (isdigit(c)) {
			f.field_width = (f.field_width * 10) + (c - '0');
			c = *++s;
		}
	}

	if (f.field_width > 509) {
		f.conversion_char = 0xFF;
		*format = f;
		return ((const char*)s + 1);
	}

	if (c == '.') {
		f.precision_specified = 1;

		if ((c = *++s) == '*') {
			if ((f.precision = va_arg(*arg, int)) < 0) {
				f.precision_specified = 0;
			}

			c = *++s;
		}
		else {
			while (isdigit(c)) {
				f.precision = (f.precision * 10) + (c - '0');
				c = *++s;
			}
		}
	}

	flag_found = 1;

	switch (c) {
		case 'h':
			f.argument_options = short_argument;

			if (s[1] == 'h') {
				f.argument_options = char_argument;
				c = *++s;
			}

			break;

		case 'l':
			f.argument_options = long_argument;

			if (s[1] == 'l') {
				f.argument_options = long_long_argument;
				c = *++s;
			}

			break;

		case 'L':
			f.argument_options = long_double_argument;
			break;
		case 'j':
			f.argument_options = intmax_argument;
			break;
		case 't':
			f.argument_options = ptrdiff_argument;
			break;
		case 'z':
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
		case 'd':
		case 'i':
		case 'u':
		case 'o':
		case 'x':
		case 'X':
			if (f.argument_options == long_double_argument) {
				f.conversion_char = 0xFF;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 1;
			}
			else if (f.justification_options == zero_fill) {
				f.justification_options = right_justification;
			}
			break;

		case 'f':
		case 'F':
			if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument) {
				f.conversion_char = 0xFF;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 1;
			}
			break;

		case 'a':
		case 'A':
			if (!f.precision_specified) {
				f.precision = 0xD;
			}

			if (f.argument_options == short_argument || f.argument_options == intmax_argument || 
				f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || 
				f.argument_options == long_long_argument || f.argument_options == char_argument) {
				f.conversion_char = 0xFF;
			}

			break;

		case 'g':
		case 'G':
			if (!f.precision) {
				f.precision = 1;
			}

		case 'e':
		case 'E':
			if (f.argument_options == short_argument || f.argument_options == intmax_argument || f.argument_options == size_t_argument || f.argument_options == ptrdiff_argument || f.argument_options == long_long_argument || f.argument_options == char_argument) {
				f.conversion_char = 0xFF;
				break;
			}

			if (!f.precision_specified) {
				f.precision = 6;
			}
			break;

		case 'p':
			f.conversion_char = 'x';
			f.alternate_form = 1;
			f.argument_options = long_argument;
			f.precision = 8;
			break;

		case 'c':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			}
			else {
				if (f.precision_specified || f.argument_options != normal_argument) {
					f.conversion_char = 0xFF;
				}
			}

			break;

		case 's':
			if (f.argument_options == long_argument) {
				f.argument_options = wchar_argument;
			}
			else {
				if (f.argument_options != normal_argument) {
					f.conversion_char = 0xFF;
				}
			}

			break;

		case 'n':
			if (f.argument_options == long_double_argument) {
				f.conversion_char = 0xFF;
			}
			
			break;

		default:
			f.conversion_char = 0xFF;
			break;
	}

	*format = f;
	return ((const char*)s + 1);
}

static char * long2str(long num, char * buff, print_format format) {
	unsigned long unsigned_num, base;
	char* p;
	int n, digits;
	int minus = 0;
	unsigned_num = num;
	minus = 0;
	
	p = buff;	
	*--p = 0;
	digits = 0;
	
	if (!num && !format.precision && !(format.alternate_form && format.conversion_char == 'o')) {
		return p;
    }
	
	switch (format.conversion_char) {
		case 'd':
		case 'i':
			base = 10;
			
			if (num < 0) {
				if (num != 0x80000000) {
					unsigned_num = -num;
                }

				minus = 1;
			}
			break;
		
		case 'o':
			base = 8;
			format.sign_options = only_minus;
			break;
		
		case 'u':
			base = 10;
			format.sign_options = only_minus;
			break;
		
		case 'x':
		case 'X':
			base = 16;
			format.sign_options = only_minus;
			break;
	}
	
	do {
		n = unsigned_num % base;
		unsigned_num /= base;
		
		if (n < 10) {
			n += '0';
        }
		else {
			n -= 10;
			
			if (format.conversion_char == 'x') {
				n += 'a';
            }
			else {
				n += 'A';
            }
		}
		
		*--p = n;
		++digits;
	}
	while (unsigned_num != 0);
	
	if (base == 8 && format.alternate_form && *p != '0') {
		*--p = '0';
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
		return (0);
	
	while (digits < format.precision) {
		*--p = '0';
		++digits;
	}
	
	if (base == 16 && format.alternate_form) {
		*--p = format.conversion_char;
		*--p = '0';
	}
	
	if (minus) {
		*--p = '-';
    }
	else if (format.sign_options == sign_always) {
		*--p = '+';
    }
	else if (format.sign_options == space_holder) {
		*--p = ' ';
    }
	
	return p;
}

char* longlong2str(long long num, char *pBuf, print_format fmt) {
	unsigned long long unsigned_num, base;
	char* p;
	int n, digits;
	int minus = 0;
	unsigned_num = num;
	minus = 0;
	p = pBuf;
	*--p = 0;
	digits = 0;

	if (!num && !fmt.precision && !(fmt.alternate_form && fmt.conversion_char == 'o')) {
		return p;
	}

	switch (fmt.conversion_char) {
		case 'd':
		case 'i':
			base = 10;

			if (num < 0) {
				if (num != 0x8000000000000000) {
					unsigned_num = -num;
				}

				minus = 1;
			}
			break;
		case 'o':
			base = 8;
			fmt.sign_options = only_minus;
			break;
		case 'u':
			base = 10;
			fmt.sign_options = only_minus;
			break;
		case 'x':
		case 'X':
			base = 16;
			fmt.sign_options = only_minus;
			break;
	}

	do {
		n = unsigned_num % base;
		unsigned_num /= base;

		if (n < 10) {
			n += '0';
		}
		else {
			n -= 10;
			if (fmt.conversion_char == 'x') {
				n += 'a';
			}
			else {
				n += 'A';
			}
		}

		*--p = n;
		++digits;
	} while (unsigned_num != 0);

	if (base == 8 && fmt.alternate_form && *p != '0') {
		*--p = '0';
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
		*--p = '0';
		++digits;
	}

	if (base == 16 && fmt.alternate_form) {
		*--p = fmt.conversion_char;
		*--p = '0';
	}

	if (minus) {
		*--p = '-';
	}
	else if (fmt.sign_options == sign_always) {
		*--p = '+';
	}
	else if (fmt.sign_options == space_holder) {
		*--p = ' ';
	}

	return p;
}


#ifdef NON_MATCHING
/*static*/ char * double2hex(long double num, char * buff, print_format format)  {
	char *p;	
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
		strcpy(p, "0x0p0");	
		return p;
	}
	
	if (*dec.sig.text == 'I') {
		if (dec.sign) {
			p = buff - 5;
			if (format.conversion_char == 'A')	
				strcpy(p, "-INF");
			else					
				strcpy(p, "-inf"); 
		}                              
		else {
			p = buff - 4;
			if (format.conversion_char == 'A')	
				strcpy(p, "INF");						
			else
				strcpy(p, "inf");     
		}                              
		
		return p;
	}
	else if (*dec.sig.text == 'N') {
		if (dec.sign) {
			p = buff - 5;              
			if (format.conversion_char == 'A')	
				strcpy(p, "-NAN");
			else					
				strcpy(p, "-nan"); 
		}                              
		else {
			p = buff - 4;              
			if (format.conversion_char == 'A')	
				strcpy(p, "NAN");						
			else										
				strcpy(p, "nan");     
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

	expbits = 6;
	expmask = 255;

	snum = ((unsigned char *)&num)[0] << 25;
	if (TARGET_FLOAT_EXP_BITS > 7)
		snum |= ((unsigned char *)&num)[1] << 17;
	if (TARGET_FLOAT_EXP_BITS > 15)
		snum |= ((unsigned char *)&num)[2] << 9;
	if (TARGET_FLOAT_EXP_BITS > 23)
		snum |= ((unsigned char *)&num)[3] << 1;

	snum	= (snum >> (32 - expbits)) & expmask;
	exp		= snum - 127;

	p = long2str(exp, buff, exp_format);
	if (format.conversion_char == 'a')
		*--p = 'p';
	else
		*--p = 'P';
	q = (unsigned char *)&num;

	if (TARGET_FLOAT_IMPLICIT_J_BIT)
		mantissa_bit = (1 + expbits + format.precision * 4) - 1;
	else
		mantissa_bit = (1 + expbits + format.precision * 4) - 4;
	
	for (hex_precision = format.precision; hex_precision >= 1; hex_precision--)
	{
		if (mantissa_bit < 32)
		{
			int mantissa_byte;
			
			mantissa_byte = mantissa_bit >> 3;
			working_byte = (*(q + mantissa_byte)) >> (7 - (mantissa_bit & 7));

			if ((mantissa_bit & ~7) != ((mantissa_bit - 4) & ~7))
				working_byte |= ((*(q + (mantissa_byte - 1))) << 8) >> (7 - ((mantissa_bit) & 7));

			if (!TARGET_FLOAT_IMPLICIT_J_BIT) {
				if (mantissa_bit == 1 + expbits)
				{
					*--p = radix_marker;							
					working_byte &= 0x1;
				}
			}
			
			if ((working_byte &= 0x0f) < 10)
				working_byte += '0';
			else
				if (format.conversion_char == 'a')
					working_byte += 'a' - 10;
				else
					working_byte += 'A' - 10;
		}
		else
			working_byte = '0'; 

		*--p = working_byte;
		mantissa_bit -= 4;
	}
	
	if (TARGET_FLOAT_IMPLICIT_J_BIT)
	{
		if (format.precision || format.alternate_form)
			*--p = radix_marker;
		*--p = '1'; 
	}
	if (format.conversion_char == 'a')
		*--p = 'x';
	else
		*--p = 'X';
	*--p = '0';
	if (dec.sign)
		*--p = '-';
	else if (format.sign_options == 1)
		*--p = '+';
	else if (format.sign_options == 2)
		*--p = ' ';

	return p;
}
#endif

extern int __pformatter(void *(*WriteProc)(void *, const char *, size_t), void *, const char *, va_list);

void* __FileWrite(void *pFile, const char *pBuffer, size_t char_num) {
    return (__fwrite(pBuffer, 1, char_num, (FILE*)pFile) == char_num ? pFile : 0);
}

void* __StringWrite(void *pCtrl, const char *pBuffer, size_t char_num) {
	size_t chars;
	void* res;
	__OutStrCtrl* ctrl = (__OutStrCtrl*)pCtrl;

	chars = ((ctrl->CharsWritten + char_num) <= ctrl->MaxCharCount) ? char_num : ctrl->MaxCharCount - ctrl->CharsWritten;
	res = (void*)memcpy(ctrl->CharStr + ctrl->CharsWritten, pBuffer, chars);
	ctrl->CharsWritten += chars;
	return (void*)1;
}

int fprintf(FILE *pFile, const char *format, ...) {
	int res;

	if (fwide(pFile, -1) >= 0) {
		return -1;
	}

	{
		va_list args;
		va_start(args, format);
		res = __pformatter(&__FileWrite, (void*)pFile, format, args);
		return res;
	}
}

int vsnprintf(char *s, size_t n, const char *format, va_list arg) {
	int end;
	__OutStrCtrl osc;
	osc.CharStr = s;
	osc.MaxCharCount = n;
	osc.CharsWritten = 0;

	end = __pformatter(&__StringWrite, &osc, format, arg);

	if (s) {
		if (end < n) {
			s[end] = '\0';
		}
		else {
			if (n > 0) {
				s[n - 1] = '\0';
			}
		}
	}

	return end;
}

int vsprintf(char *s, const char *format, va_list arg) {
	return vsnprintf(s, 0xFFFFFFFF, format, arg);
}

int vprintf(const char *pFormat, va_list arg) {
    int ret;

    if (fwide(stdout, -1) >= 0) {
        return -1;
    }

    ret = __pformatter(&__FileWrite, (void*)stdout, pFormat, arg);
    return ret;
} 

int snprintf(char *s, size_t n, const char *format, ...) {
	va_list args;
	va_start(args, format);
	return vsnprintf(s, n, format, args);
}

int sprintf(char *s, const char *format, ...) {
	va_list args;
	va_start(args, format);
	return vsnprintf(s, 0xFFFFFFFF, format, args);
}