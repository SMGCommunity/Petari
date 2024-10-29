extern int __kernel_rem_pio2(double *, double *, int, int, int, const int *);

static int two_over_pi[] = {
    0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
    0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A,
    0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129,
    0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C, 0x7026B4, 0x5F7E41,
    0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8,
    0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A, 0x6D1F6D, 0x367ECF,
    0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5,
    0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB, 0x5FB11F, 0x8D5D08,
    0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3,
    0x91615E, 0xE61B08, 0x659985, 0x5F14A0, 0x68408D, 0xFFD880,
    0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B,
};

static int npio2_hw[] = {
    0x3FF921FB, 0x400921FB, 0x4012D97C, 0x401921FB, 0x401F6A7A, 0x4022D97C,
    0x4025FDBB, 0x402921FB, 0x402C463A, 0x402F6A7A, 0x4031475C, 0x4032D97C,
    0x40346B9C, 0x4035FDBB, 0x40378FDB, 0x403921FB, 0x403AB41B, 0x403C463A,
    0x403DD85A, 0x403F6A7A, 0x40407E4C, 0x4041475C, 0x4042106C, 0x4042D97C,
    0x4043A28C, 0x40446B9C, 0x404534AC, 0x4045FDBB, 0x4046C6CB, 0x40478FDB,
    0x404858EB, 0x404921FB,
};


int __ieee754_rem_pio2(double x, double *y) {
    double z, w, t, r, fn;
    double tx[3];
    int e0, i, j, nx, n, ix, hx;

    hx = *(int*)&x;
    ix = hx & 0x7FFFFFFF;

    if (ix <= 0x3FE921FB) {
        y[0] = x;
        y[1] = 0;
        return 0;
    }

    if (ix < 0x4002D97C) {
        if (hx > 0) {
            z = x - 1.57079632673412561417e+00;

            if (ix != 0x3FF921FB) {
                y[0] = z - 6.07710050650619224932e-11;
                y[1] = (z - y[0]) - 6.07710050650619224932e-11;
            }
            else {
                z -= 6.07710050630396597660e-11;
                y[0] = z - 2.02226624879595063154e-21;
                y[1] = (z - y[0]) - 2.02226624879595063154e-21;
            }

            return 1;
        }
        else {
            z = x + 1.57079632673412561417e+00;

            if (ix != 0x3FF921FB) {
                y[0] = z + 6.07710050650619224932e-11;
                y[1] = (z - y[0]) + 6.07710050650619224932e-11;
            }
            else {
                z += 6.07710050630396597660e-11;
                y[0] = z + 2.02226624879595063154e-21;
                y[1] = (z - y[0]) + 2.02226624879595063154e-21;
            }

            return -1;
        }
    }

    if (ix <= 0x413921FB) {
        t = __fabs(x);
        n = (int)(t * 6.36619772367581382433e-01 + 5.00000000000000000000e-01);
        fn = (double)n;
        r = t - fn * 1.57079632673412561417e+00;
        w = fn * 6.07710050650619224932e-11;

        if (n < 32 && ix != npio2_hw[n - 1]) {
            y[0] = r - w;
        }
        else {
            j = ix >> 20;
            y[0] = r - w;
            i = j - ((((*(int*)&y[0])) >> 20) & 0x7FF);

            if (i > 16) {
                t = r;
                w = fn * 6.07710050630396597660e-11;
                r = t - w;
                w = fn * 2.02226624879595063154e-21 - ((t - r) - w);
                y[0] = r - w;
                i = j - ((((*(int*)&y[0])) >> 20) & 0x7FF);

                if (i > 49) {
                    t = r;
                    w = fn * 2.02226624871116645580e-21;
                    r = t - w;
                    w = fn * 8.47842766036889956997e-32 - ((t - r) - w);
                    y[0] = r - w;
                }
            }
        }

        y[1] = (r - y[0]) - w;

        if (hx < 0) {
            y[0] = -y[0];
            y[1] = -y[1];
            return -n;
        }
        else {
            return n;
        }
    }

    if (ix >= 0x7FF00000) {
        y[0] = y[1] = x - x;
        return 0;
    }

    *(1 + (int*)&z) = *(1 + (int*)&x);
    e0 = (ix >> 20) - 1046;
    *(int*)&z = ix - (e0 << 20);

    for (i = 0; i < 2; i++) {
        tx[i] = (double)((int)(z));
        z = (z - tx[i]) * 1.67772160000000000000e+07;
    }

    tx[2] = z;
    nx = 3;

    while (tx[nx - 1] == 0.00000000000000000000e+00) {
        nx--;
    }

    n = __kernel_rem_pio2(tx, y, e0, nx, 2, two_over_pi);

    if (hx < 0) {
        y[0] = -y[0];
        y[1] = -y[1];
        return -n;
    }

    return n;
}