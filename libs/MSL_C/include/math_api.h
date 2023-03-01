#ifndef MATH_API_H
#define MATH_API_H

int __fpclassifyf(float);
int __signbitd(double);
int __fpclassifyd(double);

double copysign(double, double);
double frexp(double, int *);
double ldexp(double, int);

double nan(const char *);

#endif // MATH_API_H