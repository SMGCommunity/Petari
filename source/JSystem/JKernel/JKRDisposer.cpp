#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRDisposer::JKRDisposer() : mLink(this) {
    mHeap = JKRHeap::findFromRoot(this);

    if (mHeap) {
        mHeap->mDisposerList.append(&mLink);
    }
}

JKRDisposer::~JKRDisposer() {
    if (mHeap) {
        mHeap->mDisposerList.remove(&mLink);
    }
}
