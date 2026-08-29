#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    f32 cAppearThrowUpSpd = 10.0f;
    f32 cAppearThrowUpSpd2 = 15.0f;
    f32 cGravity = 0.5f;
    f32 cReflectWallX = 0.3f;
    static f32 cSpdRotateY = JGeometry::TUtil< f32 >::PI() / 60.0f;
    static u16 sVal = 0x0258;
};  // namespace

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

BenefitItemObj::BenefitItemObj(const char* pName, const char* pPowerUp) : LiveActor(pName), _138() {
    _D8 = 0;
    _E2 = 0;
    _E5 = 0;
    _E6 = 0;
    _E4 = 0;
    _12C = 0;
    _EC = 0;
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
    MR::addHitSensor(this, "body", ATYPE_KINOKO_ONEUP, 4, 50.0f, TVec3f(0.0f, 50.0f, 0.0f));
}

void BenefitItemObj::init(const JMapInfoIter& rIter) {
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
        if (_E3 && MR::getRailArg0NoInit(this, &railArg)) {
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
    } else {
        _E1 = 1;
        _DF = 0;
        _DA = 0;
        _D9 = 0;
    }

    switch (arg0) {
    case -1:
    case 0:
        _DF = 0;
        break;
    }

    switch (arg1 & 0xF) {
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

    switch (arg4) {
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

    if (!_D8 && MR::isValidInfo(rIter)) {
        MR::useStageSwitchWriteDead(this, rIter);

        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &BenefitItemObj::appear));
            _E1 = 1;
        }
    }

    if (_D8) {
        _DF = 0;

        makeActorDead();

        return;
    }

    if (_DF) {
        makeActorAppeared();

        if (_E1) {
            MR::invalidateHitSensors(this);
        } else {
            runEfx("Glow");
        }
    } else if (_E1) {
        makeActorDead();
    } else {
        makeActorAppeared();
        runEfx("Glow");
    }

    if (!MR::isDead(this) && MR::isValidInfo(rIter)) {
        MR::useStageSwitchSleep(this, rIter);
    }

    TVec3f rotY;
    MR::getRotatedAxisY(&rotY, mRotation);
    _A4.set(rotY);

    if (!_DC) {
        return;
    }

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
        } else {
            MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
            MR::onCalcShadowOneTime(this, nullptr);
        }
    } else if ((_DC & 0x2) != 0) {
        MR::onCalcShadow(this, nullptr);
    } else {
        MR::onCalcShadowOneTime(this, nullptr);
    }

    if (setClipType) {
        MR::setClippingTypeSphere(this, shadowDist);
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

    switch (_D9) {
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
    MR::startSystemSE("SE_SY_ITEM_APPEAR");

    setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvAppearGround));

    MR::hideModel(this);

    MR::invalidateClipping(this);
}

void BenefitItemObj::appearThrowUp() {
    MR::startSystemSE("SE_SY_ITEM_APPEAR");

    runBck("Appear");

    TVec3f rotY;
    MR::getRotatedAxisY(&rotY, mRotation);
    _A4.set(rotY);

    setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvShoot));

    if (!_DD) {
        mVelocity = rotY * ::cAppearThrowUpSpd;
    } else {
        mVelocity = rotY * ::cAppearThrowUpSpd2;
    }

    MR::invalidateClipping(this);
}

void BenefitItemObj::shoot(const TVec3f& a2, const TVec3f&, bool) {
    mPosition = a2;

    if (!_DD) {
        _D9 = 2;
    } else {
        _D9 = 3;
    }

    appear();
}

void BenefitItemObj::appearThrowUpQuestionBox() {
    appearThrowUp();
}

void BenefitItemObj::control() {
    if (_E6 && MR::tryStartDemoWithoutCinemaFrame(this, "出現")) {
        _E6 = 0;

        MR::startActorCameraTargetSelf(this, mCameraInfo, -1);

        MR::requestMovementOn(this);

        MR::pauseOffCameraDirector();
        _12C = MR::getActorCameraFrames(this, mCameraInfo);
        if (_12C == 0) {
            _12C = 120;
        }
    }

    TVec3f v9(mGravity);
    MR::calcGravityOrZero(this);
    calcAndSetBaseMtxInMovement();

    if (_E4 && _12C != 0) {
        _12C--;

        if (_12C == 0) {
            _E4 = 0;

            MR::endActorCamera(this, mCameraInfo, false, -1);
            MR::endDemo(this, "出現");

            MR::validateClipping(this);
        }
    }

    if (_E2) {
        return;
    }

    if (MR::isBindedGroundDamageFire(this)) {
        runEfx("Vanish");
        kill();
    }

    if (!MR::isNearZero(mVelocity) && MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        runEfx("Vanish");
        kill();
    }
}

