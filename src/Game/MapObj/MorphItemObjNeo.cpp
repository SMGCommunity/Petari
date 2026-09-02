#include "Game/MapObj/MorphItemObjNeo.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const char* cxNameList[] = {"-----", "PowerupHopper", "PowerUpBee", "PowerupTeresa", "PowerupIce", "PowerupFire", "PowerupFoo", "END"};
    static u16 sUnknown = 60;
    static u16 sUnknown2 = 30;
    static f32 cRotateRadiusPower = 2.0f;
    static f32 cRotateRadiusMini = 30.0f;
};  // namespace

namespace NrvMorphItemObjNeo {
    NEW_NERVE(MorphItemObjNeoNrvWait, MorphItemObjNeo, Wait);
    NEW_NERVE(MorphItemObjNeoNrvAppear, MorphItemObjNeo, Appear);
    NEW_NERVE(MorphItemObjNeoNrvSwitchAppear, MorphItemObjNeo, SwitchAppear);
    NEW_NERVE(MorphItemObjNeoNrvWait2, MorphItemObjNeo, Wait);
    NEW_NERVE(MorphItemObjNeoNrvFly, MorphItemObjNeo, Fly);
    NEW_NERVE(MorphItemObjNeoNrvDemo, MorphItemObjNeo, Wait);
};  // namespace NrvMorphItemObjNeo

void MorphItemObjNeo::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    if (MR::isEqualObjectName(rIter, "MorphItemNeoHopper") || MR::isEqualObjectName(rIter, "Hopper")) {
        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("HopperLuigi");
        } else {
            pArchiveList->addArchive("HopperMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoBee")) {
        pArchiveList->addArchive("BeeMarioWing");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("BeeLuigi");
        } else {
            pArchiveList->addArchive("BeeMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoTeresa")) {
        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("TeresaLuigi");
        } else {
            pArchiveList->addArchive("TeresaMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "MorphItemNeoIce")) {
        pArchiveList->addArchive("IceMarioHandL");
        pArchiveList->addArchive("IceMarioHandR");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("IceLuigi");
        } else {
            pArchiveList->addArchive("IceMario");
        }
    }

    if (MR::isEqualObjectName(rIter, "BenefitItemInvincible")) {
        pArchiveList->addArchive("InvincibleMarioHandL");
        pArchiveList->addArchive("InvincibleMarioHandR");

        if (MR::isPlayerLuigi()) {
            pArchiveList->addArchive("InvincibleLuigi");
            return;
        } else {
            pArchiveList->addArchive("InvincibleMario");
            return;
        }
    }

    s32 arg3 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg3WithInit(rIter, &arg3);
    }

    if (MR::isEqualObjectName(rIter, "Hopper")) {
        return;
    }

    if (arg3 == 0) {
        pArchiveList->addArchive("ItemBubble");
    } else if (arg3 == -1) {
        pArchiveList->addArchive("CrystalBox");
        pArchiveList->addArchive("CrystalBoxBreak");
    }
}

MorphItemObjNeo::MorphItemObjNeo(const char* pName, s32 type) : LiveActor(pName), mMorphType(type), mActorCameraInfo() {
    mAppearMode = 0;
    mAppearFrames = 0;
    _92 = false;
}

