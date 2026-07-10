#include "Game/MapObj/PowerStar.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/PowerStarAppearPoint.hpp"
#include "Game/MapObj/PowerStarHolder.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LightUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

const GXColor lightColor[] = {
    {0x96, 0x96, 0x32, 0}, {0x32, 0x32, 0x96, 0}, {0x32, 0x96, 0x32, 0}, {0x96, 0x32, 0x32, 0}, {0x64, 0x64, 0x64, 0},
};

namespace {
    const char* const cAppearDemoName = "パワースター出現";
    const char* const cStageClearAnimNameKoopaVs3 = "GrandStarGetKoopaVs3";

    void setupColor(LiveActor* pActor, bool useFrame, int frame) {
        MR::startBtp(pActor, "PowerStar");
        MR::setBtpFrameAndStop(pActor, useFrame ? 0.0f : frame);
        MR::startBva(pActor, "PowerStar");
        MR::setBvaFrameAndStop(pActor, useFrame ? 1.0f : 0.0f);
    }

    void setupColorGrandStar(LiveActor* pActor, bool dontStartRecover) {
        if (dontStartRecover) {
            MR::showMaterial(pActor, "GrandStarEmpty");
            MR::hideMaterial(pActor, "FooMat");
        } else {
            MR::showMaterial(pActor, "FooMat");
            MR::hideMaterial(pActor, "GrandStarEmpty");
            MR::startBrk(pActor, "Recover");
            MR::setBrkFrameEndAndStop(pActor);
        }
    }

    void requestPointLight(const LiveActor* pActor, int a2) {
        TVec3f jointPos;

        MR::copyJointPos(pActor, "PowerStar", &jointPos);
        MR::requestPointLight(pActor, TVec3f(jointPos), lightColor[a2], 1.0f, -1);
    }
};  // namespace

namespace NrvPowerStar {
    NEW_NERVE(PowerStarNrvWaitStartAppear, PowerStar, WaitStartAppear);
    NEW_NERVE(PowerStarNrvAppearDemoRise, PowerStar, AppearDemoRise);
    NEW_NERVE(PowerStarNrvAppearDemoMove, PowerStar, AppearDemoMove);
    NEW_NERVE(PowerStarNrvAppearDemoKoopa, PowerStar, AppearDemoKoopa);
    NEW_NERVE(PowerStarNrvWait, PowerStar, Wait);
    NEW_NERVE(PowerStarNrvWeak, PowerStar, Weak);
    NEW_NERVE(PowerStarNrvWeakNoRotate, PowerStar, WeakNoRotate);
    NEW_NERVE(PowerStarNrvWeakToWait, PowerStar, WeakToWait);
    NEW_NERVE(PowerStarNrvStageClearDemo, PowerStar, StageClearDemo);
};  // namespace NrvPowerStar

PowerStar::PowerStar(const char* pName)
    : LiveActor(pName), mPowerStarId(-1), mIsInDemo(false), mAppearPosition(gZeroVec), mInitRotation(gZeroVec), mInitPosition(gZeroVec),
      mPowerStarModelObj(nullptr), _11C(0), mColorFrame(0), mIsGrandStar(false), _125(false), _126(false), _127(false), mCameraInfo(nullptr),
      mCameraActor(nullptr), mLuigiNPC(nullptr), _164(false) {
    _B8.identity();
    mBaseMtx.identity();
    _134.identity();
}

