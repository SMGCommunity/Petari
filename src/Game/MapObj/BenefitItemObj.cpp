#include "Game/MapObj/BenefitItemObj.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

s16 def = 600;
static f32 zero = 0.0f;

namespace {
    const f32 cAppearThrowFwdSpd = 5.0f;
    const f32 cBoundUpSpd = 20.0f;
    const f32 cAppearThrowUpSpd = 10.0f;
    const f32 cAppearThrowUpSpd2 = 15.0f;
    const f32 cGravity = 0.5f;
    const f32 cReflectWallX = 0.30000001f;
    static f32 cSpdRotateY = JGeometry::TUtil<f32>::PI() / 60.0f;
};

void ShadowClipActor::endClipped() {
    LiveActor::endClipped();
    if (MR::isClipped(_8C)) {
        _8C->endClipped(); 
    }
}

void ShadowClipActor::control() {
    if (!MR::isClipped(this)) {
        _8C->endClipped();
    }
}

BenefitItemObj::BenefitItemObj(const char *pName, const char *pPowerUp) : LiveActor(pName) {
    _138 = 0;
    _D8 = 0;
    _E2 = 0;
    _E5 = 0;
    _E6 = 0;
    _E4 = 0;
    _12C = 0;
    _EC = 0.0f;
    mFollowMtx = nullptr;
    _DD = 0;
    _DE = 0;
    mCameraInfo = nullptr;
    mHitSensorActor = nullptr;
    _130 = 0;
    _BC.zero();
    _C8.zero();
    _120 = pPowerUp;
    PSMTXIdentity(_F0);
}

void BenefitItemObj::initModelAndEfx() {
    initModelManagerWithAnm(_120, nullptr, false);
    initEffectKeeper(5, _120, false);
    initBinder(50.0f, 50.0f, 0);
    initHitSensor(1);
    MR::addHitSensor(this, "body", 77, 4, 50.0f, TVec3f(0.0f, 50.0f, 0.0f));
}