void MorphItemObjNeo::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    mSpawnPosition = mPosition;
    mAngle = 0.0f;
    _97 = 0;

    initModelManagerWithAnm(::cxNameList[mMorphType], nullptr, false);

    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_MORPH_ITEM, 4, 80.0f, TVec3f(0.0f, 60.0f, 0.0f));

    MR::initStarPointerTargetAtPos(this, 80.0f, &mPosition, TVec3f(0.0f, 60.0f, 0.0f));

    initBinder(70.0f, 60.0f, 0);
    MR::offBind(this);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    s32 arg1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);

    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);

    s32 arg3 = -1;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);

    s32 arg4 = -1;
    MR::getJMapInfoArg4NoInit(rIter, &arg4);

    s32 arg5 = -1;
    MR::getJMapInfoArg5NoInit(rIter, &arg5);

    s32 arg6 = -1;
    MR::getJMapInfoArg6NoInit(rIter, &arg6);

    s32 arg7 = -1;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);

    PSMTXCopy(getBaseMtx(), mBaseMtx);

    _95 = false;

    switch (arg0) {
    case 1:
        _95 = true;

        break;
    }

    mIsBind = arg1;
    if (mIsBind == 1) {
        MR::onBind(this);
    } else {
        MR::offBind(this);
    }

    mIsFloating = true;
    if (arg2 == 1) {
        mIsFloating = false;
    }

    if (mMorphType == 1) {
        MR::connectToSceneIndirectMapObjStrongLight(this);
    } else {
        MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    }

    mContainerType = 0;
    switch (arg3) {
    case 0:
        mContainerType = 1;

        break;
    case -1:
        mContainerType = 2;

        break;
    }

    if (mContainerType == 2) {
        mContainerModel = new ModelObj("クリスタルボックス", "CrystalBox", mBaseMtx, MR::DrawBufferType_CrystalBox, -2, -2, false);
        mContainerModel->initWithoutIter();
        mContainerModel->appear();
        MR::startBck(mContainerModel, "CrystalBox", nullptr);

        mContainerBreakModel = new ModelObj("クリスタル破壊", "CrystalBoxBreak", mBaseMtx, MR::DrawBufferType_CrystalBox, -2, -2, false);
        mContainerBreakModel->makeActorDead();
    } else if (mContainerType == 1) {
        mContainerModel = new ModelObj("アイテム泡", "ItemBubble", nullptr, -2, -2, -2, false);
        mContainerModel->initWithoutIter();
        mContainerModel->mPosition = mPosition;
        mContainerModel->mRotation = mRotation;
        mContainerModel->appear();

        mContainerBreakModel = nullptr;

        MR::startBck(mContainerModel, "Move", nullptr);
    } else {
        mContainerModel = nullptr;
        mContainerBreakModel = nullptr;
    }

    mShadowType = 0;
    switch (arg4) {
    case -1:
        mShadowType = 1;

        break;
    case 1:
        mShadowType = 2;

        break;
    case 2:
        mShadowType = 3;

        break;
    case 3:
        mShadowType = 4;

        break;
    case 4:
        mShadowType = 5;

        break;
    }

    if (mShadowType != 0) {
        if ((mShadowType & 4) != 0) {
            MR::setClippingTypeSphere(this, 2000.0f);
            mShadowType = 1;
        }

        MR::initShadowVolumeSphere(this, 40.0f);
        MR::setShadowDropLength(this, nullptr, 2000.0f);
        MR::onShadowVolumeCutDropLength(this, nullptr);

        if ((mShadowType & 1) != 0) {
            if ((mShadowType & 2) != 0) {
                MR::onCalcShadowDropPrivateGravity(this, nullptr);
            } else {
                MR::onCalcShadowDropPrivateGravityOneTime(this, nullptr);
            }
        } else {
            if ((mShadowType & 2) != 0) {
                MR::onCalcShadow(this, nullptr);
            } else {
                MR::onCalcShadowOneTime(this, nullptr);
            }
        }
    }

    initEffectKeeper(4, "MorphItemObj", false);
    initSound(4, false);

    mCameraMode = 0;
    if (arg5 > 0) {
        if (arg5 == 1) {
            MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "出現1");
            mCameraMode = 1;
        } else {
            MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "出現1");
            MR::initMultiActorCamera(this, rIter, &mActorCameraInfo, "出現2");

            mCameraMode = 2;
        }

        mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    }

    mRotateMode = arg6;

    MR::getRotatedAxisZ(&mZRotation, mRotation);

    initNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait));

    MR::validateClipping(this);

    makeActorAppeared();

    MR::validateHitSensors(this);

    mIsUseSwitch = false;

    if (MR::isValidInfo(rIter)) {
        MR::useStageSwitchSleep(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);

        if (MR::useStageSwitchReadAppear(this, rIter)) {
            makeActorDead();

            MR::syncStageSwitchAppear(this);

            mIsUseSwitch = true;
        }
    }

    if (mMorphType == 6 && arg7 == 1 && !MR::isOnGameEventFlagRedDriver()) {
        makeActorDead();
    }

    MR::tryRegisterDemoCast(this, rIter);
}

void MorphItemObjNeo::initAfterPlacement() {
}

