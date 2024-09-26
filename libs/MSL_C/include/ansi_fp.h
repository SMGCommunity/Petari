#ifndef ANSI_FP
#define ANSI_FP

#pragma options align=native

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

void __num2dec(const decform *, double, decimal *);
double __dec2num(const decimal *);

#pragma options align=reset

#endif // ANSI_FP