#include "Actor/LOD/LodCtrl.h"
#include "arch/printf.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorShadowUtil.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/CameraUtil.h"
#include "MR/ClippingUtil.h"
#include "MR/FileUtil.h"
#include "MR/ModelUtil.h"

LodCtrl::LodCtrl(LiveActor *actor, const JMapInfoIter &iter)
{
    this->mDistToMiddle = 3000.0f;
    this->mDistToLow = 2000.0f;
    this->mModelActor = actor;
    this->mModelObjMiddle = 0;
    this->mModelObjLow = 0;
    this->mIsValid = 0;
    this->_19 = 0;
    this->_1A = 1;
    this->_1B = 0;
    this->_2C = -1;
    this->mLightCtrl = 0;
    this->_1C = val_zero;
    this->_20 = val_zero;
    this->_24 = val_zero;
    this->_28 = val_zero;

    MR::getClippingDirector()->entryLodCtrl(this, iter);
    this->mLightCtrl = this->mModelActor->mLightCtrl;
}

void LodCtrl::offSyncShadowHost()
{
    MR::offShadowVisibleSyncHostAll(this->mModelActor);
    this->_1A = 0;
}

void LodCtrl::appear()
{
    MR::showModel(this->mModelActor);
    this->mLightCtrl = this->mModelActor->mLightCtrl;

    if (this->mModelObjMiddle)
    {
        if (!MR::isDead(this->mModelObjMiddle))
            this->mModelObjMiddle->makeActorDead();
    }

    if (this->mModelObjLow)
    {
        if (!MR::isDead(this->mModelObjLow))
            this->mModelObjLow->makeActorDead();

        if (!this->_1A)
            MR::offShadowVisibleSyncHostAll(this->mModelActor);
   }
}

void LodCtrl::kill()
{
    MR::showModel(this->mModelActor);

    if (this->mModelObjMiddle)
    {
        if (!MR::isDead(this->mModelObjMiddle))
            this->mModelObjMiddle->makeActorDead();
    }

    if (this->mModelObjLow)
    {
        if (!MR::isDead(this->mModelObjLow))
            this->mModelObjLow->makeActorDead();
    }

    if (!this->_1A)
        MR::onShadowVisibleSyncHostAll(this->mModelActor);

    this->mLightCtrl = 0;
}

void LodCtrl::validate()
{
    this->appear();
    this->mIsValid = 1;
}

void LodCtrl::invalidate()
{
    this->kill();
    this->mIsValid = 0;
}

void LodCtrl::update()
{
    if (MR::isDead(this->mModelActor))
        return;

    if (this->mIsValid)
    {
        bool flag = 0;

        if (this->mModelObjMiddle || this->mModelObjLow)
            flag = 1;

        if (flag)
        {
            f32 distToCamera = this->calcDistanceToCamera();
            bool val = *this->_28;
            f32 distToMiddle = this->mDistToMiddle;
            f32 distToLow = this->mDistToLow;

            if (!val)
            {
                if (!*this->_1C)
                {
                    if (this->mModelObjMiddle || !*this->_20)
                    {
                        if (this->mModelObjLow || !*this->_24)
                        {
                            if (distToCamera >= distToMiddle)
                            {
                                if (this->mModelObjMiddle || distToCamera >= distToLow)
                                {
                                    if (!this->mModelObjMiddle || distToCamera >= distToLow)
                                    {
                                        if (this->mModelObjLow)
                                            this->showLowModel();
                                    }
                                    else
                                        this->showMiddleModel();
                                }
                                else
                                    this->showHighModel();
                            }
                            else
                                this->showHighModel();
                        }
                        else
                            this->showLowModel();
                    }
                    else
                        this->showMiddleModel();
                }
                else
                    this->showHighModel();
            }
            else
                this->hideAllModel();

            if (this->mCurrentActiveObj)
            {
                if (this->mCurrentActiveObj != this->mModelActor)
                    MR::copyTransRotateScale(this->mCurrentActiveObj, this->mModelActor);
            }
        }
        else
        {
            if (*this->_28)
                this->hideAllModel();
            else
                this->showHighModel();
        }
    }
}

bool LodCtrl::isShowLowModel() const
{
    LiveActor* modelObj = this->mModelObjLow;
    bool ret = 0;

    if (modelObj)
    {
        if (modelObj == this->mCurrentActiveObj)
            ret = 1;
    }

    return ret;
}

void LodCtrl::setDistanceToMiddle(f32 dist)
{
    this->mDistToMiddle = dist;
}

f32 LodCtrl::calcDistanceToCamera() const
{
    if (!this->_1B)
        return MR::calcDistanceToPlayer(this->mModelActor);
    else
        return MR::calcCameraDistanceZ(this->mModelActor->mTranslation);
}
void LodCtrl::setDistanceToLow(f32 dist)
{
    this->mDistToLow = dist;
}

void LodCtrl::setDistanceToMiddleAndLow(f32 mid, f32 low)
{
    this->mDistToMiddle = mid;
    this->mDistToLow = low;
}

template<typename T>
void LodFuntionCall(LodCtrl *ctrl, void(*func)(LiveActor *, f32), T a3)
{
    func(ctrl->mModelActor, a3);

    if (ctrl->mModelObjMiddle)
        func(ctrl->mModelObjMiddle, a3);

    if (ctrl->mModelObjLow)
        func(ctrl->mModelObjLow, a3);    
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
    if (MR::isHiddenModel(this->mModelActor))
    {
        if (this->mLightCtrl)
            this->mModelActor->mLightCtrl->copy(this->mLightCtrl);

        this->mLightCtrl = this->mModelActor->mLightCtrl;
        MR::showModel(this->mModelActor);
    }
    else
    {
        if (this->mModelObjMiddle)
        {
            if (!MR::isDead(this->mModelObjMiddle))
                this->mModelObjMiddle->makeActorDead();
        }

        if(this->mModelObjLow)
        {
            if (!MR::isDead(this->mModelObjLow))
                this->mModelObjLow->makeActorDead();
        }
    }

    this->mCurrentActiveObj = (ModelObj*)this->mModelActor;
}

