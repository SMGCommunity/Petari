#include "ansi_fp.h"
#include "math_api.h"
#include "types.h"
#include <cmath>

extern double pow(double, double);

/* builtin for __num2dec_internal*/
inline int count_trailing(double x) {
    return __builtin___count_trailing_zero64(*(unsigned long long *)&x | 0x0010000000000000);
}

static int __must_round(const decimal *pDecimal, int position) {
    unsigned char const* i = pDecimal->sig.text + position;

    if (*i > 5) {
        return 1;
    }

    if (*i < 5) {
        return -1;
    }

    {
        unsigned char const* e = pDecimal->sig.text + pDecimal->sig.length;

        for (++i; i < e; ++i) {
            if (*i != 0) {
                return 1;
            }
        }
    }

    if (pDecimal->sig.text[position - 1] & 1) {
        return 1;
    }

    return -1;
}

/* inlined in some spots */
static void __dorounddecup(decimal *pDecimal, int digits) {
    unsigned char *b = pDecimal->sig.text;
    unsigned char *i = b + digits - 1;

    while (1) {
        if (*i < 9) {
            (*i)++;
            break;
        }

        if (i == b) {
            *i = 1;
            pDecimal->exponent++;
            break;
        }
        
        *i-- = 0;
    }
}

static void __rounddec(decimal *pDecimal, int dig) {
    if (dig <= 0 || dig >= pDecimal->sig.length) {
        return;
    }

    {
        int r = __must_round(pDecimal, dig);
        pDecimal->sig.length = dig;

        if (r < 0) {
            return;
        }
    }

    __dorounddecup(pDecimal, dig);
}

void __ull2dec(decimal *pDecimal, unsigned long long input) {
    pDecimal->sign = 0;
    pDecimal->sig.length = 0;

    for (; input != 0; input /= 10) {
        pDecimal->sig.text[pDecimal->sig.length++] = (unsigned char)(input % 10);
    }

    /* regswap if these were declared at the top...only logic I can think of is making them valid in this block */
    {
        unsigned char* start = pDecimal->sig.text;
        unsigned char* end = pDecimal->sig.text + pDecimal->sig.length;

        for (; start < --end; ++start) {
            unsigned char t = *start;
            *start = *end;
            *end = t;
        }
    }

    pDecimal->exponent = (short)(pDecimal->sig.length - 1);
}

void __timesdec(decimal *pResult, const decimal *x, const decimal *y) {
    unsigned long accumulator = 0;
    unsigned char mant[72];
    int i = x->sig.length + y->sig.length - 1;
    unsigned char *ip = mant + i + 1;
    unsigned char *ep = ip;
    pResult->sign = 0;

    for (; i > 0; --i) {
        int k = y->sig.length - 1;
        int j = i - k - 1;
        int l;
        int t;
        unsigned char const *jp, *kp;
        
        if (j < 0) {
            j = 0;
            k = i - 1;
        }
        
        jp = x->sig.text + j;
        kp = y->sig.text + k;
        l = k + 1;
        t = x->sig.length - j;
        
        if (l > t) {
            l = t;
        }

        for (; l > 0; --l, ++jp, --kp) {
            accumulator += (unsigned long)(*jp) * *kp;
        }
        
        *--ip = (unsigned char)(accumulator % 10);
        accumulator /= 10;
    }

    pResult->exponent = (short)(x->exponent + y->exponent);

    if (accumulator > 0) {
        *--ip = (unsigned char)accumulator;
        pResult->exponent++;
    }
    
    for (i = 0; i < 36 && ip < ep; ++i, ++ip) {
        pResult->sig.text[i] = *ip;
    }

    pResult->sig.length = i;

    if (ip < ep && *ip >= 5) {
        if (*ip == 5) {
            unsigned char* jp = ip + 1;
            for (; jp < ep; ++jp) {
                if (*jp != 0) {
                    goto round;
                }
            }
        
            if ((*(ip - 1) & 1) == 0) {
                return;
            }
        }
        round:
        /* inlined */
        __dorounddecup(pResult, pResult->sig.length);
    }
}