void MorphItemObjNeo::appear() {
    LiveActor::appear();

    if (mCameraMode != 0) {
        _92 = true;
    }

    mAppearMode = 0;

    if (_95 == true) {
        mAppearMode = ::sUnknown;
    }

    bool b1 = false;
    if (mCameraMode != 0 || _95) {
        b1 = true;

        setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvSwitchAppear));

        MR::invalidateHitSensors(this);
        MR::invalidateClipping(this);
    } else {
        setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));
    }

    _95 = false;
    mIsUseSwitch = false;

    if (mIsBind != 0) {
        MR::onBind(this);
    }

    if (!b1) {
        if (mMorphType == 4) {
            MR::emitEffect(this, "AppearIceFlower");
            MR::startSound(this, "SE_OJ_MORPH_FLOWER_APPEAR");
        }

        if (mMorphType == 5) {
            MR::emitEffect(this, "AppearFireFlower");
            MR::startSound(this, "SE_OJ_MORPH_FLOWER_APPEAR");
        }

        if (mMorphType == 1) {
            MR::emitEffect(this, "AppearHopper");
            MR::startSound(this, "SE_OJ_MORPH_ITEM_APPEAR");
        }

        if (mMorphType == 6) {
            MR::emitEffect(this, "AppearFoo");
            MR::startSound(this, "SE_OJ_MORPH_FLOWER_APPEAR");
        }
    }

    switch (mMorphType) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        MR::startBck(this, "Appear", nullptr);

        break;
    }
}

void MorphItemObjNeo::control() {
    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvFly))) {
        return;
    }

    if (_92 && MR::tryStartDemoWithoutCinemaFrame(this, "出現")) {
        _92 = false;

        TPos3f* pMtx = &mCameraTargetMtx->mMatrix;
        pMtx->set(getBaseMtx());
        MR::startMultiActorCameraTargetSelf(this, mActorCameraInfo, "出現1", -1);
        mAppearFrames = MR::getMultiActorCameraFrames(this, mActorCameraInfo, "出現1");

        if (mAppearFrames != 0) {
            mAppearFrames--;
        }

        MR::requestMovementOn(this);

        MR::pauseOffCameraDirector();
    }

    // TODO
    if (!isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvAppear)) && !isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvSwitchAppear))) {
        if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait))) {
            if (mContainerType == 0 && !mIsUseSwitch) {
                if ((mRotateMode & 1) != 0) {
                    mAngle += 2.0f;
                }
            } else if ((mRotateMode & 2) != 0) {
                mAngle += 2.0f;
            }
        } else if ((mRotateMode & 1) != 0) {
            mAngle += 2.0f;
        }
    }

    mAngle = MR::repeatDegree(mAngle);
}

void MorphItemObjNeo::calcAndSetBaseMtx() {
    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait))) {
        if (mContainerType == 2) {
            MtxPtr pMtx = MR::getJointMtx(mContainerModel, "CrystalBox");
            PSMTXCopy(pMtx, getBaseMtx());

            TVec3f yDir;
            reinterpret_cast< TPos3f* >(getBaseMtx())->getYDir(yDir);
            MR::addTransMtx(getBaseMtx(), yDir.scaleInline(-60.0f));

            pMtx = MR::tmpMtxScale(0.8f, 0.8f, 0.8f);
            PSMTXConcat(getBaseMtx(), pMtx, getBaseMtx());
        } else if (mContainerType == 1) {
            MtxPtr pMtx = mContainerModel->getBaseMtx();
            PSMTXCopy(pMtx, getBaseMtx());

            TVec3f yDir;
            reinterpret_cast< TPos3f* >(getBaseMtx())->getYDir(yDir);
            MR::addTransMtx(getBaseMtx(), yDir * -50.0f);

            pMtx = MR::tmpMtxScale(0.8f, 0.8f, 0.8f);
            PSMTXConcat(getBaseMtx(), pMtx, getBaseMtx());
        } else {
            LiveActor::calcAndSetBaseMtx();

            PSMTXConcat(getBaseMtx(), MR::tmpMtxRotYDeg(mAngle), getBaseMtx());
        }
    } else {
        LiveActor::calcAndSetBaseMtx();

        PSMTXConcat(getBaseMtx(), MR::tmpMtxRotYDeg(mAngle), getBaseMtx());

        if (mAppearMode != 0) {
            TPos3f rotMtx;
            PSMTXRotAxisRad(rotMtx, mVelocity, mAppearMode * (TWO_PI / ::sUnknown2));

            TVec3f rotVec;
            PSMTXMultVec(rotMtx, mZRotation, rotVec);

            rotVec.setLength(mAppearMode * ::cRotateRadiusPower + ::cRotateRadiusMini);
            MR::addTransMtx(getBaseMtx(), rotVec);
        }
    }
}

void MorphItemObjNeo::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mContainerType != 2) {
        return;
    }

    if (mContainerModel == 0) {
        return;
    }

    if (MR::isDead(mContainerModel)) {
        return;
    }

    MR::sendMsgPush(pReceiver, pSender);
}

