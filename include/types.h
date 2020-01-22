#ifndef TYPES_H
#define TYPES_H

#define offsetof(type, member) ((size_t) & (((type *) 0)->member))

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

typedef volatile u8         vu8;
typedef volatile u16        vu16;
typedef volatile u32        vu32;
typedef volatile u64        vu64;
typedef volatile s8         vs8;
typedef volatile s16        vs16;
typedef volatile s32        vs32;
typedef volatile s64        vs64;

#define NULL 0
#define override
#define noexcept
#define nullptr NULL

typedef f32 Mtx[3][4];
typedef f32 Mtx33[3][3];
typedef f32 Mtx44[4][4];

typedef u32 size_t;
typedef s32 PtrDiff;

#endif // TYPES_H