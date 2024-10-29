#include <cmath>

double tan(double x) {
    double y[2], z = 0.0;
    int n, ix;

    ix = *(int*)&x;
    ix &= 0x7FFFFFFF;

    if (ix <= 0x3FE921FB) {
        return __kernel_tan(x, z, 1);
    }
    else if (ix >= 0x7FF00000) {
        return x - x;
    }
    else {
        n = __ieee754_rem_pio2(x, y);
        return __kernel_tan(y[0], y[1], 1 - ((n & 1) << 1));
    }
}