void __str2dec(decimal *pDecimal, const char *pStr, short exp) {
    int i;
    pDecimal->exponent = exp;
    pDecimal->sign = 0;

    for (i = 0; i < 36 && *pStr != 0;) {
        pDecimal->sig.text[i++] = (unsigned char)(*pStr++ - '0');
    }

    pDecimal->sig.length = i;

    if (*pStr != 0) {
        if (*pStr < 5) {
            return;
        }

        if (*pStr > 5) {
            goto round;
        }

        {
            const char* p = pStr + 1;
            for (; *p != 0; ++p) {
                if (*p != '0') {
                    goto round;
                }
            }

            if ((pDecimal->sig.text[i - 1] & 1) == 0) {
                return;
            }
        }

        round:
            __dorounddecup(pDecimal, pDecimal->sig.length);
    }
}

static void __two_exp(decimal *pDecimal, long exp) {
    switch (exp) {
        case -64:
            __str2dec(pDecimal, "542101086242752217003726400434970855712890625", -20);
            return;
        case -53:
            __str2dec(pDecimal, "11102230246251565404236316680908203125", -16);
            return;
        case -32:
            __str2dec(pDecimal, "23283064365386962890625", -10);
            return;
        case -16:
            __str2dec(pDecimal, "152587890625", -5);
            return;
        case -8:
            __str2dec(pDecimal, "390625", -3);
            return;
        case -7:
            __str2dec(pDecimal, "78125", -3);
            return;
        case -6:
            __str2dec(pDecimal, "15625", -2);
            return;
        case -5:
            __str2dec(pDecimal, "3125", -2);
            return;
        case -4:
            __str2dec(pDecimal, "625", -2);
            return;
        case -3:
            __str2dec(pDecimal, "125", -1);
            return;
        case -2:
            __str2dec(pDecimal, "25", -1);
            return;
        case -1:
            __str2dec(pDecimal, "5", -1);
            return;
        case 0:
            __str2dec(pDecimal, "1", 0);
            return;
        case 1:
            __str2dec(pDecimal, "2", 0);
            return;
        case 2:
            __str2dec(pDecimal, "4", 0);
            return;
        case 3:
            __str2dec(pDecimal, "8", 0);
            return;
        case 4:
            __str2dec(pDecimal, "16", 1);
            return;
        case 5:
            __str2dec(pDecimal, "32", 1);
            return;
        case 6:
            __str2dec(pDecimal, "64", 1);
            return;
        case 7:
            __str2dec(pDecimal, "128", 2);
            return;
        case 8:
            __str2dec(pDecimal, "256", 2);
            return;
    }

    {
        decimal dec, temp;
        __two_exp(&dec, ((long)((0x80000000UL & exp) >> 31) + exp) >> 1);
        __timesdec(pDecimal, &dec, &dec);

        if (exp & 0x1) {
            temp = *pDecimal;

            if (exp > 0) {
                __str2dec(&dec, "2", 0);
            }
            else {
                __str2dec(&dec, "5", -1);
            }

            __timesdec(pDecimal, &temp, &dec);
        }
    }
}

