#include "MR/Actor/LiveActorUtil.h"
#include "MR/JMap/JMapUtil.h"
#include "Actor/LiveActor/LiveActor.h"

#include <math.h>

namespace MR
{
    void copyTransRotateScale(const LiveActor *pSource, LiveActor *pDest)
    {
        pDest->mTranslation.set<f32>(pSource->mTranslation);
        pDest->mRotation.set<f32>(pSource->mRotation);
        pDest->mScale.set<f32>(pSource->mScale);
    }

    void initDefaultPos(LiveActor *pActor, const JMapInfoIter &iter)
    {
        if (iter.isValid())
        {
            getJMapInfoTrans(iter, &pActor->mTranslation);
            getJMapInfoRotate(iter, &pActor->mRotation);
            getJMapInfoScale(iter, &pActor->mScale);

            // todo -- why does it omit 0.0? optimization? conversion?
            f32 rmdX = fmod(360.0f + pActor->mRotation.x - 0.0f, 360.0f);
            pActor->mRotation.x = rmdX + 0.0f;
            f32 rmdZ = 360.0f + pActor->mRotation.y - 0.0f;
            f32 rmdY = fmod(rmdZ, 360.0f);
            rmdZ = 360.0f + pActor->mRotation.z - 0.0f;
            pActor->mRotation.y = rmdY;
            pActor->mRotation.z = 0.0f + fmod(rmdZ, 360.0f);
        }
    }

    void initDefaultPosNoRepeat(LiveActor *pActor, const JMapInfoIter &iter)
    {
        getJMapInfoTrans(iter, &pActor->mTranslation);
        getJMapInfoRotate(iter, &pActor->mRotation);
        getJMapInfoScale(iter, &pActor->mScale);
    }

    void calcAnimDirect(LiveActor *pActor)
    {
        bool isCalcAnim = pActor->mFlags.mIsOnCalcAnim;
        pActor->mFlags.mIsOnCalcAnim = false;
        pActor->calcAnim();

        if (isCalcAnim)
        {
            pActor->mFlags.mIsOnCalcAnim = true;
        }
    }
};