void PowerStar::init(const JMapInfoIter& rIter) {
    mIsGrandStar = MR::isEqualObjectName(rIter, "GrandStar");

    initMapToolInfo(rIter);
    initModel();

    if (mIsGrandStar) {
        MR::connectToSceneIndirectMapObj(this);
        initSensorGrandStar();
    } else {
        MR::connectToSceneNoSilhouettedMapObj(this);
        initHitSensor(1);
        MR::addHitSensor(this, "body", ATYPE_POWER_STAR_BIND, 8, 75.0f, TVec3f(0.0f, 0.0f, 0.0f));
    }

    initEffectKeeper(0, nullptr, false);

    if (MR::isGalaxyGhostCometAppearInCurrentStage()) {
        MR::invalidateClipping(this);
    } else {
        MR::setClippingTypeSphere(this, 5000.0f);
    }

    initShadow(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    ModelObj* power_star_mdl = mPowerStarModelObj;
    ActorCameraInfo* cam_info = mCameraInfo;
    MR::initAnimCamera(power_star_mdl, cam_info, mIsGrandStar ? "GrandStarGet" : "PowerStarGet");

    if (mIsGrandStar) {
        if (MR::isStageKoopaVs()) {
            bool isStageV3 = MR::isStageKoopaVs3();
            power_star_mdl = mPowerStarModelObj;
            cam_info = mCameraInfo;
            MR::initAnimCamera(power_star_mdl, cam_info, isStageV3 ? "DemoKoopaGrandStarVs3" : "DemoKoopaGrandStar");

            if (MR::isStageKoopaVs3()) {
                MR::initAnimCamera(mPowerStarModelObj, mCameraInfo, ::cStageClearAnimNameKoopaVs3);
            }
        }
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvPowerStar::PowerStarNrvWeakNoRotate::sInstance, "ミニ太陽消失");
        MR::registerDemoActionNerve(this, &NrvPowerStar::PowerStarNrvWeakToWait::sInstance, "グランドスター復活");
        MR::registerDemoSimpleCastAll(this);
    }

    initSound(4, false);
    initNerve(&NrvPowerStar::PowerStarNrvWait::sInstance);

    if (mPowerStarId == -1) {
        if (mIsInDemo) {
            makeActorDead();
        } else {
            makeActorAppeared();
        }
    } else {
        makeActorAppeared();
    }
}

void PowerStar::initAfterPlacement() {
}

void PowerStar::appear() {
    LiveActor::appear();
    MR::invalidateClipping(this);
    requestAppearOrWait();
}

void PowerStar::makeActorAppeared() {
    mInitPosition.set< f32 >(mPosition);
    initPosture();
    LiveActor::makeActorAppeared();
}

void PowerStar::requestAppear() {
    makeActorAppeared();
    MR::invalidateClipping(this);
    MR::hideModel(this);
    MR::invalidateHitSensors(this);
    setNerve(&NrvPowerStar::PowerStarNrvWaitStartAppear::sInstance);

    if (MR::isStageKoopaVs()) {
        MR::requestStartDemoMarioPuppetableWithoutCinemaFrame(this, ::cAppearDemoName, &NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance,
                                                              nullptr);
    } else {
        MR::requestStartDemoWithoutCinemaFrame(this, ::cAppearDemoName, &NrvPowerStar::PowerStarNrvAppearDemoRise::sInstance, nullptr);
    }
}

void PowerStar::setDemoAppearPos(const TVec3f& rVec) {
    bool isGroupExist = MR::getGroupFromArray(this);

    if (isGroupExist) {
        mCameraActor = getNearestAppearPoint(rVec);
        mAppearPosition.set< f32 >(mCameraActor->mPosition);
    }

    mPosition.set< f32 >(rVec);
}

bool PowerStar::isEndAppearDemo() const {
    return isNerve(&NrvPowerStar::PowerStarNrvWait::sInstance) || isNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance);
}

void PowerStar::offAppearDemo() {
    mIsInDemo = false;
}

void PowerStar::setupColor(LiveActor* pActor, const NameObj* a2, s32 a3) {
    bool hasPowerStar;
    s32 val = 0;
    hasPowerStar = MR::hasPowerStarInCurrentStageWithDeclarer(a2->mName, a3);

    if (MR::isPowerStarGreenInCurrentStageWithDeclarer(a2->mName, a3)) {
        val = 2;
    } else if (MR::isPowerStarRedInCurrentStageWithDeclarer(a2->mName, a3)) {
        val = 3;
    }

    if (MR::isGrandStarInCurrentStageWithDeclarer(a2->mName, a3)) {
        ::setupColorGrandStar(pActor, hasPowerStar);
    } else {
        ::setupColor(pActor, hasPowerStar, val);
    }
}

void PowerStar::setupColorAtResultSequence(LiveActor* pPowerStar, bool a2) {
    bool hasStarAtRes;
    s32 val = 0;
    hasStarAtRes = MR::hasPowerStarAtResultSequence();

    if (MR::isPowerStarGreenAtResultSequence()) {
        val = 2;
    } else if (MR::isPowerStarRedAtResultSequence()) {
        val = 3;
    }

    if (a2) {
        ::setupColorGrandStar(pPowerStar, hasStarAtRes);
    } else {
        ::setupColor(pPowerStar, hasStarAtRes, val);
    }
}

