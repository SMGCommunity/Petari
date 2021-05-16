#include "LiveActor/LOD/LodCtrl.h"
#include <stdio.h>
#include "Util/ActorMovementUtil.h"
#include "Util/ActorShadowUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/CameraUtil.h"
#include "Util/ClippingUtil.h"
#include "Util/FileUtil.h"
#include "Util/ModelUtil.h"

LodCtrl::LodCtrl(LiveActor *pActor, const JMapInfoIter &iter)
{
    mDistToMiddle = 3000.0f;
    mDistToLow = 2000.0f;
    mModelActor = pActor;
    mModelObjMiddle = 0;
    mModelObjLow = 0;
    mIsValid = 0;
    _19 = 0;
    mIsShadowVisible = 1;
    _1B = 0;
    _2C = -1;
    mLightCtrl = 0;
    _1C = val_zero;
    _20 = val_zero;
    _24 = val_zero;
    _28 = val_zero;

    // assign our LodCtrl to the clipping director
    MR::getClippingDirector()->entryLodCtrl(this, iter);
    // assign the source model actor's light control to this
    mLightCtrl = mModelActor->mLightCtrl;
}

// turn off syncing shadows, and set the respective flag
void LodCtrl::offSyncShadowHost()
{
    MR::offShadowVisibleSyncHostAll(mModelActor);
    mIsShadowVisible = 0;
}

// makes the model actor, light, and all of the model objects appear if they exist
void LodCtrl::appear()
{
    MR::showModel(mModelActor);
    mLightCtrl = mModelActor->mLightCtrl;

    if (mModelObjMiddle)
    {
        if (!MR::isDead(mModelObjMiddle))
        {
            mModelObjMiddle->makeActorDead();
        }
    }

    if (mModelObjLow)
    {
        if (!MR::isDead(mModelObjLow))
        {
            mModelObjLow->makeActorDead();
        }

        if (!mIsShadowVisible)
        {
            MR::offShadowVisibleSyncHostAll(mModelActor);
        }
   }
}

void LodCtrl::kill()
{
    MR::showModel(mModelActor);

    if (mModelObjMiddle)
    {
        if (!MR::isDead(mModelObjMiddle))
        {
            mModelObjMiddle->makeActorDead();
        }
    }

    if (mModelObjLow)
    {
        if (!MR::isDead(mModelObjLow))
        {
            mModelObjLow->makeActorDead();
        }
    }

    if (!mIsShadowVisible)
    {
        MR::onShadowVisibleSyncHostAll(mModelActor);
    }

    mLightCtrl = 0;
}

void LodCtrl::validate()
{
    appear();
    mIsValid = 1;
}

void LodCtrl::invalidate()
{
    kill();
    mIsValid = 0;
}

void LodCtrl::update()
{
    if (MR::isDead(mModelActor))
    {
        return;
    }

    if (mIsValid)
    {
        bool flag = 0;

        if (mModelObjMiddle || mModelObjLow)
        {
            flag = 1;
        }

        if (flag)
        {
            f32 distToCamera = calcDistanceToCamera();
            bool val = *_28;
            f32 distToMiddle = mDistToMiddle;
            f32 distToLow = mDistToLow;

            if (!val)
            {
                if (!*_1C)
                {
                    if (mModelObjMiddle || !*_20)
                    {
                        if (mModelObjLow || !*_24)
                        {
                            if (distToCamera >= distToMiddle)
                            {
                                if (mModelObjMiddle || distToCamera >= distToLow)
                                {
                                    if (!mModelObjMiddle || distToCamera >= distToLow)
                                    {
                                        if (mModelObjLow)
                                        {
                                            showLowModel();
                                        }
                                    }
                                    else
                                    {
                                        showMiddleModel();
                                    }
                                }
                                else
                                {
                                    showHighModel();
                                }
                            }
                            else
                            {
                                showHighModel();
                            }
                        }
                        else
                        {
                            showLowModel();
                        }
                    }
                    else
                    {
                        showMiddleModel();
                    }
                }
                else
                {
                    showHighModel();
                }
            }
            else
            {
                hideAllModel();
            }

            if (mCurrentActiveObj)
            {
                if (mCurrentActiveObj != mModelActor)
                {
                    MR::copyTransRotateScale(mCurrentActiveObj, mModelActor);
                }
            }
        }
        else
        {
            if (*_28)
            {
                hideAllModel();
            }
            else
            {
                showHighModel();
            }
        }
    }
}

