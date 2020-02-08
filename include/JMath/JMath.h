#ifndef JMATH_H
#define JMATH_H

#include <revolution/mtx.h>

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);

class JMathInlineVEC
{
public:
    static void PSVECAdd(const Vec *, const Vec *, Vec *);
};

#endif // JMATH_H