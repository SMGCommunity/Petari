#ifndef TYPES_H
#define TYPES_H

typedef signed char         s8;
typedef signed short        s16;
typedef signed long         s32;
typedef signed long long    s64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef long                u32;
typedef long long           u64;
typedef float               f32;
typedef double              f64;

struct Vec
{
    //void operator=(const Vec &in);

    f32 x;
    f32 y;
    f32 z;
};

typedef f32 Mtx[3][4];
typedef f32 Mtx33[3][3];
typedef f32 Mtx44[4][4];

#endif // TYPES_H