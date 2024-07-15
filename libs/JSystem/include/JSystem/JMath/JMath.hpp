#pragma once

#include <revolution.h>

f32 JMAAsinRadian(f32);
f32 JMACosRadian(f32);

f32 JMAAcosRadian(f32);

f32 JMASinDegree(f32);
f32 JMACosDegree(f32);

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);


namespace JMath {
    void gekko_ps_copy12(void *, const void *);
};

class JMathInlineVEC {
public:
    static void PSVECAdd(const Vec *, const Vec *, Vec *);
    static void PSVECSubtract(const Vec *, const Vec *, Vec *);
};