void BenefitItemObj::init(const JMapInfoIter &rIter) {
    LiveActor::init(rIter);
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
    }

    initModelAndEfx();
    MR::connectToSceneItemStrongLight(this);
    MR::offCalcGravity(this);
    MR::calcUpVec(&mGravity, this);
    mGravity = -mGravity;
    initSound(4, false);
    mFlashCtrl = new FlashingCtrl(this, true);
    MR::offBind(this);
    _E3 = 0;

    if (MR::isValidInfo(rIter)) {
        _E3 = MR::isConnectedWithRail(rIter);
        if (_E3) {
            initRailRider(rIter);
        }
        _E8 = 5.0f;
        s32 railArg;
        if (_E3 && MR::getJMapInfoRailArg0NoInit(this, &railArg)) {
            _E8 = railArg / 10.0f;
        }
    }

    MR::setClippingFar100m(this);
    MR::setClippingTypeSphere(this, 100.0f);
    initNerve(&NrvBenefitItemObj::HostTypeNrvWait::sInstance);
    _DF = 1;
    _D9 = 0;
    _DA = 0;
    _E1 = 0;
    s32 arg0 = -1;
    s32 arg1 = -1;
    s32 arg2 = -1;
    s32 arg3 = -1;
    s32 arg4 = -1;
    s32 arg5 = -1;
    s32 arg6 = -1;
    s32 arg7 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);
        MR::getJMapInfoArg2NoInit(rIter, &arg2);
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        MR::getJMapInfoArg4NoInit(rIter, &arg4);
        MR::getJMapInfoArg5NoInit(rIter, &arg5);
        MR::getJMapInfoArg6NoInit(rIter, &arg6);
        MR::getJMapInfoArg7NoInit(rIter, &arg7);
    }
    else {
        _E1 = 1;
        _DF = 0;
        _DA = 0;
        _D9 = 0;
    }

    switch(arg0) {
        case -1:
        case 0:
            _DF = 0;
            break;
    }

    switch(arg1 & 0xF) {
        case 1:
            _D9 = 1;
            break;
        case 2:
            _D9 = 2;
            break;
        case 3:
            _D9 = 3;
            break;
    }

    if (arg1 != -1 && (arg1 & 0x80) != 0) {
        _DE = 1;
    }

    switch (arg2) {
        case 1:
            _DA = 1;
            break;
        case 2:
            _DA = 2;
            break;
        case 3:
            _DA = 3;
            break;
    }

    bool setClipType = false;
    if (arg3 == 1) {
        setClipType = true;
    }

    _DC = 0;

    switch(arg4) {
        case -1:
            _DC = 1;
            break;
        case 1:
            _DC = 2;
            break;
        case 2:
            _DC = 3;
            break;
        case 3:
            _DC = 4;
            break;
    }
    
    if (arg5 == 1 && MR::isValidInfo(rIter)) {
        MR::initActorCamera(this, rIter, &mCameraInfo);
        _E4 = 1;
    }

    if (arg6 == -1) {
        arg6 = 0;
    }

    _DB = arg6;
    f32 shadowDist = 2000.0f;
    if (arg7 != -1) {
        shadowDist = arg7;
    }

    if (!_D8) {
        if (MR::isValidInfo(rIter)) {
            MR::useStageSwitchWriteDead(this, rIter);

            if (MR::useStageSwitchReadAppear(this, rIter)) {
                MR::FunctorV0M<BenefitItemObj *, void (BenefitItemObj::*)()> appearFunc = MR::Functor<BenefitItemObj>(this, &BenefitItemObj::appear);
                MR::listenStageSwitchOnAppear(this, appearFunc);
                _E1 = 1;
            }
        }
    }

    if (_D8) {
        _DF = 0;
        makeActorDead();
    }
    else {
        if (_DF) {
            makeActorAppeared();
            if (_E1) {
                MR::invalidateHitSensors(this);
            }
            else {
                runEfx("Glow");
            }
        }
        else if (_E1) {
            makeActorDead();
        }
        else {
            makeActorAppeared();
            runEfx("Glow");
        }

        if (!MR::isDead(this) && MR::isValidInfo(rIter)) {
            MR::useStageSwitchSleep(this, rIter);
        }

        TVec3f rotate_axis_y;
        MR::getRotatedAxisY(&rotate_axis_y, mRotation);
        _A4.set(rotate_axis_y);

        if (_DC) {
            MR::initShadowVolumeCylinder(this, 50.0f);
            MR::setShadowDropLength(this, nullptr, shadowDist);

            if (arg7 == -1) {
                MR::onShadowVolumeCutDropLength(this, nullptr);
                MR::setShadowVolumeEndDropOffset(this, nullptr, 50.0f);
            }

            if ((_DC & 0x1) != 0) {
                if ((_DC & 0x2) != 0) {
                    MR::onCalcShadowDropPrivateGravity(this, nullptr);
                    MR::onCalcShadow(this, nullptr);
                }
                else {
                    MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
                    MR::onCalcShadowOneTime(this, nullptr);
                }
            }
            else if ((_DC & 0x2) != 0) {
                MR::onCalcShadow(this, nullptr);
            }
            else {
                MR::onCalcShadowOneTime(this, nullptr);
            }

            if (setClipType) {
                MR::setClippingTypeSphere(this, shadowDist);
            }
        }
    }
}

void BenefitItemObj::initAfterPlacement() {
    if (_E3) {
        MR::moveCoordAndTransToNearestRailPos(this);
    }
}

void BenefitItemObj::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    runEfx("Glow");
}

void BenefitItemObj::appear() {
    LiveActor::appear();
    _E1 = 0;
    _DF = 1;

    switch(_D9) {
        case 0:
            if (_DA) {
                setNerve(&NrvBenefitItemObj::HostTypeNrvPreEscape::sInstance);
            }
            MR::validateHitSensors(this);
            break;
        case 1:
            appearGround();
            break;
        case 2:
            appearThrowUp();
            break;
        case 3:
            appearThrowUpQuestionBox();
            break;
    }

    LiveActor::calcAndSetBaseMtx();
    PSMTXCopy(getBaseMtx(), _F0);

    if (_E4) {
        _E6 = 1;
        MR::invalidateClipping(this);
    }

    if (_DA) {
        MR::invalidateClipping(this);
    }

    if (_DE) {
        MR::emitEffect(this, "AppearPicture");
    }
}

void BenefitItemObj::appearGround() {
    MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
    setNerve(&NrvBenefitItemObj::HostTypeNrvAppearGround::sInstance);
    MR::hideModel(this);
    MR::invalidateClipping(this);
}

