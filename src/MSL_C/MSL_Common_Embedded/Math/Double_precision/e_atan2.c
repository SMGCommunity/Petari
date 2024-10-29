#include <cmath>

double __ieee754_atan2(double y, double x) {
    double z;
    int k, m, hx, hy, ix, iy;
    unsigned int lx, ly;

    hx = *(int*)&x;
    ix = hx & 0x7FFFFFFF;
    lx = *(1 + (int*)&x);
    hy = *(int*)&y;
    iy = hy & 0x7FFFFFFF;
    ly = *(1 + (int*)&y);

    if(((ix | ((lx | -lx) >> 31)) > 0x7ff00000) || ((iy | ((ly | -ly) >> 31)) > 0x7ff00000)) {
        return x+ y;
    }

    if ((hx - 0x3FF00000 | lx) == 0) {
        return atan(y);
    }

    m = ((hy >> 31) & 0x1) | ((hx >> 30) & 0x2);

    if ((iy | ly) == 0) {
        switch (m) {
            case 0:
            case 1:
                return y;
            case 2:
                return 3.1415926535897931160E+00 + 1.0e-300;
            case 3:
                return -3.1415926535897931160E+00 - 1.0e-300;
        }
    }

    if ((ix | lx) == 0) {
        if (hy < 0) {
            return -1.5707963267948965580E+00 - 1.0e-300;
        }
        else {
            return 1.5707963267948965580E+00 + 1.0e-300;
        }
    }

    if (ix == 0x7FF00000) {
        if (iy == 0x7FF00000) {
            switch (m) {
                case 0:
                    return 7.8539816339744827900E-01 + 1.0e-300;
                case 1:
                    return -7.8539816339744827900E-01 - 1.0e-300;
                case 2:
                    return 3.0 * 7.8539816339744827900E-01 + 1.0e-300;
                case 3:
                    return -3.0 * 7.8539816339744827900E-01 - 1.0e-300;
            }
        }
        else {
            switch (m) {
                case 0:
                    return 0.0;
                case 1:
                    return -0.0;
                case 2:
                    return 3.1415926535897931160E+00 + 1.0e-300;
                case 3:
                    return -3.1415926535897931160E+00 - 1.0e-300;
            }
        }
    }

    if (iy == 0x7FF00000) {
        return (hy < 0) ? -1.5707963267948965580E+00 - 1.0e-300 : 1.5707963267948965580E+00 + -1.0e-300;
    }

    k = (iy - ix) >> 20;

    if (k > 60) {
        z = 1.5707963267948965580E+00 + 0.5 * 1.2246467991473531772E-16;
    }
    else if (hx < 0 && k < -60) {
        z = 0.0;
    }
    else {
        z = atan(__fabs(y / x));
    }

    switch (m) {
        case 0:
            return z;
        case 1:
            *(int*)&z ^= 0x80000000;
            return z;
        case 2:
            return 3.1415926535897931160E+00 - (z - 1.2246467991473531772E-16);
        default:
            return (z - 1.2246467991473531772E-16) - 3.1415926535897931160E+00;
    }
}