void BenefitItemObj::exeWait() {
    if (!_DF) {
        return;
    }

    if (_E1) {
        return;
    }

    if (_DB) {
        doRotateY();
        return;
    }

    if (_E1) {
        return;
    }

    if (_DB & 0x2) {
        doRotateY();
        return;
    }

    if (_DB & 0x1) {
        doRotateY();
    }
}

void BenefitItemObj::doRotateY() {
    if (isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvAppearGround))) {
        _EC = 0.0f;
    } else {
        _EC = MR::repeat(_EC + ::cSpdRotateY, 0.0f, TWO_PI);
    }
}

void BenefitItemObj::exeShoot() {
    //FIXME
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    TVec3f newVel(mVelocity);
    f32 val = MR::vecKillElement(newVel, mGravity, &newVel);

    if (MR::isBindedGround(this)) {
        runBck("Land");

        MR::validateHitSensors(this);

        mVelocity.zero();

        if (_DA == 0) {
            if (MR::isGreaterStep(this, 10)) {
                setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvWait));
            }
        } else {
            setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvEscape));
        }

        return;
    }

    //FIXME: regswap
    val += ::cGravity;

    if (getNerveStep() == 60) {
        MR::validateHitSensors(this);
    }

    if (MR::isBindedWall(this)) {
        newVel -= *MR::getWallNormal(this) * ::cReflectWallX * MR::vecKillElement(newVel, *MR::getWallNormal(this), &newVel);
    }

    newVel += mGravity * val;
    mVelocity.set(newVel);
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

    mVelocity.set(0.0f);
    mPosition.set(mHitSensorActor->mPosition);
}

bool BenefitItemObj::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    return MR::isMsgStarPieceReflect(msg);
}

bool BenefitItemObj::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isDead(this)) {
        return false;
    }

    if (MR::receiveItemShowMsg(msg, pSender, pReceiver)) {
        return true;
    }

    if (MR::receiveItemHideMsg(msg, pSender, pReceiver)) {
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

    if (msg == ACTMES_ITEM_GET) {
        if (mHitSensorActor) {
            return false;
        }

        if (isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvWait)) || isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvShoot)) ||
            isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvAppearGround)) || isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvEscape))) {
            mHitSensorActor = pSender->mHost;

            setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvCatch));

            MR::tryRumblePadWeak(this, WPAD_CHAN0);

            return true;
        }
    }

    return false;
}

void BenefitItemObj::exeAppearGround() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    if (getNerveStep() < 60) {
        return;
    }

    if (getNerveStep() == 60) {
        MR::showModel(this);

        runBck("Appear");

        MR::startSound(this, "SE_OJ_KINOKO_1UP_GROW");
    }

    if (!MR::isBckStopped(this)) {
        return;
    }

    if (!_DA) {
        setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvWait));
    } else {
        setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvPreEscape));
    }

    MR::validateHitSensors(this);
}

void BenefitItemObj::exePreEscape() {
    if (getNerveStep() >= 30) {
        setNerve(GET_NERVE(BenefitItemObj, HostTypeNrvEscape));
    }
}

void BenefitItemObj::initEscape() {
    _B0 = ::sVal;
    _B2 = 0;
    _B4 = 0;
    _B6 = 0;
    _B8 = 0;

    TVec3f rotZ;
    MR::getRotatedAxisZ(&rotZ, mRotation);
    _BC = rotZ * 7.0f;

    MR::startSound(this, "SE_OJ_KINOKO_1UP_RUN_START");
}

