#include <cstdio>
#include "wchar_io.h"

enum justification_options
{
	left_justification,
	right_justification,
	zero_fill
};

enum sign_options
{
	only_minus,
	sign_always,
	space_holder
};

typedef struct {
	unsigned char	justification_options;
	unsigned char 	sign_options;
	unsigned char 	precision_specified;
	unsigned char	alternate_form;
	unsigned char 	argument_options;
	unsigned char	conversion_char;
	int			  	field_width;
	int				precision;
} print_format;

char * long2str(long num, char * buff, print_format format) {
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

extern int __pformatter(void *(*WriteProc)(void *, const char *, size_t), void *, const char *, va_list);

void* __FileWrite(void *, const char *, size_t) {
    return;
}

int vprintf(const char *pFormat, va_list arg) {
    int ret;

    if (fwide(stdout, -1) >= 0) {
        return -1;
    }

    ret = __pformatter(&__FileWrite, (void*)stdout, pFormat, arg);
    return ret;
}