void BenefitItemObj::appearThrowUp() {
    MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
    runBck("Appear");
    TVec3f rotated_axis;
    MR::getRotatedAxisY(&rotated_axis, mRotation);
    _A4.set(rotated_axis);
    setNerve(&NrvBenefitItemObj::HostTypeNrvShoot::sInstance);

    if (!_DD) {
        f32 speed = cAppearThrowUpSpd;
        mVelocity = MR::createVecAndScale(rotated_axis, speed);
    }
    else {
        f32 speed = cAppearThrowUpSpd2;
        mVelocity = MR::createVecAndScale(rotated_axis, speed);
    }
    
    MR::invalidateClipping(this);
}

void BenefitItemObj::shoot(const TVec3f &a2, const TVec3f &a3, bool a4) {
    mPosition = a2;
    
    if (_D9) {
        _D9 = 2;
    }
    else {
        _D9 = 3;
    }

    appear();
}

void BenefitItemObj::appearThrowUpQuestionBox() {
    appearThrowUp();
}

void BenefitItemObj::control() {
    if (_E6) {
        if (MR::tryStartDemoWithoutCinemaFrame(this, "出現")) {
            _E6 = 0;
            MR::startActorCameraTargetSelf(this, mCameraInfo, -1);
            MR::requestMovementOn(this);
            MR::pauseOffCameraDirector();
            _12C = MR::getActorCameraFrames(this, mCameraInfo);

            if (_12C == 0) {
                _12C = 120;
            }
        }
    }

    TVec3f v9(mGravity);
    MR::calcGravityOrZero(this);
    calcAndSetBaseMtxInMovement();

    if (_E4) {
        s32 v4 = _12C;
        if (v4) {
            s32 v5 = v4 - 1;
            _12C = v5;
            if (!v5) {
                _E4 = 0;
                MR::endActorCamera(this, mCameraInfo, false, -1);
                MR::endDemo(this, "出現");
                MR::validateClipping(this);
            }
        }
    }

    if (!_E2) {
        if (MR::isBindedGroundDamageFire(this)) {
            runEfx("Vanish");
            kill();
        }

        if (!MR::isNearZero(mVelocity, 0.001f)) {
            if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                runEfx("Vanish");
                kill();
            }
        }
    }
}

#ifdef NON_MATCHING
// close but small diff
void BenefitItemObj::exeWait() {
    if (_DF && !_E1) {
        if (_DB && !_E1) {
            doRotateY();
            return;
        }

        if (_DB & 0x2) {
            doRotateY();
            return;
        }

        if (!(_DB & 0x1)) {
            return;
        }

        doRotateY();
        return;
    }
}
#endif


void BenefitItemObj::doRotateY() {
    if (isNerve(&NrvBenefitItemObj::HostTypeNrvAppearGround::sInstance)) {
        _EC = 0.0f;
    }
    else {
        _EC = (f32)fmod((6.2831855f + ((_EC + cSpdRotateY) - zero)), 6.283185482025146) + zero;
    }
}

void BenefitItemObj::exeShoot() {
    f32 reflect, v2, gravity, v4;

    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    TVec3f v9(mVelocity);
    v2 = MR::vecKillElement(v9, mGravity, &v9);
    if (MR::isBindedGround(this)) {
        runBck("Land");
        MR::validateHitSensors(this);
        mVelocity.zero();
        if (!_DA) {
            if (MR::isGreaterStep(this, 10)) {
                setNerve(&NrvBenefitItemObj::HostTypeNrvWait::sInstance);
            }
        }
        else
            setNerve(&NrvBenefitItemObj::HostTypeNrvEscape::sInstance);
    }
    else {
        gravity = v2;
        gravity += cGravity;
        if (getNerveStep() == 60) {
            MR::validateHitSensors(this);
        }

        if (MR::isBindedWall(this)) {
            v4 = MR::vecKillElement(v9, *MR::getWallNormal(this), &v9); 
            reflect = cReflectWallX;
            v9 -= MR::createVecAndScale(MR::createVecAndScale(*MR::getWallNormal(this), v4), reflect);
        }

        v9 += MR::createVecAndScale(mGravity, gravity);
        mVelocity.set(v9);
    }
}

void BenefitItemObj::exeCatch() {
    if (MR::isFirstStep(this)) {
        runEfx("Get");
        MR::requestOneUp();
        MR::hideModel(this);
        MR::incPlayerLeft();
        MR::explainOneUpIfAtFirst();
        kill();
    }

    mVelocity.x = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.z = 0.0f;
    mPosition.set(mHitSensorActor->mPosition);
}

