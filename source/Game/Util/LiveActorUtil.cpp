#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"

namespace MR {
    void calcAnimDirect(LiveActor *pActor) {
        bool isNoCalcAnim = pActor->mFlags.mIsNoCalcAnim;
        pActor->mFlags.mIsNoCalcAnim = false;
        pActor->calcAnim();

        if (isNoCalcAnim) {
            pActor->mFlags.mIsNoCalcAnim = true;
        }
    }
};
