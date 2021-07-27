#pragma once

#include <revolution/mtx.h>

void JMAVECScaleAdd(const Vec *, const Vec *, Vec *, f32);
void JMAVECLerp(const Vec *, const Vec *, Vec *, f32);
void JMAMTXApplyScale(const Mtx *, Mtx *, f32, f32, f32);

namespace JMathInlineVEC
{
    void PSVECAdd(const Vec *, const Vec *, Vec *);
    void PSVECSubtract(const Vec *, const Vec *, Vec *);
};