int __equals_dec(const decimal *x, const decimal *y) {
    if (x->sig.text[0] == 0) {
        if (y->sig.text[0] == 0) {
            return 1;
        }
        
        return 0;
    }

    if (y->sig.text[0] == 0) {
        if (x->sig.text[0] == 0) {
            return 1;
        }

        return 0;
    }

    if (x->exponent == y->exponent) {
        int i;
        int l = x->sig.length;
        
        if (l > y->sig.length) {
            l = y->sig.length;
        }

        for (i = 0; i < l; ++i) {
            if (x->sig.text[i] != y->sig.text[i]) {
                return 0;
            }
        }

        if (l == x->sig.length) {
            x = y;
        }

        for (; i < x->sig.length; ++i) {
            if (x->sig.text[i] != 0) {
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

static int __less_dec(const decimal *x, const decimal *y) {
    if (x->sig.text[0] == 0) {
        if (y->sig.text[0] != 0) {
            return 1;
        }

        return 0;
    }

    if (y->sig.text[0] == 0) {
        return 0;
    }

    if (x->exponent == y->exponent) {
        int i;
        int l = x->sig.length;

        if (l > y->sig.length) {
            l = y->sig.length;
        }

        for (i = 0; i < l; ++i) {
            if (x->sig.text[i] < y->sig.text[i]) {
                return 1;
            }

            if (y->sig.text[i] < x->sig.text[i]) {
                return 0;
            }
        }

        if (l == x->sig.length) {
            for (; i < y->sig.length; ++i) {
                if (y->sig.text[i] != 0) {
                    return 1;
                }
            }
        }

        return 0;
    }

    return x->exponent < y->exponent;
}

void __minus_dec(decimal *z, const decimal *x, const decimal *y) {
	int zlen, dexp;
	unsigned char *ib, *i, *ie;
	unsigned char const *jb, *j, *jn;
	*z = *x;

	if (y->sig.text[0] == 0) {
		return;
    }

	zlen = z->sig.length;

	if (zlen < y->sig.length) {
		zlen = y->sig.length;
    }

	dexp = z->exponent - y->exponent;
	zlen += dexp;

	if (zlen > 36) {
		zlen = 36;
    }
 
	while (z->sig.length < zlen) {
		z->sig.text[z->sig.length++] = 0;
    }

	ib = z->sig.text;
	i = ib + zlen;

	if (y->sig.length + dexp < zlen) {
		i = ib + (y->sig.length + dexp);
    }

	jb = y->sig.text;
	j = jb + (i - ib - dexp);
	jn = j;

	while (i > ib && j > jb) {
		--i;
		--j;
		if (*i < *j) {
			unsigned char *k = i - 1;
			while (*k == 0) {
				--k;
            }
			while (k != i) {
				--*k;
				*++k += 10;
			}
		}

		*i -= *j;
	}

	if (jn - jb < y->sig.length) {
		BOOL round_down = FALSE; 
		if (*jn < 5) {
			round_down = TRUE;
        }
		else if (*jn == 5) {
			unsigned char const *je = y->sig.text + y->sig.length;
    
			for (j = jn + 1; j < je; ++j) {
				if (*j != 0) {
					goto done;
                }
            }

			i = ib + (jn - jb) + dexp - 1;

			if (*i & 1) {
				round_down = 1;
            }
		}
		if (round_down) {
			if (*i < 1) {
				unsigned char *k = i - 1;

				while (*k == 0) {
					--k;
                }
				while (k != i) {
					--*k;
					*++k += 10;
				}
			}

			*i -= 1;
		}
	}

done:
	for (i = ib; *i == 0; ++i);

	if (i > ib) {
		unsigned char dl = (unsigned char)(i - ib);
		z->exponent -= dl;
		ie = ib + z->sig.length;
		for (; i < ie; ++i, ++ib)
			*ib = *i;
		z->sig.length -= dl;
	}

	ib = z->sig.text;

	for (i = ib + z->sig.length; i > ib;) {
		--i;
		if (*i != 0) {
			break;
        }
	}

	z->sig.length = (unsigned char)(i - ib + 1);
}

void __num2dec_internal(decimal *pDecimal, double x) {
	char sign = (char)(__signbitd(x) != 0);
	if (x == 0.0) {
		pDecimal->sign = sign;
		pDecimal->exponent = 0;
		pDecimal->sig.length = 1;
		pDecimal->sig.text[0] = 0;
		return;
	}

	if (!(__fpclassifyd(x) > 2)) {
		pDecimal->sign = sign;
		pDecimal->exponent = 0;
		pDecimal->sig.length = 1;
		pDecimal->sig.text[0] = (unsigned char)(__fpclassifyd(x) == 1 ? 'N' : 'I');
		return;
	}

	if (sign) {
		x = -x;
    }

	{
        int exp;
        double frac = frexp(x, &exp);
        long num_bits_extract = 53 - count_trailing(frac);
        decimal int_d, pow2_d;
        __two_exp(&pow2_d, exp-num_bits_extract);
        __ull2dec(&int_d, (unsigned long long)ldexp(frac, (int)num_bits_extract));
        __timesdec(pDecimal, &int_d, &pow2_d);
        pDecimal->sign = sign;
	}
}

void __num2dec(const decform *pForm, double x, decimal *pDecimal) {
    short digits = pForm->digits;
    int i;

    __num2dec_internal(pDecimal, x);

    if (pDecimal->sig.text[0] > 9) {
        return;
    }

    if (digits > 36) {
        digits = 36;
    }

    __rounddec(pDecimal, digits);

    while (pDecimal->sig.length < digits) {
        pDecimal->sig.text[pDecimal->sig.length++] = 0;
    }

    pDecimal->exponent -= pDecimal->sig.length - 1;

    for (i = 0; i < pDecimal->sig.length; ++i) {
        pDecimal->sig.text[i] += '0';
    }
}

double __dec2num(const decimal *d)
{
	if (d->sig.length <= 0) {
		return copysign(0.0, d->sign == 0 ? 1.0 : -1.0);
    }

	switch (d->sig.text[0]) {
	case '0':
		return copysign(0.0, d->sign == 0 ? 1.0 : -1.0);
	case 'I':
		return copysign((double)INFINITY, d->sign == 0 ? 1.0 : -1.0);
	case 'N':
		{
            double result;
            long long *ll = (long long*)&result;
            *ll = 0x7FF0000000000000ULL;
            if (d->sign)
                *ll |= 0x8000000000000000ULL;
            *ll |= 0x0008000000000000ULL;
        
            return result;
		}
	}
	{
        static double pow_10[8] = {1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8};
        decimal dec = *d;
        unsigned char *i = dec.sig.text;
        unsigned char *e = i + dec.sig.length;
        double first_guess;
        int exponent;

        for (; i < e; ++i) {
            *i -= '0';
        }

        dec.exponent += dec.sig.length - 1;
        exponent = dec.exponent;
        {
            decimal max;
            __str2dec(&max, "179769313486231580793728714053034151", 308);
            if (__less_dec(&max, &dec)) {
                return copysign((double)INFINITY, d->sign == 0 ? 1.0 : -1.0);
            }   
        }

        i = dec.sig.text;
        first_guess = *i++;

        while (i < e) {
            unsigned long ival = 0;
            int j;
            double temp1, temp2;
            int ndig = (int)(e - i) % 8;

            if (ndig == 0) {
                ndig = 8;
            }

            for (j = 0; j < ndig; ++j, ++i) {
                ival = ival * 10 + *i;
            }

            temp1 = first_guess * pow_10[ndig - 1];
            temp2 = temp1 + ival;
        
            if (ival != 0 && temp1 == temp2) {
                break;
            }

            first_guess = temp2;
            exponent -= ndig;
        }
        if (exponent < 0) {
            first_guess /= pow(5.0, -exponent);
        }
        else {
            first_guess *= pow(5.0, exponent);
        }

        first_guess = ldexp(first_guess, exponent);

        if (__fpclassifyd(first_guess) == 2) {
            first_guess = (double)0x7FEFFFFFFFFFFFFF;
        }

        {
            decimal feedback1, feedback2, difflow, diffhigh;
            double next_guess;
            long long *ull = (long long*)&next_guess;
            int guessed_low = 0;
            __num2dec_internal(&feedback1, first_guess);

            if (__equals_dec(&feedback1, &dec)) {
                goto done;
            }

            if (__less_dec(&feedback1, &dec)) {
                guessed_low = 1;
            }

            next_guess = first_guess;
            while (1) {
                if (guessed_low) {
                    ++*ull;
                    if (__fpclassifyd(next_guess) == 2) {
                        goto done;
                    }
                }
                else {
                    --*ull;
                }

                __num2dec_internal(&feedback2, next_guess);
                if (guessed_low && !__less_dec(&feedback2, &dec)) {
                    break;
                }
                else if (!guessed_low && !__less_dec(&dec, &feedback2)) {
                    difflow = feedback1;
                    feedback1 = feedback2;
                    feedback2 =  difflow;
                    {
                        double temp = first_guess;
                        first_guess = next_guess;
                        next_guess = temp;
                    }
                    break;
                }
                feedback1 = feedback2;
                first_guess = next_guess;
            }

            __minus_dec(&difflow, &dec, &feedback1);
            __minus_dec(&diffhigh, &feedback2, &dec);
            if (__equals_dec(&difflow, &diffhigh)) {
                if (*(long long*)&first_guess & 1) {
                    first_guess = next_guess;
                }
            }
            else if (!__less_dec(&difflow, &diffhigh)) {
                first_guess = next_guess;
            }
        }
        done:
            if (dec.sign) {
                first_guess = -first_guess;
            }
            return first_guess;
	}
}