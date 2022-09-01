#ifndef GXREGS_H
#define GXREGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/base/PPCWGPipe.h>

extern volatile PPCWGPipe gxfifo: 0xCC008000;

#define SET_FLAG(regOrg, newFlag, shift, size)  \
    (regOrg) = (u32)__rlwimi((int)(regOrg), (int)(newFlag), (shift), (32 - (shift) - (size)), (31 - (shift)));   \

/* GX fifo write helpers */
#define GX_WRITE_U8(ub)	    \
    gxfifo.u8 = (u8)(ub)

#define GX_WRITE_U16(us)   \
   gxfifo.u16 = (u16)(us)

#define GX_WRITE_U32(ui)   \
   gxfifo.u32 = (u32)(ui)

#define GX_WRITE_F32(f)	 	\
   gxfifo.f32 = (f32)(f);

#define GX_WRITE_REG(reg)	 	\
   GX_WRITE_U8((0x61));         \
   GX_WRITE_U32((reg))        \

#ifdef __cplusplus
}
#endif

#endif // GXREGS_H