#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long u32;
typedef unsigned long long u64;

u32 __cvt_fp2unsigned(double);
#ifdef __MWERKS__
asm void __div2u(void);
asm void __div2i(void);
asm void __mod2u(void);
asm void __mod2i(void);
asm void __shl2i(void);
asm void __shr2u(void);
#endif
u64 __cvt_dbl_usll(double);
#ifdef __MWERKS__
asm void __cvt_dbl_ull(void);
#endif

#ifdef __cplusplus
}
#endif