void PowerStar::requestPointLight(const LiveActor* pActor, const NameObj* a2, s32 a3) {
    s32 colorIdx = 0;

    if (MR::hasPowerStarInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 4;
    } else if (MR::isPowerStarGreenInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 2;
    } else if (MR::isPowerStarRedInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 3;
    }

    ::requestPointLight(pActor, colorIdx);
}

void PowerStar::requestPointLightAtResultSequence(const LiveActor* pActor) {
    s32 val = 0;

    if (MR::hasPowerStarAtResultSequence()) {
        val = 4;
    } else if (MR::isPowerStarGreenAtResultSequence()) {
        val = 2;
    } else if (MR::isPowerStarRedAtResultSequence()) {
        val = 3;
    }

    ::requestPointLight(pActor, val);
}

f32 PowerStar::getPowerStarWaitRotateSpeed() {
    return 3.0f;
}

s32 PowerStar::getBtpFrameCurrentStage(s32 a1) {
    if (MR::isPowerStarGreenInCurrentStage(a1)) {
        return 2;
    }

    if (MR::isPowerStarRedInCurrentStage(a1)) {
        return 3;
    }

    return 0;
}

bool PowerStar::isCurrentStageKoopaVs3() {
    return MR::isStageKoopaVs3();
}

void PowerStar::initShadowPowerStar(LiveActor* pActor, bool a2) {
    MR::initShadowFromCSV(pActor, "Shadow");
    MR::invalidateShadowAll(pActor);
    MR::validateShadowGroup(pActor, a2 ? "円柱" : "通常");
}

void PowerStar::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (MR::isPowerStarGetDemoWithLuigiCurrentGalaxy()) {
        pCollector->addArchive("LuigiNPC");
    }
}

void PowerStar::control() {
    if (!isNerve(&NrvPowerStar::PowerStarNrvWaitStartAppear::sInstance)) {
        TVec3f jointPos;

        if (isNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance)) {
            MR::copyJointPos(mPowerStarModelObj, "PowerStar", &jointPos);
        } else {
            MR::copyJointPos(this, "PowerStar", &jointPos);
        }

        bool cond = (isNerve(&NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance) && MR::isStageKoopaVs3()) ? true : false;
        s32 val = cond ? 120 : -1;

        MR::requestPointLight(this, TVec3f(jointPos), lightColor[mColorFrame], 1.0f, val);
    }
}

void PowerStar::calcAndSetBaseMtx() {
    TPos3f mtx2;
    TPos3f mtx;
    // FIXME
    mtx.setRotateInlineZeroTrans(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(mRotation.y));

    mtx2.concat(_B8, mtx);
    mtx2.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx2);
}

bool PowerStar::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    return MR::isMsgJetTurtleAttack(msg);
}

bool PowerStar::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    switch (msg) {
    case ACTMES_IS_RUSH_TAKEOVER:
        return true;
    case ACTMES_AUTORUSH_BEGIN:
        if (isNerve(&NrvPowerStar::PowerStarNrvWait::sInstance)) {
            MR::startSystemSE("SE_SY_STAR_GET");
            MR::stopSoundPlayer("SE_PV_BURN_RUN", 0);
            MR::stopSoundPlayer("SE_PV_NEEDLE_DAMAGE_RUN", 0);
            MR::makeMtxTR((MtxPtr)&mBaseMtx, *MR::getPlayerPos(), *MR::getPlayerRotate());
            setNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance);
            return true;
        }
        break;
    case ACTMES_UPDATE_BASEMTX:
        MR::setPlayerBaseMtx((MtxPtr)&mBaseMtx);
        return true;
    }

    return false;
}

void PowerStar::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mPowerStarId);
    MR::getJMapInfoArg1NoInit(rIter, &_125);
    MR::getJMapInfoArg2NoInit(rIter, &_126);
    MR::getJMapInfoArg4NoInit(rIter, &_127);
    mAppearPosition.set< f32 >(mPosition);
    mInitRotation.set< f32 >(mRotation);
    MR::registerPowerStar(this, mPowerStarId);
    MR::joinToGroupArray(this, rIter, "パワースター出現ポイントグループ", 0x10);

    if (mPowerStarId > 0) {
        if (!MR::hasPowerStarInCurrentStage(mPowerStarId)) {
            u32 frame;

            if (mIsGrandStar) {
                frame = 0;
            } else {
                frame = getBtpFrameCurrentStage(mPowerStarId);
            }

            mColorFrame = frame;
        } else {
            mColorFrame = 4;
        }
    }
}

