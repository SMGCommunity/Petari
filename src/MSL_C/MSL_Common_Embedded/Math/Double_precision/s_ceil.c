double ceil(double x) {
    int i0, i1, j0;
    unsigned int i, j;

    i0 = *(int*)&x;
    i1 = *(1 + (int*)&x);
    j0 = ((i0 >> 20) & 0x7FF) - 0x3FF;

    if (j0 < 20) {
        if (j0 < 0) {
            if (1.0e300 + x > 0.0) {
                if (i0 < 0) {
                    i0 = 0x80000000;
                    i1 = 0;
                }
                else if ((i0 | i1) != 0) {
                    i0 = 0x3FF00000;
                    i1 = 0;
                }
            }
        }
        else {
            i = (0xFFFFF) >> j0;

            if (((i0 & i) | i1) == 0) {
                return x;
            }

            if (1.0e300 + x > 0.0) {
                if (i0 > 0){
                    i += (0x100000) >> j0;
                }

                i0 &= ~i;
                i1 = 0;
            }
        }
    }
    else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x;
        }

        return x;
    }
    else {
        i = 0xFFFFFFFF >> (j0 - 20);
        if ((i1 & i) == 0) {
            return x;
        }

        if (1.0e300 + x > 0.0) {
            if (i0 > 0) {
                if (j0 == 20) {
                    i0 += 1;
                }
                else {
                    j = i1 + (1 << (52 - j0));

                    if (j < i1) {
                        i0 += 1;
                    }

                    i1 = j;
                }
            }

            i1 &= ~i;
        }
    }

    *(int *)&x = i0;
    *(1 + (int*)&x) = i1;
    return x;
}