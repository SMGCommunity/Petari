#include <cmath>

double scalbn(double x, int n) {
    double man;
    int exponent;

    man = frexp(x, &exponent);
    exponent += n;
    return ldexp(man, exponent);
}