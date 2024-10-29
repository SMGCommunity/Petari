#include <cerrno>

static const double
ln2_hi  =  6.93147180369123816490e-01,
ln2_lo  =  1.90821492927058770002e-10,
two54   =  1.80143985094819840000e+16,
Lg1 = 6.666666666666735130e-01,
Lg2 = 3.999999999940941908e-01,
Lg3 = 2.857142874366239149e-01,
Lg4 = 2.222219843214978396e-01,
Lg5 = 1.818357216161805012e-01,
Lg6 = 1.531383769920937332e-01,
Lg7 = 1.479819860511658591e-01;

static double zero = 0.0;

double __ieee754_log(double x) {
    double hfsq, f, s, z, R, w, t1, t2, dk;
    int k, hx, i, j;
    unsigned int lx;

    hx = *(int*)&x;
    lx = *(1 + (int*)&x);
    k = 0;

    if (hx < 0x00100000) {
        if (((hx&0x7fffffff)|lx)==0) {
            return -1.80143985094819840000e+16 / zero;
        }

        if (hx < 0) {
            errno = 0x21;
            return (x - x) / zero;
        }

        k -= 54;
        x *= 1.80143985094819840000e+16;
        hx = *(int*)&x;
    }

    if (hx >= 0x7FF00000) {
        return x + x;
    }

    k += (hx >> 20) - 1023;
    hx &= 0xFFFFF;
    i = (hx + 0x95f64) & 0x100000;
    *(int*)&x = hx | (i ^ 0x3FF00000);
    k += (i >> 20);
    f = x - 1.0;

    if((0xFFFFF & (2 + hx))< 3) {
        if(f == 0.0) {
            if(k == 0) {
                return 0.0;
            }
            else {
                dk = (double)k;
                return dk * 6.93147180369123816490e-01 + dk * 1.90821492927058770002e-10;
            }
        }

        R = f * f * (0.5 - 0.33333333333333333 * f);

        if (k == 0) {
            return f - R;
        }
        else {
            dk = (double)k;
            return dk * 6.93147180369123816490e-01 - ((R - dk * 1.90821492927058770002e-10) - f);
        }
    }

    s = f / (2.0 + f); 
    dk = (double)k;
    z = s * s;
    i = hx - 0x6147a;
    w = z * z;
    j = 0x6b851 - hx;
    t1= w * (Lg2 + w * (Lg4 + w * Lg6)); 
    t2= z *(Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7))); 
    i |= j;
    R = t2 + t1;

    if(i > 0) {
	    hfsq = 0.5 * f * f;
	    if(k == 0) {
            return f -(hfsq - s * (hfsq + R));
         } 
         else {
		     return dk * ln2_hi - ((hfsq-(s * (hfsq + R) + dk * ln2_lo)) - f);
         }
	} else {
        if(k == 0) {
            return f-s*(f-R);
            } 
        else {
            return dk * ln2_hi-((s * (f - R) - dk * ln2_lo) - f);
        }
	}
}