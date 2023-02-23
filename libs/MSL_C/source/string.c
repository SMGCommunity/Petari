#include <cstring>

#define K1 0x80808080 
#define K2 0xfefefeff

char* strcpy(char *dst, const char *src) {
    register unsigned char *destb, *fromb;
    register unsigned int w, t, align;
    register unsigned int k1;
    register unsigned int k2;
			
    fromb = (unsigned char *)src;
    destb = (unsigned char *)dst;
    
    if ((align = ((int)fromb & 3)) != ((int)destb & 3)) {
        goto bytecopy;
    }

    if (align) {
        if ((*destb = *fromb) == 0) {
            return dst;
        }

        for (align = 3 - align; align; align--) {
            if ((*(++destb) = *(++fromb)) == 0) {
                return dst;
            }
        }

        ++destb;
        ++fromb;
    }

    k1 = K1;
    k2 = K2;

	w = *((int*)(fromb));
    t = w + k2;
    t &= ~w;
    t &= k1;
    if (t) {
        goto bytecopy;
    }
    
    --((int*)(destb));
			
    do {
        *(++((int*)(destb))) = w;
        w = *(++((int*)(fromb)));
        t = w + k2;
        t &= ~w;
        t &= k1;

        if (t) {
            goto adjust;
        }
    } while(1);

    adjust:
        ++((int*)(destb));
    bytecopy:
        if ((*destb = *fromb) == 0) {
            return dst;
        }
        do {
            if ((*(++destb) = *(++fromb)) == 0) {
                return dst;
            }
                
        } while(1);

        return dst;
}

char* strncpy(char *dst, const char *src, size_t n) {
    const unsigned char* p = (const unsigned char*)src - 1;
    unsigned char* q = (unsigned char*)dst -1 ;
    n++;

    while (--n) {
        if (!(*++q = *++p)) {
            while (--n) {
                *++q = 0;
            }
            break;
        }
    }

    return dst;
}

char* strcat(char *dst, const char *src) {
    const unsigned char* p = (unsigned char*)src - 1;
    unsigned char* q = (unsigned char*)dst - 1;

    while (*++q);
    q--;
    while (*++q = *++p);

    return dst;
}

int strcmp(const char *str1, const char *str2) {
    register unsigned char *left = (unsigned char *)str1;
    register unsigned char *right = (unsigned char *)str2;
    int k1, k2, align, x, dist;
    unsigned int l1, r1;

    l1 = *left;
    r1 = *right;
    dist = l1 - r1;

    if (dist) {
        return dist;
    }

    if ((align = ((unsigned int)left & 3)) != ((unsigned int)right & 3)) {
        goto bytecopy;
    }
    if (align) {
        if (l1 == 0) {
            return (0);
        }
        for (align = 3 - align; align; align--) {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1) {
                return (l1 - r1);
            }
            if (l1 == 0) {
                return (0);
            }
        }
        left++;
        right++;
    }

    k1 = K1;
    k2 = K2;
    l1 = *(int*)left;
    r1 = *(int*)right;
    x = l1 + k2;
    x &= ~l1;
    if (x & k1) {
        goto adjust;
    }
    while(l1 == r1) {
        l1 = *(++((int*)(left)));
        r1 = *(++((int*)(right)));
        x = l1 + k2;
        if (x & k1) {
            goto adjust;
        }
    }

    adjust:
        l1 = *left;
        r1 = *right;
        dist = l1 - r1;
        if (dist) {
            return (dist);
        }
    bytecopy:
        if (l1 == 0) {
            return (0);
        }
        do {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1) {
                return (l1 - r1);
            }
            if (l1 == 0) {
                return (0);
            }
        } while (1);
}

int strncmp(const char *str1, const char *str2, size_t n) {
    const unsigned char * p1 = (unsigned char *) str1 - 1;
    const unsigned char * p2 = (unsigned char *) str2 - 1;
    unsigned long c1, c2;
    
    n++;
    
    while (--n) {
        if ((c1 = *++p1) != (c2 = *++p2)) {
            return(c1 - c2);
        }
        else if (!c1) {
            break;
        }
    }

    return 0;
}

char* strchr(const char *str, int chr) {
    const unsigned char* p = (unsigned char*) str - 1;
    unsigned long c = (chr & 0xff);
    unsigned long ch;

    while(ch = *++p) {
        if (ch == c) {
            return((char*)p);
        }
    }

    return(c ? 0 : (char*)p);
}

char* strrchr(const char *str, int chr) {
    const unsigned char* p = (unsigned char*) str - 1;
    const unsigned char* q = 0;
    unsigned long c = (chr & 0xff);
    unsigned long ch;
    
    while(ch = *++p) {
        if (ch == c)
            q = p;
    }
    
    if (q) {
        return((char*)q);
    }
    
    return(c ? 0 : (char*)p);
}

char* strstr(const char *str, const char *pat) {
    unsigned char* s1 = (unsigned char*)str - 1;
    unsigned char* p1 = (unsigned char*)pat - 1;
    unsigned long firstc, c1, c2;

    if ((pat == 0) || (!(firstc = *++p1))) {
        return ((char*)str);
    }

    while (c1 = *++s1) {
        if (c1 == firstc) {
            const unsigned char* s2 = s1 - 1;
            const unsigned char* p2 = p1 - 1;

            while ((c1 = *++s2) == (c2 = *++p2) && c1) {

            }

            if (!c2) {
                return ((char*)s1);
            }
        }
    }

    return 0;
}