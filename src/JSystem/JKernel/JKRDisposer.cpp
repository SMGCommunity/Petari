#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

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
