#ifndef INLINES_H
#define INLINES_H

static inline f32 fabs(f32 src)
{
    f32 ret;
    __asm("fabs %0, %1" : "=f"(ret) : "f"(src));
    return ret;
}

static inline f32 fneg(f32 src)
{
    f32 ret;
    __asm("fneg %0, %1" : "=f"(ret) : "f"(src));
    return ret;
}

#endif // INLINES_H