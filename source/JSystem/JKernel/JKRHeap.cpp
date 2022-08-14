#include "JSystem/JKernel/JKRHeap.h"

JKRHeap* JKRHeap::sCurrentHeap;
JKRHeap* JKRHeap::sRootHeap;
JKRHeap* JKRHeap::sSystemHeap;

JKRHeap* JKRHeap::becomeCurrentHeap() {
    JKRHeap* cur = sCurrentHeap;
    sCurrentHeap = this;
    return cur;
}