bool MorphItemObjNeo::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvAppear))) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if ((MR::isMsgPlayerHitAll(msg) || MR::isMsgPlayerHipDrop(msg)) && isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait))) {
        if (mContainerModel != nullptr) {
            mContainerModel->kill();

            if (mContainerBreakModel != nullptr) {
                mContainerBreakModel->appear();

                MR::startBck(mContainerBreakModel, "Break", nullptr);

                MR::startSound(this, "SE_OJ_CRYSTAL_CAGE_S_BREAK");

                setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvAppear));
            } else {
                MR::startSound(this, "SE_OJ_ITEM_BUBBLE_BREAK");

                setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));
            }

            return true;
        } else {
            return false;
        }
    }

    return false;
}

bool MorphItemObjNeo::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isDead(this)) {
        return false;
    }

    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvAppear))) {
        return false;
    }

    if (MR::isMsgItemGet(msg)) {
        if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait))) {
            if (mContainerType == 1) {
                MR::startSound(this, "SE_OJ_ITEM_BUBBLE_BREAK");

                if (mContainerModel != nullptr) {
                    mContainerModel->kill();
                }

                setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));

                if (mIsBind != 0) {
                    MR::onBind(this);
                }
            }

            if (mContainerType != 0) {
                return false;
            }
        }

        if (MR::isPlayerElementModeInvincible()) {
            if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2)) ||
                ((isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait)) && mContainerType == 0))) {
                setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvFly));

                mVelocity = MR::getSensorPos(pReceiver) - MR::getSensorPos(pSender);
                mVelocity.setLength(50.0f);

                MR::calcGravity(this);

                mVelocity += -mGravity * 30.0f;
            }

            return false;
        }

        // FIXME
        bool isInvalid = false;
        switch (mMorphType) {
        case 0:
            MR::startSystemSE("SE_SY_POWER_UP");

            break;
        case 1:
            if (MR::isPlayerElementModeHopper()) {
                isInvalid = true;
            } else {
                MR::startSystemSE("SE_SY_MORPH");
            }

            break;
        case 2:
            if (MR::isPlayerElementModeBee()) {
                isInvalid = true;
            } else {
                MR::startSystemSE("SE_SY_MORPH");
            }

            break;
        case 3:
            if (MR::isPlayerElementModeTeresa()) {
                isInvalid = true;
            } else {
                MR::startSystemSE("SE_SY_MORPH");
            }

            break;
        case 4:
        case 5:
        case 6:
        case 7:
            MR::startSystemSE("SE_SY_POWER_UP_2");

            break;
        }

        if (isInvalid) {
            MR::startSound(this, "SE_OJ_MORPH_ITEM_INVALID");
        }

        MR::changePlayerItemStatus(mMorphType);

        mAngle = 0.0f;

        calcAndSetBaseMtx();

        makeActorDead();

        return true;
    }

    return false;
}

void MorphItemObjNeo::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    makeActorDead();

    mPosition = mSpawnPosition;
    mVelocity.zero();

    _97 = 0;

    mScale.set(0.0f);

    makeActorAppeared();

    MR::emitEffect(this, "ItemRevival");

    MR::startSound(this, "SE_OJ_MORPH_ITEM_RECOVER");

    if (mContainerModel != nullptr) {
        mContainerModel->appear();
    }

    setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait));

    if (mIsBind != 1) {
        MR::offBind(this);
    }
}

void MorphItemObjNeo::makeActorDead() {
    if (mContainerModel != nullptr) {
        mContainerModel->makeActorDead();
    }

    LiveActor::makeActorDead();
}

void MorphItemObjNeo::glowEffect() {
    switch (mMorphType) {
    case 1:
        MR::emitEffect(this, "GlowHopper");

        break;
    case 2:
        MR::emitEffect(this, "GlowBee");

        break;
    case 3:
        MR::emitEffect(this, "GlowTeresa");

        break;
    case 4:
        MR::emitEffect(this, "GlowIce");

        break;
    case 5:
        MR::emitEffect(this, "GlowFire");

        break;
    case 6:
        MR::emitEffect(this, "GlowFoo");

        break;
    }
}

HitSensor* getBodySensor(LiveActor* pActor) {
    return pActor->getSensor("body");
}