bool LodCtrl::isShowLowModel() const
{
    LiveActor *pModelObj = mModelObjLow;
    bool ret = 0;

    if (pModelObj)
    {
        if (pModelObj == mCurrentActiveObj)
        {
            ret = 1;
        }
    }

    return ret;
}

void LodCtrl::setDistanceToMiddle(f32 dist)
{
    mDistToMiddle = dist;
}

f32 LodCtrl::calcDistanceToCamera() const
{
    if (!_1B)
    {
        return MR::calcDistanceToPlayer(mModelActor);
    }
    else
    {
        return MR::calcCameraDistanceZ(mModelActor->mTranslation);
    }
}
void LodCtrl::setDistanceToLow(f32 dist)
{
    mDistToLow = dist;
}

void LodCtrl::setDistanceToMiddleAndLow(f32 mid, f32 low)
{
    mDistToMiddle = mid;
    mDistToLow = low;
}

template<typename T>
void LodFuntionCall(LodCtrl *pCtrl, void(*pFunc)(LiveActor *, f32), T a3)
{
    pFunc(pCtrl->mModelActor, a3);

    if (pCtrl->mModelObjMiddle)
    {
        pFunc(pCtrl->mModelObjMiddle, a3);
    }

    if (pCtrl->mModelObjLow)
    {
        pFunc(pCtrl->mModelObjLow, a3);
    } 
}

void LodCtrl::setClippingTypeSphereContainsModelBoundingBox(f32 arg)
{
    LodFuntionCall<f32>(this, MR::setClippingTypeSphereContainsModelBoundingBox, arg);
}

void LodCtrl::setFarClipping(f32 clipping)
{
    LodFuntionCall<f32>(this, MR::setClippingFar, clipping);
}

/*
void LodCtrl::invalidateClipping()
{
    LodFuntionCall(this, MR::invalidateClipping);
}
*/

void LodCtrl::showHighModel()
{
    if (MR::isHiddenModel(mModelActor))
    {
        if (mLightCtrl)
        {
            mModelActor->mLightCtrl->copy(mLightCtrl);
        }

        mLightCtrl = mModelActor->mLightCtrl;
        MR::showModel(mModelActor);
    }
    else
    {
        if (mModelObjMiddle)
        {
            if (!MR::isDead(mModelObjMiddle))
            {
                mModelObjMiddle->makeActorDead();
            }
        }

        if(mModelObjLow)
        {
            if (!MR::isDead(mModelObjLow))
            {
                mModelObjLow->makeActorDead();
            }
        }
    }

    mCurrentActiveObj = (ModelObj*)mModelActor;
}

void LodCtrl::showMiddleModel()
{
    if (MR::isDead(mModelObjMiddle))
    {
        if (mLightCtrl)
        {
            mModelObjMiddle->mLightCtrl->copy(mLightCtrl);
        }

        mLightCtrl = mModelObjMiddle->mLightCtrl;
        mModelObjMiddle->makeActorAppeared();
        MR::calcAnimDirect(mModelObjMiddle);
    }
    else
    {
        if (!MR::isHiddenModel(mModelActor))
        {
            MR::hideModelAndOnCalcAnim(mModelActor);
        }

        if(mModelObjLow)
        {
            if (!MR::isDead(mModelObjLow))
            {
                mModelObjLow->makeActorDead();
            }
        }
    }

    mCurrentActiveObj = mModelObjMiddle;
}

