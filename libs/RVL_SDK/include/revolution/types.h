#ifndef TYPES_H
#define TYPES_H

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;

typedef int BOOL;

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef nullptr
#ifdef __cplusplus
#define nullptr 0
#endif
#endif

#ifndef override
#ifdef __cplusplus
#define override
#endif
#endif

#ifdef __MWERKS__
#define __REGISTER register
#else
#define __REGISTER
#endif

#if !defined(AT_ADDRESS)
#if defined(__MWERKS__)
#define AT_ADDRESS(x)							: x
#else
#define AT_ADDRESS(x)
#endif
#endif

#define NO_INLINE __attribute__((noinline))

#if __MWERKS__
#define ATTRIBUTE_ALIGN(num) __attribute__((aligned(num)))
#else
#define ATTRIBUTE_ALIGN(num)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ROUND_UP(x, align) (((x) + (align) - 1) & (-(align)))
#define ROUND_UP_PTR(x, align) ((void*)((((u32)(x)) + (align) - 1) & (~((align) - 1))))

#define ALIGN_PREV(X, N) ((X) & ~((N) - 1))
#define ALIGN_NEXT(X, N) ALIGN_PREV(((X) + (N) - 1), N)

#define ARRAY_SIZE(o) (s32)(sizeof(o) / sizeof(o[0]))
#define ARRAY_SIZEU(o) (sizeof(o) / sizeof(o[0]))

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* just some common intrinsics */

#ifndef __MWERKS__
f32 __frsqrte(f32);
u32 __cntlzw(u32);
s32 __abs(s32);
f32 __fabsf(f32);
void* __memcpy(void*, const void*, int);
#endif

#endif  // TYPES_H
