#include "JSystem/JAudio2/JASDvdThread.hpp"
#include "JSystem/JAudio2/JASTaskThread.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"

namespace JASDvd {
    static JASTaskThread* sThread;
};  // namespace JASDvd

JASTaskThread* JASDvd::getThreadPointer() {
    return JASDvd::sThread;
}

bool JASDvd::createThread(s32 priority, int msgCount, u32 stackSize) {
    if (sThread != NULL) {
        return false;
    }

    sThread = new (JASDram, 0) JASTaskThread(priority, msgCount, stackSize);

    JKRHeap* pCurrentHeap = JKRGetSystemHeap();

    sThread->setCurrentHeap(pCurrentHeap);
    sThread->resume();
    return true;
}