void LodCtrl::showLowModel()
{
    if (MR::isDead(mModelObjLow))
    {
        if (mLightCtrl)
        {
            mModelObjLow->mLightCtrl->copy(mLightCtrl);
        }

        mLightCtrl = mModelObjLow->mLightCtrl;
        mModelObjLow->makeActorAppeared();
        MR::calcAnimDirect(mModelObjLow);
    }
    else
    {
        if (!MR::isHiddenModel(mModelActor))
        {
            MR::hideModelAndOnCalcAnim(mModelActor);
        }

        if(mModelObjMiddle)
        {
            if (!MR::isDead(mModelObjMiddle))
            {
                mModelObjMiddle->makeActorDead();
            }
        }
    }

    mCurrentActiveObj = mModelObjLow;
}

void LodCtrl::hideAllModel()
{
    if (!MR::isHiddenModel(mModelActor))
    {
        MR::hideModelAndOnCalcAnim(mModelActor);
    }

    if (mModelObjMiddle)
    {
        if (!MR::isDead(mModelObjMiddle))
        {
            mModelObjMiddle->makeActorDead();
        }
    }

    if (mModelObjLow)
    {
        if (!MR::isDead(mModelObjLow))
        {
            mModelObjLow->makeActorDead();
        }
    }

    mCurrentActiveObj = 0;
}

void LodCtrl::setViewCtrlPtr(const bool *p1, const bool *p2, const bool *p3, const bool *p4)
{
    _1C = p1;
    _20 = p2;
    _24 = p3;
    _28 = p4;
}

void LodCtrl::createLodModel(s32 a1, s32 a2, s32 a3)
{
    mModelObjMiddle = initLodModel(a1, a2, a3, 0);
    mModelObjLow = initLodModel(a1, a2, a3, 1);

    if (mModelObjMiddle && mModelObjLow)
    {
        appear();
        mIsValid = 1;
    }
    else
    {
        kill();
        mIsValid = 0;
    }
}

void LodCtrl::syncMaterialAnimation()
{
    if (mModelObjMiddle)
    {
        MR::syncMaterialAnimation(mModelObjMiddle, mModelActor);
    }

    if (mModelObjLow)
    {
        MR::syncMaterialAnimation(mModelObjLow, mModelActor);
    }
}

void LodCtrl::syncJointAnimation()
{
    if (mModelObjMiddle)
    {
        MR::syncJointAnimation(mModelObjMiddle, mModelActor);
    }

    if (mModelObjLow)
    {
        MR::syncJointAnimation(mModelObjLow, mModelActor);
    }
}

void LodCtrl::initLightCtrl()
{
    if (mModelObjMiddle)
    {    
        MR::initLightCtrl(mModelObjMiddle);
    }

    if (mModelObjLow)
    {
        MR::initLightCtrl(mModelObjLow);
    }
}

ModelObj* LodCtrl::initLodModel(s32 a1, s32 a2, s32 a3, bool a4) const
{
    const char* pObjName;
    char* pType = "Middle";
    const char* pModelName = MR::getModelResName(mModelActor);

    if (!a4)
    {
        pType = "Low";
    }

    char in;
    snprintf(&in, 0x100, "/ObjectData/%s%s.arc", pModelName, pType);

    if (!MR::isFileExist(&in, 0))
    {
        return 0;
    }
    else
    {
        if (!a4)
        {
            pObjName = MR::createMiddleModelObjName(mModelActor);
        }
        else
        {
            pObjName = MR::createLowModelObjName(mModelActor);
        }

        snprintf(&in, 0x100, "%s%s", pModelName, pType);

        // todo -- placed improperly
        Mtx* pMtx = mModelActor->getBaseMtx();
        ModelObj* pObj = new ModelObj(pObjName, &in, pMtx, a1, a2, a3, 0);
        pObj->initWithoutIter();
        pObj->makeActorAppeared();
        MR::setClippingTypeSphereContainsModelBoundingBox(pObj, 100.0f);
        MR::copyTransRotateScale(mModelActor, pObj);

        return pObj;
    }
}

bool LodCtrlFunction::isExistLodLowModel(const char *pName)
{
    char in;
    snprintf(&in, 0x100, "/ObjectData/%sLow.arc", pName);
    return MR::isFileExist(&in, 0);
}