void PowerStar::initModel() {
    initModelManagerWithAnm(isGrandStar() ? "GrandStar" : "PowerStar", 0, false);

    if (mIsGrandStar) {
        ::setupColorGrandStar(this, mColorFrame_v == 4);
    } else {
        ::setupColor(this, mColorFrame_v == 4, mColorFrame_v);
    }

    if (mIsGrandStar) {
        MtxPtr pMtx = (MtxPtr)&mBaseMtx;
        const char* pName = "グランドスターデモモデル";

        mPowerStarModelObj = MR::createModelObjIndirectMapObj(pName, mIsGrandStar ? "GrandStar" : "PowerStar", pMtx);

        ::setupColorGrandStar(mPowerStarModelObj, mColorFrame_v == 4);
    } else {
        MtxPtr pMtx = (MtxPtr)&mBaseMtx;
        const char* pName = "パワースターデモモデル";

        mPowerStarModelObj = MR::createModelObjNoSilhouettedMapObj(pName, mIsGrandStar ? "GrandStar" : "PowerStar", pMtx);

        ::setupColor(mPowerStarModelObj, mColorFrame_v == 4, mColorFrame_v);
    }

    MR::invalidateClipping(mPowerStarModelObj);
    mPowerStarModelObj->makeActorDead();

    if (MR::isPowerStarGetDemoWithLuigiCurrentGalaxy()) {
        mLuigiNPC = MR::createModelObjNpc("ルイージデモモデル", "LuigiNPC", (MtxPtr)&mBaseMtx);

        MR::initLightCtrl(mLuigiNPC);
        MR::invalidateClipping(mLuigiNPC);
        mLuigiNPC->makeActorDead();
    }

    MR::tryCreateMirrorActor(this, isGrandStar() ? "GrandStar" : "PowerStar");
}

void PowerStar::initSensorGrandStar() {
    initHitSensor(6);
    TVec3f body1Offs = TVec3f(0.0f, 100.0f, 0.0f);
    const char* body1Name = "body1";
    MR::addHitSensorMtx(this, body1Name, ATYPE_POWER_STAR_BIND, 8, 60.0f, getBaseMtx(), body1Offs);
    TVec3f body2Offs = TVec3f(-95.0f, 35.0f, 0.0f);
    const char* body2Name = "body2";
    MR::addHitSensorMtx(this, body2Name, ATYPE_POWER_STAR_BIND, 8, 60.0f, getBaseMtx(), body2Offs);
    TVec3f body3Offs = TVec3f(-55.0f, -75.0f, 0.0f);
    const char* body3Name = "body3";
    MR::addHitSensorMtx(this, body3Name, ATYPE_POWER_STAR_BIND, 8, 60.0f, getBaseMtx(), body3Offs);
    TVec3f body4Offs = TVec3f(55.0f, -75.0f, 0.0f);
    const char* body4Name = "body4";
    MR::addHitSensorMtx(this, body4Name, ATYPE_POWER_STAR_BIND, 8, 60.0f, getBaseMtx(), body4Offs);
    TVec3f body5Offs = TVec3f(95.0f, 35.0f, 0.0f);
    const char* body5Name = "body5";
    MR::addHitSensorMtx(this, body5Name, ATYPE_POWER_STAR_BIND, 8, 60.0f, getBaseMtx(), body5Offs);
    TVec3f body6Offs = TVec3f(0.0f, 0.0f, 0.0f);
    const char* body6Name = "body6";
    MR::addHitSensorMtx(this, body6Name, ATYPE_POWER_STAR_BIND, 8, 80.0f, getBaseMtx(), body6Offs);
}

void PowerStar::initShadow(const JMapInfoIter& rIter) {
    if (!mIsGrandStar) {
        bool arg3 = false;
        MR::getJMapInfoArg3NoInit(rIter, &arg3);
        bool thing = arg3;
        MR::initShadowFromCSV(this, "Shadow");
        MR::invalidateShadowAll(this);
        MR::validateShadowGroup(this, thing ? "円柱" : "通常");
    } else {
        MR::initShadowFromCSV(this, "Shadow");
    }
}

