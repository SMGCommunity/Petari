#include <revolution/gd/GDBase.h>

GDLObj* __GDCurrentDL = NULL;
static GDOverflowCallback overflowcb = NULL;

void GDInitGDLObj(GDLObj* pDL, void* pStart, u32 length) {
    pDL->start = pStart;
    pDL->ptr = (u8*)pStart;
    pDL->top = (u8*)pStart + length;
    pDL->length = length;
}

void GDFlushCurrToMem() {
    DCFlushRange(__GDCurrentDL->start, __GDCurrentDL->length);
}

void GDPadCurr32() {
    u32 n = ((u32)__GDCurrentDL->ptr & 31);
    if (n) {
        for (; n < 32; n++) {
            __GDWrite(0);
        }
    }
}

void GDOverflowed() {
    if (overflowcb) {
        (*overflowcb)();
    }
}
