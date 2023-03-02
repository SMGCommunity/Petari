#include <cmath>

double sin(double x) {
    double y[2], z = 0.0;
    int n, ix;

    ix = *(int*)&x;
    ix &= 0x7FFFFFFF;

    if (ix <= 0x3FE921FB) {
        return __kernel_sin(x, z, 0);
    }
    else if (ix >= 0x7FF00000) {
        return x - x;
    }
    else {
        n = __ieee754_rem_pio2(x, y);

        switch (n & 3) {
            case 0:
                return __kernel_sin(y[0], y[1], 1);
            case 1:
                return __kernel_cos(y[0], y[1]);
            case 2:
                return -__kernel_sin(y[0], y[1], 1);
            default:
                return -__kernel_cos(y[0], y[1]);
        }
    }
}