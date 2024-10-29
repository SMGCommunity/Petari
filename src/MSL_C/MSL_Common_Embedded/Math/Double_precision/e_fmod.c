static const double one = 1.0;
static const double Zero[] = {0.0, -0.0};

double __ieee754_fmod(double x, double y) {
    int n, hx, hy, hz, ix, iy, sx, i;
    unsigned int lx, ly, lz;

    hx = *(int*)&x;
    lx = *(1 + (int*)&x);
    hy = *(int*)&y;
    ly = *(1 + (int*)&y);
    sx = hx & 0x80000000;
    hx ^= sx;
    hy &= 0x7FFFFFFF;

    if ((hy | ly) == 0 || (hx >= 0x7FF00000) || ((hy | ((ly | -ly) >> 31)) > 0x7FF00000)) {
        return (x * y) / (x * y);
    }

    if (hx <= hy) {
        if ((hx < hy) || (lx < ly)) {
            return x;
        }

        if (lx == ly) {
            return Zero[(unsigned int)sx >> 31];
        }
    }

    if (hx < 0x100000) {
        if (hx == 0) {
            for (ix = -1043, i = lx; i > 0; i <<= 1) {
                ix -= 1;
            }
        }
        else {
            for (ix = -1022, i = (hx << 11); i > 0; i <<= 1) {
                ix -= 1;
            }
        }
    }
    else {
        ix = (hx >> 20) - 1023;
    }

    if (hy < 0x00100000) {
        if (hy == 0) {
            for (iy = -1043, i = ly; i > 0; i <<= 1) {
                iy -=1;
            }
        }
        else {
            for (iy = -1022, i = (hy << 11); i > 0; i <<= 1) {
                iy -= 1;
            }
        }
    }
    else {
        iy = (hy >> 20) - 1023;
    }

    if (ix >= -1022) {
        hx = 0x100000 | (0xFFFFF & hx);
    }
    else {
        n = -1022 - ix;

        if (n <= 31) {
            hx = (hx << n) | (lx >> (32 - n));
            lx <<= n;
        }
        else {
            hx = lx << (n - 32);
            lx = 0;
        }
    }

    if (iy >= -1022) {
        hy = 0x100000 | (0xFFFFF & hy);
    }
    else {
        n = -1022 - iy;

        if (n <= 31) {
            hy = (hy << n) | (ly >> (32 - n));
            ly <<= n;
        }
        else {
            hy = ly << (n - 32);
            ly = 0;
        }
    }

    n = ix - iy;
	while(n--) {
	    hz= hx - hy;
        lz= lx - ly; 
        
        if(lx < ly) {
            hz -= 1;
        }
	    if(hz < 0){
            hx = hx + hx + (lx >> 31); 
            lx = lx + lx;
        }
	    else {
	    	if((hz | lz) == 0)
		    return Zero[(unsigned int) sx >> 31];
	    	hx = hz + hz + (lz >> 31); 
            lx = lz + lz;
	    }
	}

    hz= hx - hy;
    lz= lx - ly; 
    
    if(lx < ly) {
        hz -= 1;
    }
    
    if(hz >= 0) {
        hx = hz;
        lx = lz;
    }

    if((hx | lx) == 0)
        return Zero[(unsigned int)sx >> 31];

    while(hx < 0x100000) {
        hx = hx + hx + (lx >> 31); 
        lx = lx + lx;
        iy -= 1;
    }

    if(iy>= -1022) {
        hx = ((hx - 0x100000)|((iy + 1023) << 20));
        *(int*)&x = hx | sx;
        *(1 + (int*)&x) = lx;
    }
    else {
        n = -1022 - iy;

        if (n <= 20) {
            lx = (lx >> n) | ((unsigned int)hx << (32 - n));
            hx >>= n;
        }
        else if (n <= 31) {
            lx = (hx << (32 - n)) | (lx >> n); 
            hx = sx;
        }
        else {
            lx = hx >>(n - 32);
            hx = sx;
        }

        *(int*)&x = hx | sx;
        *(1 + (int*)&x) = lx;
        x *= one;
    }

    return x;
}
