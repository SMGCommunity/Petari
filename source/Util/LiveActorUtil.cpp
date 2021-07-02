#include "Util/LiveActorUtil.h"
#include "Util/JMapUtil.h"
#include "Util/ModelUtil.h"
#include "System/Resource/ResourceHolder.h"
#include "LiveActor/LiveActor.h"

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

    bool isValidMovement(const LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped)
        {
            return false;
        }

        return true;
    }

    bool isValidCalcAnim(const LiveActor *pActor)
    {
        if (pActor->mFlags.mIsDead || !pActor->mFlags.mIsClipped || pActor->mFlags.mIsOnCalcAnim)
        {
            return false;
        }
        
        return true;
    }

    bool isValidcalcViewAndEntry(const LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsDead || pActor->mFlags.mIsClipped)
        {
            return false;
        }

        return true;
    }

    bool isValidDraw(const LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsDead || !pActor->mFlags.mIsClipped || pActor->mFlags.mIsNoEntryDrawBuffer)
        {
            return false;
        }

        return false;
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

    bool isDead(const LiveActor *pActor)
    {
        return pActor->mFlags.mIsDead;
    }

    bool isHiddenModel(const LiveActor *pActor)
    {
        return pActor->mFlags.mIsNoEntryDrawBuffer;
    }

    void showModel(LiveActor *pActor)
    {
        if (pActor->mFlags.mIsOnCalcAnim)
        {
            pActor->mFlags.mIsOnCalcAnim = false;
        }

        if (pActor->mFlags.mIsNoCalcView)
        {
            pActor->mFlags.mIsNoCalcView = false;
        }

        if (pActor->mFlags.mIsNoEntryDrawBuffer)
        {
            MR::onEntryDrawBuffer(pActor);
        }
    }

    void hideModel(LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsOnCalcAnim)
        {
            pActor->mFlags.mIsOnCalcAnim = true;
        }

        if (!pActor->mFlags.mIsNoCalcView)
        {
            pActor->mFlags.mIsNoCalcView = true;
        }

        if (!pActor->mFlags.mIsNoEntryDrawBuffer)
        {
            MR::offEntryDrawBuffer(pActor);
        }
    }

    void hideModelAndOnCalcAnim(LiveActor *pActor)
    {
        MR::hideModel(pActor);

        pActor->mFlags.mIsOnCalcAnim = false;
    }

    void showModelIfHidden(LiveActor *pActor)
    {
        if (pActor->mFlags.mIsNoEntryDrawBuffer)
        {
            MR::showModel(pActor);
        }
    }

    const char *getModelResName(const LiveActor *pActor)
    {
        ResourceHolder* res = MR::getModelResourceHolder(pActor);
        
        return res->mResourceTable->getResName((u32)0);
    }

    bool isClipped(const LiveActor *pActor)
    {
        return pActor->mFlags.mIsClipped;
    }

    bool isNoEntryDrawBuffer(const LiveActor *pActor)
    {
        return pActor->mFlags.mIsNoEntryDrawBuffer;
    }

    bool isNoBind(const LiveActor *pActor)
    {
        return pActor->mFlags.mIsOnBind;
    }

    void onBind(LiveActor *pActor)
    {
        pActor->mFlags.mIsOnBind = false;
    }

    void offBind(LiveActor *pActor)
    {
        pActor->mFlags.mIsOnBind = true;
    }

    bool isCalcGravity(LiveActor *pActor)
    {
        return pActor->mFlags.mIsCalcGravity;
    }

    void onCalcGravity(LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsDead)
        {
            calcGravity(pActor);
        }

        pActor->mFlags.mIsCalcGravity = true;
    }

    void offCalcGravity(LiveActor *pActor)
    {
        pActor->mFlags.mIsCalcGravity = false;
    }

    bool isValidDraw(LiveActor *pActor)
    {
        if (!pActor->mFlags.mIsDead || !pActor->mFlags.mIsClipped || pActor->mFlags.mIsNoEntryDrawBuffer)
        {
            return false;
        }

        return true;
    }

    void initLightCtrl(LiveActor *pActor)
    {
        pActor->initActorLightCtrl();
        pActor->mLightCtrl->init(-1, false);
    }

    void updateLightControl(LiveActor *pActor)
    {
        pActor->mLightCtrl->update(false);
    }

    void setBaseTMtx(LiveActor *pActor, Mtx mtx)
    {
        J3DModel* pModel = getJ3DModel(pActor);
        PSMTXCopy(mtx, pModel->_24);
    }

    bool isStep(const LiveActor *pActor, s32 step)
    {
        return step == pActor->getNerveStep();
    }

    bool isFirstStep(const LiveActor *pActor)
    {
        return MR::isStep(pActor, 0);
    }
};