void PowerStar::initPosture() {
    MR::calcGravity(this, mAppearPosition);
    TPos3f mtx;
    MR::makeMtxRotate(mtx, mInitRotation);

    if (!_126) {
        TVec3f yDir;
        mtx.getYDir(yDir);

        TVec3f negGravity = mGravity.negateInline();
        if (!MR::isSameDirection(negGravity, yDir, 0.01f)) {
            MR::makeMtxUpFront(&_B8, negGravity, yDir);
        } else {
            MR::makeMtxUpNoSupport(&_B8, negGravity);
        }
    } else {
        _B8.setInline(&mtx);
    }

    _B8.zeroTrans();
    mRotation.y = 0.0f;
}

void PowerStar::endAppearDemo() {
    if (!_11C) {
        MR::endDemo(this, ::cAppearDemoName);
    }

    MR::moveVolumeStageBGM(1.0f, 60);
    MR::moveVolumeSubBGM(1.0f, 60);
    setNerve(&NrvPowerStar::PowerStarNrvWait::sInstance);
}

PowerStarAppearPoint* PowerStar::getNearestAppearPoint(const TVec3f& rPos) const {
    LiveActorGroup* pGroup = MR::getGroupFromArray(this);
    f32 minDist = FLOAT_MAX;
    PowerStarAppearPoint* pNearestAppearPoint = nullptr;

    for (int i = 0; i < pGroup->mObjectCount; i++) {
        LiveActor* pActor = pGroup->getActor(i);

        if (pActor != this) {
            f32 dist = rPos.distance(pActor->mPosition);

            if (dist < minDist) {
                pNearestAppearPoint = static_cast< PowerStarAppearPoint* >(pActor);
                minDist = dist;
            }
        }
    }

    return pNearestAppearPoint;
}

LiveActor* PowerStar::getAppearCameraActor() {
    if (mCameraActor == nullptr) {
        return this;
    }

    return mCameraActor;
}

ActorCameraInfo* PowerStar::getAppearCameraInfo() const {
    if (mCameraActor == nullptr) {
        return mCameraInfo;
    }

    return mCameraActor->mCameraInfo;
}

void PowerStar::requestAppearOrWait() {
    if (MR::isDemoCast(this, 0)) {
        setNerve(&NrvPowerStar::PowerStarNrvWeak::sInstance);
    } else if (mIsInDemo) {
        if (_11C) {
            MR::invalidateClipping(this);
            MR::requestMovementOn(this);

            if (MR::isStageKoopaVs()) {
                setNerve(&NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance);
            } else {
                setNerve(&NrvPowerStar::PowerStarNrvAppearDemoRise::sInstance);
            }
        } else {
            requestAppear();
        }
    } else {
        setNerve(&NrvPowerStar::PowerStarNrvWait::sInstance);
    }
}

void PowerStar::calcAppearDemoRiseTrans(TVec3f* pOutTrans, f32 a2) const {
    if (_125) {
        MR::calcGravityVector(this, mInitPosition, pOutTrans, 0, 0);
        pOutTrans->x *= -a2;
        pOutTrans->y = (pOutTrans->y * -a2);
        pOutTrans->z = (pOutTrans->z * -a2);
    } else {
        TVec3f vec;
        vec.y = a2;
        vec.x = 0.0f;
        vec.z = 0.0f;
        _B8.mult(vec, *pOutTrans);
    }

    pOutTrans->add(mInitPosition);
}

void PowerStar::processWait(f32 val) {
    mRotation.y = MR::repeatDegree(mRotation.y);

    if (mIsGrandStar) {
        if (MR::changeShowModelFlagSyncNearClipping(this, 250.0f)) {
            MR::onDrawEffect(this);
        } else {
            MR::offDrawEffect(this);
        }
    }

    if (MR::isGalaxyGhostCometAppearInCurrentStage()) {
        if (MR::isJudgedToClipFrustum100m(mPosition, 5000.0f)) {
            MR::hideModelAndOnCalcAnimIfShown(this);
            MR::forceDeleteEffect(this, "Light");
        } else {
            MR::showModelIfHidden(this);
            MR::emitEffect(this, "Light");
        }
    }

    if (!_164) {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST_W");
        } else {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST");
        }
    }
}

void PowerStar::exeWaitStartAppear() {
}

