#include "Actor/LOD/LodCtrl.h"
#include "MR/actor/ActorShadowUtil.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/ClippingUtil.h"

LodCtrl::LodCtrl(LiveActor *actor, const JMapInfoIter &iter)
{
    this->mDistToMiddle = 3000.0f;
    this->mDistToLow = 2000.0f;
    this->_C = actor;
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
    this->mLightCtrl = this->_C->mLightCtrl;
}

void LodCtrl::offSyncShadowHost()
{
    MR::offShadowVisibleSyncHostAll(this->_C);
    this->_1A = 0;
}

void LodCtrl::appear()
{
    MR::showModel(this->_C);
    this->mLightCtrl = this->_C->mLightCtrl;

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
            MR::offShadowVisibleSyncHostAll(this->_C);
   }
}

void LodCtrl::kill()
{
    MR::showModel(this->_C);

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
        MR::onShadowVisibleSyncHostAll(this->_C);

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
    if (MR::isDead(this->_C))
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

            if (this->_8)
            {
                if (this->_8 != this->_C)
                    MR::copyTransRotateScale(this->_8, this->_C);
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
        if (modelObj == this->_8)
            ret = 1;
    }

    return ret;
}

void LodCtrl::setDistanceToMiddle(f32 dist)
{
    this->mDistToMiddle = dist;
}