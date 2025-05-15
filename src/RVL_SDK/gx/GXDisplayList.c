#include <revolution/gx.h>
#include <revolution/gx/GXFifo.h>
#include <revolution/gx/GXRegs.h>
#include <revolution/os.h>
#include <cstdio>
#include <mem.h>

static GXFifoObj DisplayListFifo;
static GXFifoObj OldCPUFifo;
static GXData __savedGXdata;

void GXBeginDisplayList(void *list, u32 size)
{
    __GXFifoObj* dlFifo = (__GXFifoObj*)&DisplayListFifo;
    if (gx->dirtyState) {
        __GXSetDirtyState();
    }

    if (gx->dlSaveContext) {
        memcpy(&__savedGXdata, gx, sizeof(*gx));
    }

    dlFifo->base = (u8*)list;
    dlFifo->top = (u8*)list + size - 4;
    dlFifo->size = size;
    dlFifo->count = 0;
    dlFifo->rdPtr = list;
    dlFifo->wrPtr = list;

    gx->inDispList   = GX_TRUE;

    GXFlush();
    GXGetCPUFifo(&OldCPUFifo);
    GXSetCPUFifo(&DisplayListFifo);
    GXResetWriteGatherPipe();
}

u32 GXEndDisplayList(void) {
    GXBool ov;
    BOOL enabled;
    u32 cpenable;

    GXFlush();
    GXGetCPUFifo(&DisplayListFifo);
    ov = GXGetFifoWrap(&DisplayListFifo);

    GXSetCPUFifo(&OldCPUFifo);

    if (gx->dlSaveContext)
    {
        enabled = OSDisableInterrupts();
        cpenable = gx->cpEnable;
        memcpy(gx, &__savedGXdata, sizeof(*gx));
        gx->cpEnable = cpenable;
        OSRestoreInterrupts(enabled);
    }

    gx->inDispList = GX_FALSE;
    
    if (!ov) {
        return GXGetFifoCount(&DisplayListFifo);
    }
    else
    {
        return 0;
    }
}

void GXCallDisplayList(const void *list, u32 nbytes) {
    if (gx->dirtyState)  {
        __GXSetDirtyState();
    }

    if (!(*(u32*)(&gx->vNumNot))) {
        __GXSendFlushPrim();
    }

    GX_WRITE_U8(0x40);
    GX_WRITE_U32((u32)list);
    GX_WRITE_U32((u32)nbytes);
}
