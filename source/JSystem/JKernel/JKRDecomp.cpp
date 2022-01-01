#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "revolution.h"

JKRDecomp::JKRDecomp(long a1) : JKRThread(0x4000, 0x10, a1) {
    OSResumeThread(mThread);
}

JKRDecomp::~JKRDecomp() {

}








JKRDecomp *JKRDecomp::create(long a1) {
    if (sDecompInstance == NULL) {
        sDecompInstance = new(JKRHeap::sGameHeap, 0) JKRDecomp(a1);
    }

    return sDecompInstance;
}