#include "JSystem/JUtility/JUTXfb.hpp"

void JUTXfb::clearIndex() {
    mDrawingXfbIndex = -1;
    mDrawnXfbIndex = -1;
    mDisplayingXfbIndex = -1;
}

JUTXfb* JUTXfb::sManager;

JUTXfb::~JUTXfb() {
    for (int i = 0; i < 3; i++) {
        delXfb(i);
    }
    sManager = nullptr;
}

void JUTXfb::delXfb(int xfbIdx) {
    if (mXfbAllocated[xfbIdx] && mXfbs[xfbIdx]) {
        delete mXfbs[xfbIdx];
    }
}

JUTXfb::JUTXfb(void* a1, void* a2, void* a3) {
    mBufferNum = 0x3;
    clearIndex();
    mSDrawingFlag = 0x63;
    initiate(a1, a2, a3, UNK_3);
}

JUTXfb* JUTXfb::createManager(void* a1, void* a2, void* a3) {
    if (sManager == nullptr) {
        sManager = new JUTXfb(a1, a2, a3);
    }
    return sManager;
}

void JUTXfb::destroyManager() {
    delete sManager;
    sManager = nullptr;
}

void JUTXfb::initiate(void* a1, void* a2, void* a3, JUTXfb::EXfbNumber num) {
    mXfbs[0] = a1;

    if (num < 2) {
        a2 = nullptr;
    }

    mXfbs[1] = a2;

    if (num < 3) {
        a3 = nullptr;
    }

    mXfbs[2] = a3;
    mXfbAllocated[0] = false;
    mXfbAllocated[1] = false;
    mXfbAllocated[2] = false;
}