bool BenefitItemObj::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    return MR::isMsgStarPieceReflect(msg);
}

bool BenefitItemObj::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::receiveItemShowMsg(msg, a2, a3)) {
        return true;
    }

    if (MR::receiveItemHideMsg(msg, a2, a3)) {
        return true;
    }

    if (MR::isMsgItemStartMove(msg)) {
        _E2 = 1;
        return true;
    }

    if (MR::isMsgItemEndMove(msg)) {
        _E2 = 0;
        return true;
    }

    if (msg == 135) {
        if (mHitSensorActor) {
            return false;
        }

        if (isNerve(&NrvBenefitItemObj::HostTypeNrvWait::sInstance) 
            || isNerve(&NrvBenefitItemObj::HostTypeNrvShoot::sInstance) 
            || isNerve(&NrvBenefitItemObj::HostTypeNrvAppearGround::sInstance) 
            || isNerve(&NrvBenefitItemObj::HostTypeNrvEscape::sInstance)) {
                mHitSensorActor = a2->mActor;
                setNerve(&NrvBenefitItemObj::HostTypeNrvCatch::sInstance);
                MR::tryRumblePadWeak(this, 0);
                return true;
        }
    }

    return false;
}

void BenefitItemObj::exeAppearGround() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    if (getNerveStep() >= 60) {
        if (getNerveStep() == 60) {
            MR::showModel(this);
            runBck("Appear");
            MR::startSound(this, "SE_OJ_KINOKO_1UP_GROW", -1, -1);
        }

        if (MR::isBckStopped(this)) {
            if (_DA) {
                setNerve(&NrvBenefitItemObj::HostTypeNrvPreEscape::sInstance);
            }
            else {
                setNerve(&NrvBenefitItemObj::HostTypeNrvWait::sInstance);
            }

            MR::validateHitSensors(this);
        }
    }
}

void BenefitItemObj::exePreEscape() {
    if (getNerveStep() >= 30) {
        setNerve(&NrvBenefitItemObj::HostTypeNrvEscape::sInstance);
    }
}

void BenefitItemObj::initEscape() {
    _B0 = def;
    _B2 = 0;
    _B4 = 0;
    _B6 = 0;
    _B8 = 0;
    TVec3f axis_z;
    MR::getRotatedAxisZ(&axis_z, mRotation);
    TVec3f stack_8(axis_z);
    stack_8.scale(7.0f);
    _BC = stack_8;
    MR::startSound(this, "SE_OJ_KINOKO_1UP_RUN_START", -1, -1);
}

void BenefitItemObj::doEscape() {
    MR::startLevelSound(this, "SE_OJ_LV_KINOKO_1UP_RUN", -1, -1, -1);
    bool cond = false;
    if (_DA == 2) {
        cond = true;
    }

    if (MR::isBindedGround(this)) {
        f32 mag = PSVECMag(_BC.toCVec());
        MR::vecKillElement(_BC, mGravity, &_BC);
        _BC.setLength(mag);

        if (cond) {
            TVec3f* player_pos = MR::getPlayerPos();
            TVec3f v22(mPosition);
            v22 -= *player_pos;
            MR::vecKillElement(v22, mGravity, &v22);
            MR::normalizeOrZero(&v22);
            if (_BC.dot(v22) < 0.0f) {
                _BC += MR::createVecAndScale(v22, 0.2f);
                _BC.x *= 1.1f;
                _BC.y *= 1.1f;
                _BC.z *= 1.1f;
            }
            else {
                _BC += MR::createVecAndScale(v22, 0.0099999998f);
                _BC.x *= 0.0099999998f;
                _BC.y *= 0.0099999998f;
                _BC.z *= 0.0099999998f;
            }
        } 

        if (PSVECMag(_BC.toCVec()) > 7.0f) {
            _BC.setLength(7.0f);
        }

        if (_E5) { 
            _E5 = 0;
            runBck("Land");
        }

        _C8 = MR::createVecAndScale(mGravity, 5.0f);
        runEfx("Move");
    }
    else { 
        _E5 = 1;
        _BC.x *= 0.94999999f;
        _BC.y *= 0.94999999f;
        _BC.z *= 0.94999999f;
        _C8 += MR::createVecAndScale(mGravity, 1.0f); 

        if (PSVECMag(_C8.toCVec()) > 20.0f) {
            _C8.setLength(20.0f);
        }

        stopEfx("Move"); 
    }

    if (MR::isBindedWall(this) && !_E5) {
        if (MR::getWallNormal(this)->dot(mGravity) < 0.0f) {
            f32 scalar = MR::vecKillElement(_BC, *MR::getWallNormal(this), &_BC);
            _BC += -*MR::getWallNormal(this) % scalar;
            _C8 += -mGravity % 20.0f;
            _E5 = 1; 
        } 
    }
}