void BenefitItemObj::doEscape() {
    MR::startLevelSound(this, "SE_OJ_LV_KINOKO_1UP_RUN");

    bool val = false;
    if (_DA == 2) {
        val = true;
    }

    if (MR::isBindedGround(this)) {
        f32 length = _BC.length();
        MR::vecKillElement(_BC, mGravity, &_BC);
        _BC.setLength(length);

        if (val) {
            TVec3f vec38(mPosition - *MR::getPlayerPos());
            MR::vecKillElement(vec38, mGravity, &vec38);
            MR::normalizeOrZero(&vec38);

            if (_BC.dot(vec38) < 0.0f) {
                _BC += vec38 * 0.01f;
                _BC.x *= 1.1f;
                _BC.y *= 1.1f;
                _BC.z *= 1.1f;
            } else {
                _BC += vec38 * 0.02f;
                _BC.x *= 1.1f;
                _BC.y *= 1.1f;
                _BC.z *= 1.1f;
            }
        }

        if (_BC.length() > 7.0f) {
            _BC.setLength(7.0f);
        }

        if (_E5) {
            _E5 = 0;
            runBck("Land");
        }

        _C8 = mGravity * 5.0f;

        runEfx("Move");
    } else {
        _BC.x *= 0.95f;
        _BC.y *= 0.95f;
        _BC.z *= 0.95f;
        _E5 = 1;

        _C8 += mGravity * 1.0f;

        if (_C8.length() > 20.0f) {
            _C8.setLength(20.0f);
        }

        stopEfx("Move");
    }

    if (!MR::isBindedWall(this)) {
        return;
    }

    if (_E5) {
        return;
    }

    if (!(MR::getWallNormal(this)->dot(mVelocity) < 0.0f)) {
        return;
    }

    _BC += -*MR::getWallNormal(this) * MR::vecKillElement(_BC, *MR::getWallNormal(this), &_BC);
    _C8 += -mGravity * 20.0f;

    _E5 = 1;
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

        _B0--;

        if (_B8 != 0) {
            _B8--;
            MR::hideModel(this);
        }

        if (_B6 != 0 && --_B6 == 0) {
            _B6 = _B2;
            _B8 = _B4;
        }

        if (_B0 == 0x3C) {
            _B2 = 4;
            _B4 = 2;
        } else if (_B0 == 0x78) {
            _B2 = 8;
            _B4 = 3;
        } else if (_B0 == 0xB4) {
            _B2 = 12;
            _B4 = 4;
            _B6 = 12;
            _B8 = 4;
        }

        if (_B0 == 0) {
            runEfx("Vanish");
            makeActorDead();
        }
    }

    if (_E3) {
        MR::moveCoord(this, _E8);
        _BC = MR::getRailPos(this) - mPosition;
    }

    mVelocity = _BC;
    mVelocity += _C8;

    if ((_DB & 0x4) != 0) {
        doRotateY();
    }

    if (!_DC) {
        return;
    }

    if ((_DC & 0x2) != 0) {
        return;
    }

    if ((_DC & 0x1) != 0) {
        MR::onCalcShadowOneTime(this, nullptr);
    } else {
        MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
    }
}

void BenefitItemObj::calcAndSetBaseMtx() {
}

void BenefitItemObj::calcAndSetBaseMtxInMovement() {
    if (isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvWait))) {
        LiveActor::calcAndSetBaseMtx();

        PSMTXCopy(getBaseMtx(), _F0);

        if (mFollowMtx) {
            PSMTXCopy(mFollowMtx, _F0);
            MR::extractMtxTrans(mFollowMtx, &mPosition);
        }

        MR::multMtx(getBaseMtx(), MR::tmpMtxRotYRad(_EC), _F0);

        return;
    }

    if (MR::isBindedGround(this)) {
        _A4 = *MR::getGroundNormal(this);
    } else if (MR::isBindedWall(this)) {
        _A4 = *MR::getWallNormal(this);
    }

    TVec3f stack_8(mVelocity);
    MR::normalizeOrZero(&stack_8);
    if (isNerve(GET_NERVE(BenefitItemObj, HostTypeNrvPreEscape))) {
        MR::vecBlendSphere(_98, stack_8, &stack_8, 0.1f);
        _98 = stack_8;
    }

    if (MR::isNearZero(stack_8)) {
        return;
    }

    if (MR::isSameDirection(_A4, stack_8, 0.01f)) {
        MR::setMtxTrans(getBaseMtx(), mPosition);
        return;
    }

    TPos3f pos;
    MR::makeMtxUpFrontPos(&pos, _A4, stack_8, mPosition);
    MR::blendMtx(_F0, pos, 0.2f, _F0);
    MR::multMtx(getBaseMtx(), MR::tmpMtxRotYRad(_EC), _F0);
    MR::setMtxTrans(getBaseMtx(), mPosition);
}

void BenefitItemObj::setFollowMtx(MtxPtr pMtx) {
    mFollowMtx = pMtx;
    MR::extractMtxTrans(pMtx, &mPosition);
}

void BenefitItemObj::runBck(const char* pBckName) {
    MR::startBck(this, pBckName, nullptr);
}

void BenefitItemObj::runEfx(const char* pEfxName) {
    MR::emitEffect(this, pEfxName);
}

void BenefitItemObj::stopEfx(const char* pEfxName) {
    MR::deleteEffect(this, pEfxName);
}

void MR::setMtxTrans(MtxPtr pMtx, const TVec3f& rVec) {
    setMtxTrans(pMtx, rVec.x, rVec.y, rVec.z);
}
