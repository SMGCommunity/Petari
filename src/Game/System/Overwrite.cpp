#include "Game/System/WPad.hpp"
#include "JSystem/JUtility/JUTException.hpp"
#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JKernel/JKRThread.hpp"
#include <JSystem/JAudio2/JASAudioThread.hpp>

JASAudioThread::JASAudioThread(int stackSize, int msgCount, u32 threadPriority)
    : JKRThread(JASDram, threadPriority, msgCount, stackSize), JASGlobalInstance< JASAudioThread >(true) {
    sbPauseFlag = false;
    OSInitThreadQueue(&sThreadQueue);
}

bool JUTException::readPad(u32* pTrigger, u32* pHold) {
    OSTime startTime = OSGetTime();
    OSTime elapsed;

    do {
        elapsed = OSTicksToMilliseconds(OSGetTime() - startTime);
    } while (elapsed < 50);

    *pHold = 0;
    *pTrigger = 0;

    MR::getPadDataForExceptionNoInit(WPAD_CHAN0, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN1, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN2, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN3, pHold, pTrigger);

    return true;
}