void BenefitItemObj::exeEscape() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
        MR::onBind(this);
        initEscape(); 
    }

    doEscape();

    if (_B0) {
        MR::showModel(this);
        s16 v1 = _B0;
        _B0 = v1 - 1;
        s16 v2 = _B8;

        if (v2) {
            _B8--;
            MR::hideModel(this);
        }

        if (_B6) {
            s16 v3 = _B6 - 1;
            _B6--;

            if ((s16)v3) {
                _B6 = _B2;
                _B8 = _B4;
            }
        }

        if (_B0 == 0x3C) {
            _B2 = 4;
            _B4 = 2;
        }
        else if (_B0 == 0x78) {
            _B2 = 8;
            _B4 = 3;
        }
        else if (_B0 == 0xB4) {
            _B2 = 12;
            _B4 = 4;
            _B6 = 12;
            _B8 = 4;
        }

        if (!_B0) {
            runEfx("Vanish");
            makeActorDead();
        }
    }

    if (_E3) {
        MR::moveCoord(this, _E8);
        TVec3f railPos(MR::getRailPos(this));
        railPos -= mPosition;
        _BC = railPos;
    }

    mVelocity = _BC;
    mVelocity += _C8;

    if ((_DB & 0x4) != 0) {
        doRotateY();
    }

    if (_DC && (_DC & 0x2) == 0) {
        if ((_DC & 0x1) != 0) {
            MR::onCalcShadowOneTime(this, nullptr);
        }
        else {
            MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
        }
    }
}

void BenefitItemObj::calcAndSetBaseMtx() {

}

void BenefitItemObj::calcAndSetBaseMtxInMovement() {
    if (isNerve(&NrvBenefitItemObj::HostTypeNrvWait::sInstance)) {
        LiveActor::calcAndSetBaseMtx();
        PSMTXCopy(getBaseMtx(), _F0);
        if (mFollowMtx) {
            PSMTXCopy(mFollowMtx, _F0);
            MR::extractMtxTrans(mFollowMtx, &mPosition);
        }

        MtxPtr mtx = MR::tmpMtxRotYRad(_EC);
        PSMTXConcat(_F0, mtx, getBaseMtx());
    }
    else {
        if (MR::isBindedGround(this)) {
            _A4 = *MR::getGroundNormal(this);
        }
        else if (MR::isBindedWall(this)) {
            _A4 = *MR::getWallNormal(this);
        }

        TVec3f stack_8(mVelocity);
        MR::normalizeOrZero(&stack_8);
        if (isNerve(&NrvBenefitItemObj::HostTypeNrvPreEscape::sInstance)) {
            MR::vecBlendSphere(_98, stack_8, &stack_8, 0.1f);
            _98 = stack_8;
        }

        if (!MR::isNearZero(stack_8, 0.001f)) {
            if (MR::isSameDirection(_A4, stack_8, 0.0099999998f)) {
                MR::setMtxTrans(getBaseMtx(), mPosition);
            }
            else {
                TPos3f pos;
                MR::makeMtxUpFrontPos(&pos, _A4, stack_8, mPosition);
                MR::blendMtx(_F0, pos.toMtxPtr(), 0.2f, _F0);
                MtxPtr mtx = MR::tmpMtxRotYRad(_EC);
                PSMTXConcat(_F0, mtx, getBaseMtx());
                MR::setMtxTrans(getBaseMtx(), mPosition);
            }
        }
    }
}

void BenefitItemObj::setFollowMtx(MtxPtr mtx) {
    mFollowMtx = mtx;
    MR::extractMtxTrans(mtx, &mPosition);
}

void BenefitItemObj::runBck(const char *pBckName) {
    MR::startBck(this, pBckName, nullptr);
}

void BenefitItemObj::runEfx(const char *pEfxName) {
    MR::emitEffect(this, pEfxName);
}

void BenefitItemObj::stopEfx(const char *pEfxName) {
    MR::deleteEffect(this, pEfxName);
}