void LodCtrl::showMiddleModel()
{
    if (MR::isDead(this->mModelObjMiddle))
    {
        if (this->mLightCtrl)
            this->mModelObjMiddle->mLightCtrl->copy(this->mLightCtrl);

        this->mLightCtrl = this->mModelObjMiddle->mLightCtrl;
        this->mModelObjMiddle->makeActorAppeared();
        MR::calcAnimDirect(this->mModelObjMiddle);
    }
    else
    {
        if (!MR::isHiddenModel(this->mModelActor))
            MR::hideModelAndOnCalcAnim(this->mModelActor);

        if(this->mModelObjLow)
        {
            if (!MR::isDead(this->mModelObjLow))
                this->mModelObjLow->makeActorDead();
        }
    }

    this->mCurrentActiveObj = this->mModelObjMiddle;
}

void LodCtrl::showLowModel()
{
    if (MR::isDead(this->mModelObjLow))
    {
        if (this->mLightCtrl)
            this->mModelObjLow->mLightCtrl->copy(this->mLightCtrl);

        this->mLightCtrl = this->mModelObjLow->mLightCtrl;
        this->mModelObjLow->makeActorAppeared();
        MR::calcAnimDirect(this->mModelObjLow);
    }
    else
    {
        if (!MR::isHiddenModel(this->mModelActor))
            MR::hideModelAndOnCalcAnim(this->mModelActor);

        if(this->mModelObjMiddle)
        {
            if (!MR::isDead(this->mModelObjMiddle))
                this->mModelObjMiddle->makeActorDead();
        }
    }

    this->mCurrentActiveObj = this->mModelObjLow;
}

void LodCtrl::hideAllModel()
{
    if (!MR::isHiddenModel(this->mModelActor))
        MR::hideModelAndOnCalcAnim(this->mModelActor);

    if (this->mModelObjMiddle)
    {
        if (!MR::isDead(this->mModelObjMiddle))
            this->mModelObjMiddle->makeActorDead();
    }

    if (this->mModelObjLow)
    {
        if (!MR::isDead(this->mModelObjLow))
            this->mModelObjLow->makeActorDead();
    }

    this->mCurrentActiveObj = 0;
}

void LodCtrl::setViewCtrlPtr(const bool *a1, const bool *a2, const bool *a3, const bool *a4)
{
    this->_1C = a1;
    this->_20 = a2;
    this->_24 = a3;
    this->_28 = a4;
}

void LodCtrl::createLodModel(s32 a1, s32 a2, s32 a3)
{
    this->mModelObjMiddle = this->initLodModel(a1, a2, a3, 0);
    this->mModelObjLow = this->initLodModel(a1, a2, a3, 1);

    if (this->mModelObjMiddle && this->mModelObjLow)
    {
        this->appear();
        this->mIsValid = 1;
    }
    else
    {
        this->kill();
        this->mIsValid = 0;
    }
}

void LodCtrl::syncMaterialAnimation()
{
    if (this->mModelObjMiddle)
        MR::syncMaterialAnimation(this->mModelObjMiddle, this->mModelActor);

    if (this->mModelObjLow)
        MR::syncMaterialAnimation(this->mModelObjLow, this->mModelActor);
}

void LodCtrl::syncJointAnimation()
{
    if (this->mModelObjMiddle)
        MR::syncJointAnimation(this->mModelObjMiddle, this->mModelActor);

    if (this->mModelObjLow)
        MR::syncJointAnimation(this->mModelObjLow, this->mModelActor);
}

void LodCtrl::initLightCtrl()
{
    if (this->mModelObjMiddle)
        MR::initLightCtrl(this->mModelObjMiddle);

    if (this->mModelObjLow)
        MR::initLightCtrl(this->mModelObjLow);
}

ModelObj* LodCtrl::initLodModel(s32 a1, s32 a2, s32 a3, bool a4) const
{
    const char* objName;
    char* type = "Middle";
    const char* modelName = MR::getModelResName(this->mModelActor);

    if (!a4)
        type = "Low";

    char in;
    snprintf(&in, 0x100, "/ObjectData/%s%s.arc", modelName, type);

    if (!MR::isFileExist(&in, 0))
        return 0;
    else
    {
        if (!a4)
            objName = MR::createMiddleModelObjName(this->mModelActor);
        else
            objName = MR::createLowModelObjName(this->mModelActor);

        snprintf(&in, 0x100, "%s%s", modelName, type);

        // todo -- placed improperly
        Mtx* mtx = this->mModelActor->getBaseMtx();
        ModelObj* obj = new ModelObj(objName, &in, mtx, a1, a2, a3, 0);
        obj->initWithoutIter();
        obj->makeActorAppeared();
        MR::setClippingTypeSphereContainsModelBoundingBox(obj, 100.0f);
        MR::copyTransRotateScale(this->mModelActor, obj);

        return obj;
    }
}

bool LodCtrlFunction::isExistLodLowModel(const char *name)
{
    char in;
    snprintf(&in, 0x100, "/ObjectData/%sLow.arc", name);
    return MR::isFileExist(&in, 0);
}