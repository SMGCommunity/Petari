#include "math_api.h"

double ldexp(double x, int n) {
    int exp, hx, lx;

    if (!(__fpclassifyd(x) > 2) || x == 0.0) {
        return x;
    }

    hx = *(int*)&x;
    lx = *(1 + (int*)&x);
    exp = (hx & 0x7FF00000) >> 20;

    if (exp == 0) {
        if ((lx | (hx & 0x7FFFFFFF)) == 0) {
            return x;
        }

        x *= 1.80143985094819840000e+16;
        hx = *(int*)&x;
        exp = ((hx & 0x7FF00000) >> 20) - 54;

        if (n < -50000) {
            return 1.0e-300 * x;
        }
    }

    if (exp == 0x7FF) {
        return x + x;
    }

    exp = exp + n;
    
    if (exp > 0x7FE) {
        return 1.0e+300 * copysign(1.0e+300, x);
    }

    if (exp > 0) {
        *(int*)&x = (hx & 0x800FFFFF)| (exp << 20);
        return x;
    }

    if (exp <= -54) {
        if (n > 50000) {
            return 1.0e+300 * copysign(1.0e+300, x);
        }
        else {
            return 1.0e-300 * copysign(1.0e-300, x);
        }
    }

    exp += 54;
    *(int*)&x = (hx & 0x800FFFFF) | (exp << 20);
    return x * 5.55111512312578270212e-17;
}