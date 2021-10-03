#pragma once

#include <revolution.h>

f32 JMASinRadian(f32);
f32 JMACosRadian(f32);

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);

class JMathInlineVEC {
public:
    static void PSVECSubtract(const Vec *, const Vec *, Vec *);
};