void PowerStar::exeAppearDemoRise() {
    if (MR::isFirstStep(this)) {
        MR::showModelIfHidden(this);
        MR::moveVolumeStageBGM(0.0f, 5);
        MR::moveVolumeSubBGM(0.0f, 5);

        MR::startActorCameraTargetOther(getAppearCameraActor(), getAppearCameraInfo(), CameraTargetArg(this), 0);
        mPosition.set(mInitPosition);
        MR::emitEffect(this, "Light");
    }

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST_W");
    } else {
        MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST");
    }

    f32 easeOut = MR::calcNerveEaseOutValue(this, 60, 0.0f, 300.0f);
    calcAppearDemoRiseTrans(&mPosition, easeOut);
    mRotation.y = MR::repeatDegree(mRotation.y + 10.0f);

    if (MR::isStep(this, 80)) {
        setNerve(&NrvPowerStar::PowerStarNrvAppearDemoMove::sInstance);
    }
}

void PowerStar::exeAppearDemoMove() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_SY_POW_STAR_APPEAR");
    }

    if (MR::isLessStep(this, 120)) {
        TVec3f vec;
        f32 step = getNerveStep() / 120.0f;
        calcAppearDemoRiseTrans(&vec, 300.0f);
        vec.lerp(mAppearPosition, mPosition, step);

        TVec3f vec2;
        MR::vecKillElement(vec, mGravity, &vec2);

        TVec3f vec3;
        MR::vecKillElement(mAppearPosition, mGravity, &vec3);

        f32 val = vec2.distance(vec3) * 0.5f;
        f32 val2 = MR::clamp(val, 300.0f, 5000.0f);

        TVec3f zVec(0.0f, 0.0f, 0.0f);
        zVec.y = MR::sin(step * PI) * val2;
        _B8.mult(zVec, zVec);
        mPosition.add(zVec);
    } else {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST_W");
        } else {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST");
        }
    }

    if (MR::isStep(this, 120)) {
        mPosition.set(mAppearPosition);

        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startSound(this, "SE_OJ_POW_STAR_MOVE_END_W");
        } else {
            MR::startSound(this, "SE_OJ_POW_STAR_MOVE_END");
        }
    }

    mRotation.y = MR::repeatDegree(
        mRotation.y + (MR::isLessStep(this, 120) ? 20.0f : MR::getEaseOutValue(getNerveStep() - 120, 20.0f, mIsGrandStar ? 2.0f : 3.0f, 90.0f)));

    if (MR::isStep(this, 210)) {
        MR::resetCameraLocalOffset();

        MR::endActorCamera(getAppearCameraActor(), getAppearCameraInfo(), false, 0);
        endAppearDemo();
    }
}

void PowerStar::exeAppearDemoKoopa() {
    if (MR::isFirstStep(this)) {
        MR::showModelIfHidden(this);
        MR::moveVolumeStageBGM(0.0f, 5);
        MR::moveVolumeSubBGM(0.0f, 5);

        MR::startAnimCameraTargetSelf(mPowerStarModelObj, mCameraInfo, MR::isStageKoopaVs3() ? "DemoKoopaGrandStarVs3" : "DemoKoopaGrandStar", 0,
                                      1.0f);

        MR::hideModelAndOnCalcAnimIfShown(this);

        MR::forceDeleteEffect(this, "Light");

        mBaseMtx.set(_B8);

        mBaseMtx.setTrans(mInitPosition);

        mPowerStarModelObj->kill();

        MR::requestMovementOn(mPowerStarModelObj);

        MR::startBck(mPowerStarModelObj, MR::isStageKoopaVs3() ? "DemoKoopaGrandStarVs3" : "DemoKoopaGrandStar", nullptr);

        _134.set(MR::getPlayerBaseMtx());

        MR::setPlayerBaseMtx(mBaseMtx);
    }

    if (MR::isStep(this, 1)) {
        MR::startBckPlayer(MR::isStageKoopaVs3() ? "DemoKoopaGrandStarVs3" : "DemoKoopaGrandStar", 0l);
    }

    if (MR::isStageKoopaVs3()) {
        if (MR::isStep(this, 152)) {
            MR::startSubBGM("BGM_KOOPA_GRAND_STAR_3", false);
        }
    } else {
        if (MR::isStep(this, 90)) {
            MR::startSubBGM("BGM_KOOPA_GRAND_STAR_1", false);
        }
    }

    if (MR::isStageKoopaVs3()) {
        if (MR::isStep(this, static_cast< s32 >(MR::getBckFrameMax(mPowerStarModelObj)) - 30)) {
            MR::startStageBGM("MBGM_STAR_EXIST_2", false);
        }
    }

    if (MR::isBckStopped(mPowerStarModelObj)) {
        MR::setPlayerBaseMtx(_134);
        mPosition.set(mAppearPosition);

        MR::showModelIfHidden(this);
        mPowerStarModelObj->appear();

        MR::endAnimCamera(mPowerStarModelObj, mCameraInfo, MR::isStageKoopaVs3() ? "DemoKoopaGrandStarVs3" : "DemoKoopaGrandStar", 0, true);
        endAppearDemo();

        if (MR::isStageKoopaVs3()) {
            MR::overlayWithPreviousScreen(2);
        }
    }
}