void MorphItemObjNeo::exeWait() {
    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvDemo))) {
        mAngle = 0.0f;
        return;
    }

    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait))) {
        if (mContainerType == 2) {
            ::getBodySensor(this)->mRadius = 130.0f;
            MR::setSensorOffset(this, "body", TVec3f(0.0f, 190.0f, 0.0f));
        } else if (mContainerType == 1) {
            ::getBodySensor(this)->mRadius = 110.0f;
            MR::setSensorOffset(this, "body", TVec3f(0.0f, 0.0f, 0.0f));
        } else {
            glowEffect();
        }
    } else {
        ::getBodySensor(this)->mRadius = 80.0f;
        MR::setSensorOffset(this, "body", TVec3f(0.0f, 60.0f, 0.0f));

        glowEffect();
    }

    if (isDemo()) {
        return;
    }

    if (isNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2)) && mIsFloating && _97 <= 3) {
        if (MR::isBindedGround(this) && mVelocity.dot(mGravity) > 0.0f) {
            if (mVelocity.length() < 2.0f) {
                mVelocity.zero();

                return;
            }

            if (_97 < 3) {
                mVelocity = mVelocity * -0.5f;
            } else {
                mVelocity.zero();
            }

            _97++;

            return;
        }

        MR::calcGravity(this);
        mVelocity += mGravity * 0.75f;

        return;
    }

    mVelocity.zero();

    if (MR::isInvalidClipping(this)) {
        MR::validateClipping(this);
    }
}

void MorphItemObjNeo::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::calcGravity(this);

        mVelocity = mGravity * -25.0f;

        if (mIsBind != 0) {
            MR::onBind(this);
        }
    }

    if (mContainerBreakModel != nullptr) {
        if (MR::isBckStopped(mContainerBreakModel) && getNerveStep() > 30) {
            setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));
        }

        mVelocity += mGravity * 0.75f;
    } else {
        setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));
    }
}

void MorphItemObjNeo::exeSwitchAppear() {
    if (MR::isFirstStep(this)) {
        MR::calcGravity(this);

        if (mAppearMode != 0) {
            MR::startBck(this, "Appear", nullptr);

            if (mMorphType == 3) {
                MR::emitEffect(this, "AppearTeresa");
            } else if (mMorphType == 1) {
                MR::emitEffect(this, "AppearHopper");
            } else {
                MR::emitEffect(this, "AppearBee");
            }

            if (mMorphType != 4 && mMorphType != 5) {
                MR::startSound(this, "SE_OJ_MORPH_ITEM_APPEAR");
            }
        } else {
            MR::hideModel(this);
        }

        MR::invalidateHitSensors(this);

        mVelocity = mGravity * -10.0f;
    }

    if (getNerveStep() == 60) {
        MR::showModel(this);

        if (mMorphType == 4) {
            MR::startBck(this, "Appear", nullptr);
            MR::emitEffect(this, "AppearIceFlower");
            MR::startSound(this, "SE_OJ_MORPH_FLOWER_APPEAR");
        }

        if (mMorphType == 5) {
            MR::startBck(this, "Appear", nullptr);
            MR::emitEffect(this, "AppearFireFlower");
            MR::startSound(this, "SE_OJ_MORPH_FLOWER_APPEAR");
        }
    }

    if (mCameraMode != 0 && mAppearFrames != 0 && --mAppearFrames == 0) {
        MR::endMultiActorCamera(this, mActorCameraInfo, "出現1", false, -1);
        MR::endMultiActorCamera(this, mActorCameraInfo, "出現2", false, -1);

        mCameraMode--;

        if (mCameraMode == 0) {
            MR::endDemo(this, "出現");
            MR::validateClipping(this);
        } else {
            TPos3f* pMtx = &mCameraTargetMtx->mMatrix;
            pMtx->set(getBaseMtx());

            MR::startMultiActorCameraTargetOther(this, mActorCameraInfo, "出現2", CameraTargetArg(nullptr, mCameraTargetMtx, nullptr, nullptr), -1);

            mAppearFrames = MR::getMultiActorCameraFrames(this, mActorCameraInfo, "出現2");
        }
    }

    if (mAppearMode != 0) {
        mAppearMode--;
    }

    if (mAppearMode == 0) {
        MR::validateHitSensors(this);
        mVelocity.zero();
    }

    if (mCameraMode != 0) {
        return;
    }

    if (mAppearMode != 0) {
        return;
    }

    setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait2));
}

void MorphItemObjNeo::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    // FIXME
    mVelocity.set(mVelocity.x * 0.999f, mVelocity.y * 0.999f, mVelocity.z * 0.999f);

    MR::calcGravity(this);

    mVelocity += mGravity * 0.2f;

    if (getNerveStep() == 300) {
        kill();
        setNerve(GET_NERVE(MorphItemObjNeo, MorphItemObjNeoNrvWait));
    }
}

bool MorphItemObjNeo::isDemo() const {
    bool ret = false;
    if (mAppearMode != 0 || mAppearFrames != 0) {
        ret = true;
    }

    return ret;
}
