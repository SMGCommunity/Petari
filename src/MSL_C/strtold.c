#include "stdio_api.h"
#include "ansi_fp.h"
#include "locale.h"
#include <cctype>
#include <cmath>
#include "mem.h"
#pragma exceptions on

extern double nan(const char *);
extern double __dec2num(const decimal *);

#define LDBL_MAX					0x1.fffffffffffffP1023L
#define LDBL_EPSILON				0x1.0000000000000P-52L
#define LDBL_MIN					0x1.0000000000000P-1022L

#define _MSL_SHRT_MAX				0x7FFF
#define _MSL_USHRT_MAX				0x7FFF
#define _MSL_SHRT_MIN				(~_MSL_SHRT_MAX)

#define SHRT_MAX					_MSL_SHRT_MAX
#define SHRT_MIN					_MSL_SHRT_MIN

#define final_state(scan_state)	(scan_state & (2048 | 4096))
#define success(scan_state) (scan_state & 3628)
#define hex_success(count, scan_state) (count-1 > 2 && scan_state & (398))

#define fetch() (count++, (*ReadProc)(ReadProcArg, 0, __GetAChar))
#define unfetch(c)	(*ReadProc)(ReadProcArg, c, __UngetAChar)	

long double __strtold(int max_width, int (*ReadProc)(void *, int, int), void* ReadProcArg, int* chars_scanned, int* overflow) {
	int scan_state = 1;
	int hex_scan_state	= 0;
	int count = 0;
	int spaces = 0;
	int c;
	decimal d = {0, 0, 0, {0, ""}};
	int sig_negative = 0;
	int exp_negative = 0;
	long exp_value = 0;
	int exp_adjust = 0;
	register long double result = 0.0;
	int sign_detected	= 0;
	int radix_marker = *(unsigned char*)(__lconv).decimal_point;
	
	unsigned char*	chptr;
	unsigned char	mantissa[8];
	unsigned mantissa_digits;
	unsigned long exponent = 0;

	int ui;
	unsigned char uch, uch1;
	int NibbleIndex;
	int expsign = 0; 
	int exp_digits = 0; 
	unsigned intdigits = 0;

	*overflow = 0;
	c = fetch();
	
	while (count <= max_width && c != -1 && !final_state(scan_state)) {
		switch (scan_state) {
			case 1:
				if (isspace(c)) {
					c = fetch();
					count--;
					spaces++;
					break;
				}

				switch(toupper(c)) {
					case '-':
						sig_negative = 1;
						
					case '+':
						c = fetch();
						sign_detected = 1;
						break;
					case 'I':
						c = fetch();
						scan_state = 16384;
						break;
						
					case 'N':
						c = fetch();
						scan_state = 8192;
						break;
				
					default:
						scan_state = 2;
						break;
				}
				break;

			case 16384: {
				int i = 1;
				char model[] = "INFINITY";

				while((i < 8) && (toupper(c) == model[i])) {
					i++;
					c = fetch();
				}

				if((i == 3) || (i == 8)) {
					if (sig_negative) {
						result = -INFINITY;
						/* TODO -- I AM A HACK */
						__asm {
							frsp result, result
						};
					}
					else {
						result = INFINITY;
					}

					*chars_scanned = spaces + i + sign_detected;
					return result;
				}
				else {
					scan_state = 4096;
				}

				break;
			}
			
			case 8192: {
				int i = 1, j = 0;
				char model[] = "NAN(";
				char nan_arg[32] = "";
				while((i < 4) && (toupper(c) == model[i])) {
					i++;
					c = fetch();
				}

				if ((i == 3) || (i == 4)) {
					if (i == 4) {
						while((j < 32) && (isdigit(c) || isalpha(c) || (c == radix_marker))) {
							nan_arg[j++] = (char)c;
							c = fetch();
						}

						if (c != ')') {
							scan_state = 4096;
							break;
						}
						else {
							j++;
						}
					}
					nan_arg[j] = '\0';
						
					if (sig_negative) {
						result = -nan(nan_arg);
					}
					else {
						result = nan(nan_arg);
					}

					*chars_scanned = spaces + i + j + sign_detected;
					return result;
				}
				else {
					scan_state = 4096;
				}
				break;
			}

			case 2:
				if (c == radix_marker) {
					scan_state = 16;
					c = fetch();
					break;
				}
				if (!isdigit(c)) {
					scan_state = 4096;
					break;
				}

				if (c == '0') {
					c = fetch();
					if (toupper(c) == 'X') {
						scan_state = 32768;
						hex_scan_state = 1;
					}
					else {
						scan_state = 4;
					}
					break;
				}

				scan_state = 8;
				break;
			
			case 4:
				if (c == '0') {
					c = fetch();
					
					break;
				}
				scan_state = 8;
				break;
			
			case 8:
				if (!isdigit(c)) {
					if (c == radix_marker) {
						scan_state = 32;
						c = fetch();
					}
					else {
						scan_state = 64;
					}
					break;
				}
				if (d.sig.length < 20) {
					d.sig.text[d.sig.length++] = (unsigned char)c;
				}
				else {
					exp_adjust++;
				}

				c = fetch();
				break;
			
			case 16:
				if (!isdigit(c)) {
					scan_state = 4096;
					break;
				}

				scan_state = 32;
				break;
			
			case 32:
				if (!isdigit(c)) {
					scan_state = 64;
					break;
				}

				if (d.sig.length < 20) {
					if ( c != '0' || d.sig.length) {
						d.sig.text[d.sig.length++] = (unsigned char)c;
					}

					exp_adjust--;
				}
				c = fetch();
				break;
			
			case 64:
				if (toupper(c) == 'E') {
					scan_state = 128;
					c = fetch();
					break;
				}
				scan_state = 2048;
				break;
			
			case 128:
				if (c == '+') {
					c = fetch();
				}
				else if (c == '-') {
					c = fetch();
					exp_negative = 1;
				}

				scan_state = 256;
				break;
					
			case 256:
				if (!isdigit(c)) {
					scan_state = 4096;
					break;
				}

				if (c == '0') {
					scan_state = 512;
					c = fetch();
					break;
				}

				scan_state = 1024;
				break;
			
			case 512:
				if (c == '0') {
					c = fetch();
					break;
				}

				scan_state = 1024;
				break;
			
			case 1024:
				if (!isdigit(c)) {
					scan_state = 2048;
					break;
				}

				exp_value = exp_value*10 + (c - '0');
				if (exp_value > SHRT_MAX) {
					*overflow = 1;
				}

				c = fetch();
				break;

		
			case 32768: {
				switch(hex_scan_state) {
					case 1:
						memset(mantissa, 0, sizeof(mantissa));
						chptr			= mantissa;
						mantissa_digits = (53 + 3) / 4;
						intdigits		= 0;
						NibbleIndex		= 0;
						hex_scan_state = 2;
						c = fetch();
						break;
					
					case 2:
						if (c == '0') {
							c = fetch();
							break;
						}

						hex_scan_state = 4;						
						break;
					
					case 4: 
						if (!isxdigit(c)) {
							if (c == radix_marker) {
								hex_scan_state = 8;
								c = fetch();
							}

							else {
								hex_scan_state = 16;
							}
							break;
						}


						if (intdigits < mantissa_digits) {
							intdigits++;
							uch = *(chptr + NibbleIndex / 2);

							ui = toupper(c);
							if (ui >= 'A') {
								ui = ui - 'A' + 10;
							}
							else {
								ui -= '0';
							}

							uch1 = (unsigned char)ui;

							if ((NibbleIndex % 2) != 0) {
								uch |= uch1;
							}
							else {
								uch |= (unsigned char)(uch1 << 4);
							}

							*(chptr + NibbleIndex++ / 2) = uch;
							c = fetch();
						}

						else {
							c = fetch();
						}

						break;
					
					case 8:
						if (!isxdigit(c)) {
							hex_scan_state = 16;
							break;
						}

						if (intdigits < mantissa_digits) {
							uch = *(chptr + NibbleIndex / 2);
							ui = toupper(c);

							if (ui >= 'A') {
								ui = ui - 'A' + 10;
							}
							else {
								ui -= '0';
							}

							uch1 = (unsigned char)ui;

							if ((NibbleIndex % 2) != 0) {
								uch |= uch1;
							}
							else {
								uch |= (unsigned char)(uch1 << 4);
							}

							*(chptr + NibbleIndex++ / 2) = uch;
							c = fetch();
						}
						else {
							c = fetch();
						}
						break;

					case 16:
						if (toupper(c) == 'P') {
							hex_scan_state = 32;
							exp_digits++;
							c = fetch();
						}
						else {
							scan_state = 2048;
						}

						break;
							
					case 32:		
						exp_digits++;
						if (c == '-') {
							expsign = 1;
						}
						else if (c != '+') {
							c = unfetch(c);
							count--;
							exp_digits--;
						}

						hex_scan_state = 64;
						c = fetch();
						break;
							
					case 64:
						if (!isdigit(c)) {
							scan_state = 4096;
							break;
						}

						if (c == '0') {
							exp_digits++;
							hex_scan_state = 128;
							c = fetch();
							break;
						}

						hex_scan_state = 256;
						break;

					case 128:
						if (c == '0') {
							c = fetch();
							break;
						}

						hex_scan_state = 256;
						break;
											
					case 256:
						if (!isdigit(c)) {
							scan_state = 2048;
							break;
						}

						exponent = exponent * 10 + (c - '0');

						if (exp_value > SHRT_MAX) {
							*overflow = 1;
						}

						exp_digits++;
						c = fetch();
						
						break;
						
					}
			}
			break;
		}
	}
	
	if (scan_state != 32768 ? !success(scan_state) : !hex_success(count, hex_scan_state)) {
		count = 0;
		*chars_scanned = 0;
	}
	else {
		count--;
		*chars_scanned = count + spaces;
	}
	
	unfetch(c);

	if (hex_scan_state == 0) {
		if (exp_negative) {
			exp_value = -exp_value;
		}

		{
			int n = d.sig.length;
			unsigned char* p = &d.sig.text[n];
			
			while (n-- && *--p == '0') {
				exp_adjust++;
			}
			
			d.sig.length = (unsigned char)(n + 1);
			
			if (d.sig.length == 0) {
				d.sig.text[d.sig.length++] = '0';
			}
		}
		
		exp_value += exp_adjust;
		
		if (exp_value < SHRT_MIN || exp_value > SHRT_MAX) {
			*overflow = 1;
		}
		
		if (*overflow) {
			if (exp_negative) {
				return 0.0;
			}
			else {
				return sig_negative ? -HUGE_VAL : HUGE_VAL;
			}
		}
		
		d.exponent = (short) exp_value;
		
		result = __dec2num(&d);
		
		if (result != 0.0 && result < LDBL_MIN) {
			*overflow = 1;
		}
		else if (result > LDBL_MAX) {
			*overflow = 1;
			result	  = HUGE_VAL;
		} 
		
		if (sig_negative && success(scan_state)) {
			result = -result;
		}
		
		return result;
	}
	else {
		unsigned mantissa_bit, dbl_bit;
		unsigned one_bit;
		long double dbl_bits_storage;
		unsigned char *dbl_bits = (unsigned char *) &dbl_bits_storage;

		if (expsign) {
			exponent = -exponent;
		}

		exponent += intdigits*4;
		
		one_bit = 0;
		while (one_bit < 4 && !(mantissa[0] & (0x80 >> one_bit))) {
			one_bit++;
			exponent--;
		}

		exponent--;

		if (1) {
			one_bit++;
		}

		if (one_bit) {
			unsigned char carry = 0;
			for (chptr = mantissa+sizeof(mantissa)-1; chptr >= mantissa; chptr--) {
				unsigned char a = *chptr;
				*chptr = (unsigned char)((a << one_bit) | carry);
				carry = (unsigned char)(a >> (8 - one_bit));
			}
		}
		
		memset(dbl_bits, 0, sizeof(dbl_bits_storage));
		dbl_bit = (64 - 52);
		
		for (mantissa_bit = 0; mantissa_bit < 52; mantissa_bit += 8) {
			unsigned char ui = mantissa[mantissa_bit >> 3];
			int halfbits;

			if (mantissa_bit + 8 > 52) {
				ui &= 0xff << (52 - mantissa_bit);
			}
			
			halfbits = (dbl_bit & 7);
			dbl_bits[dbl_bit>>3] |= (unsigned char)(ui >> halfbits);
			dbl_bit += 8;
			dbl_bits[dbl_bit>>3] |= (unsigned char)(ui << (8 - halfbits));
		}

		exponent += 1024-1;

		if ((exponent & ~(1024 * 2 - 1))) {
			*overflow = 1;
			return 0.0;
		}

		exponent <<= 32 - 11;
		
		dbl_bits[0] |= exponent >> 25;

		if (11 > 7) {
			dbl_bits[1] |= exponent >> 17;
		}

		if (11 > 15) {
			dbl_bits[2] |= exponent >> 9;
		}

		if (11 > 23) {
			dbl_bits[3] |= exponent >> 1;
		}
	
		if (sig_negative) {
			dbl_bits[0] |= 0x80;
		}

		result = *(long double *)dbl_bits;
		
		return result;
	}
}