void PowerStar::exeWait() {
    if (MR::isFirstStep(this)) {
        if (!MR::isGalaxyGhostCometAppearInCurrentStage()) {
            MR::validateClipping(this);
        }

        MR::validateHitSensors(this);
        MR::emitEffect(this, "Light");
    }

    processWait(mIsGrandStar ? 2.0f : 3.0f);
}

void PowerStar::exeWeak() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::validateClipping(this);

        if (mColorFrame != 4) {
            MR::startBrkAndSetFrameAndStop(this, "Recover", 0.0f);
        }
    }

    _164 = true;

    processWait(0.3f);
}

void PowerStar::exeWeakNoRotate() {
    if (MR::isFirstStep(this)) {
        mRotation.y = 45.0f;
    }

    _164 = true;

    processWait(0.0f);
}

void PowerStar::exeWeakToWait() {
    if (MR::isFirstStep(this)) {
        if (mColorFrame != 4) {
            MR::setBrkRate(this, 1.0f);
        }
    }

    _164 = false;

    processWait(MR::calcNerveEaseInValue(this, 30, 0.3f, mIsGrandStar ? 2.0f : 3.0f));
    MR::setNerveAtStep(this, &NrvPowerStar::PowerStarNrvWait::sInstance, 30);
}

void PowerStar::exeStageClearDemo() {
    if (MR::isFirstStep(this)) {
        mIsGrandStar ? MR::requestGrandStarGetDemo() : MR::requestPowerStarGetDemo();

        if (mPowerStarId > 0) {
            MR::sendStageResultSequenceParam(mPowerStarId);
        }

        MR::invalidateClipping(this);

        MR::startAnimCameraTargetSelf(mPowerStarModelObj, mCameraInfo, mIsGrandStar ? "GrandStarGet" : "PowerStarGet", 0, 1.0f);
    }

    if (MR::isStep(this, 1)) {
        if (_127) {
            MR::makeMtxTR(mBaseMtx, mCameraActor);
            mBaseMtx.zeroTrans();
        } else {
            mBaseMtx.set(_B8);
        }

        MR::forceDeleteEffect(this, "Light");

        MR::hideModelIfShown(this);

        MR::startBckPlayer(mIsGrandStar ? "GrandStarGet" : "PowerStarGet", 0l);

        mBaseMtx.setTrans(mPosition);

        mPowerStarModelObj->makeActorAppeared();

        MR::startBck(mPowerStarModelObj, mIsGrandStar ? "GrandStarGet" : "PowerStarGet", nullptr);

        if (MR::isPowerStarGetDemoWithLuigiCurrentGalaxyAndScenario(mPowerStarId)) {
            mLuigiNPC->makeActorAppeared();

            MR::startBck(mLuigiNPC, "PowerStarGet", nullptr);
            MR::startBva(mLuigiNPC, "PowerStarGet");
        }

        MR::startStarPointerModePowerStarGetDemo(this);
        MR::sendMsgToAllLiveActor(ACTMES_START_POWER_STAR_GET, this);
    }

    if (mIsGrandStar && MR::isStageKoopaVs3() && MR::isStep(this, 600)) {
        MR::startBckPlayer(::cStageClearAnimNameKoopaVs3, static_cast< const char* >(nullptr));
        MR::startBck(mPowerStarModelObj, ::cStageClearAnimNameKoopaVs3, nullptr);
        MR::startAnimCameraTargetSelf(mPowerStarModelObj, mCameraInfo, ::cStageClearAnimNameKoopaVs3, 1, 1.0f);
    }
}

PowerStar::~PowerStar() {
}
