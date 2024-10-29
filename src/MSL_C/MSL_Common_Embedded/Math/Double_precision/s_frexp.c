double frexp(double x, int *ptr) {
    int hx, ix, lx;
    hx = *(int*)&x;
    ix = 0x7FFFFFFF & hx;
    lx = *(1 + (int*)&x);
    *ptr = 0;

    if (ix >= 0x7FF00000 || ((ix | lx) == 0)) {
        return x;
    }

    if (ix < 0x100000) {
        x *= 1.80143985094819840000e+16;
        hx = *(int*)&x;
        ix = hx & 0x7FFFFFFF;
        *ptr = -54;
    }

    *ptr += (ix >> 20) - 1022;
    hx = (hx & 0x800FFFFF) | 0x3FE00000;
    *(int*)&x = hx;
    return x;
}