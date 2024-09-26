double __kernel_cos(double x, double y) {
    double a, hz, z, r, qx;
    int ix = (*(int*)&x) & 0x7FFFFFFF;

    if (ix < 0x3E400000) {
        if ((int)x == 0) {
            return 1.0;
        }
    }

    z = x * x;
    r = z * (4.16666666666666019037e-02 + z * (-1.38888888888741095749e-03 + z * (2.48015872894767294178e-05 + z * (-2.75573143513906633035e-07 + z * (2.08757232129817482790e-09 + z * -1.13596475577881948265e-11)))));
    
    if (ix < 0x3FD33333) {
        return 1.0 - (0.5 * z - (z * r - x * y));
    }
    else {
        if (ix > 0x3FE90000) {
            qx = 0.28125;
        }
        else {
            *(int*)&qx = ix - 0x200000;
            *(1 + (int*)&qx) = 0;
        }

        hz = 0.5 * z - qx;
        a = 1.0 - qx;
        return a - (hz - (z * r - x * y));
    }
}