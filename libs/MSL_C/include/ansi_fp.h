#ifndef ANSI_FP
#define ANSI_FP

typedef struct decimal {
    char sign;
    char _1;
    short exponent;

    struct {
        unsigned char length;
        unsigned char text[36];
        unsigned char pad;
    } sig;
} decimal;

typedef struct decform {
    char style;
    char _1;
    short digits;
} decform;

#endif // ANSI_FP