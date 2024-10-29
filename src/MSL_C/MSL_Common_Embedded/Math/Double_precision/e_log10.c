#include <cmath>
#include <cerrno>

static double zero = 0.0;

double __ieee754_log10(double x) {
    double y, z;
    int i, k, hx;
    unsigned int lx;

    hx = *(int*)&x;
    lx = *(1 + (int*)&x);
    k = 0;

    if (hx < 0x100000) {
        if (((hx & 0x7FFFFFFF) | lx) == 0) {
            errno = 0x21;
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
    i = ((unsigned int)k & 0x80000000) >> 31;
    hx = (hx & 0xFFFFF) | ((0x3FF - i) << 20);
    y = (double)(k + i);
    *(int*)&x = hx;
    z = y * 3.69423907715893078616e-13 + 4.34294481903251816668e-01 * __ieee754_log(x);
    return z + y * 3.01029995663611771306e-01;
}