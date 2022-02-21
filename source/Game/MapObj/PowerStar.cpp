#include "Game/MapObj/PowerStar.h"
#include "Game/MapObj/PowerStarHolder.h"
#include "Game/LiveActor/LiveActorGroup.h"
#include "Game/Util.h"

#ifdef __cplusplus
extern "C" {
f32 sin(f32);
f32 cos(f32);
f64 fmod(f64, f64);
}
#endif

const GXColor lightColor[5] = { { 0x96, 0x96, 0x32, 0 },
                                        { 0x32, 0x32, 0x96, 0 },
                                        { 0x32, 0x96, 0x32, 0 },
                                        { 0x96, 0x32, 0x32, 0 },
                                        { 0x64, 0x64, 0x64, 0}   
                                        };

namespace {
    void setupColor(LiveActor *pActor, bool useFrame, int frame) NO_INLINE {
        MR::startBtp(pActor, "PowerStar");
        MR::setBtpFrameAndStop(pActor, useFrame ? 0.0f : frame);
        MR::startBva(pActor, "PowerStar");
        MR::setBvaFrameAndStop(pActor, useFrame ? 1.0f : 0.0f);
    }

    void setupColorGrandStar(LiveActor *pActor, bool dontStartRecover) NO_INLINE {
        if (dontStartRecover) {
            MR::showMaterial(pActor, "GrandStarEmpty");
            MR::hideMaterial(pActor, "FooMat");
        }
        else {
            MR::showMaterial(pActor, "FooMat");
            MR::hideMaterial(pActor, "GrandStarEmpty");
            MR::startBrk(pActor, "Recover");
            MR::setBrkFrameEndAndStop(pActor);
        }
    }

    void requestPointLight(const LiveActor *pActor, int a2) NO_INLINE {
        TVec3f joint_pos;
        MR::copyJointPos(pActor, "PowerStar", &joint_pos);
        MR::requestPointLight(pActor, TVec3f(joint_pos), lightColor[a2], 1.0f, -1);
    }
};

void PowerStar::initAfterPlacement() {

}

void PowerStar::appear() {
    LiveActor::appear();
    MR::invalidateClipping(this);
    requestAppearOrWait();
}

void PowerStar::makeActorAppeared() {
    _AC.set(mPosition);
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
        MR::requestStartDemoMarioPuppetableWithoutCinmeaFrame(this, cAppearDemoName, &NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance, 0);
    }
    else {
        MR::requestStartDemoWithoutCinemaFrame(this, cAppearDemoName, &NrvPowerStar::PowerStarNrvAppearDemoRise::sInstance, 0);
    }
}

void PowerStar::setDemoAppearPos(const TVec3f &rVec) {
    bool isinArray = MR::getGroupFromArray(this);
    if (isinArray) {
        mCameraActor = getNearestAppearPoint(rVec);
        mAppearPosition.set(mCameraActor->mPosition);
    }

    mPosition.set(rVec);
}

bool PowerStar::isEndAppearDemo() const {
    bool ret = false;

    if (isNerve(&NrvPowerStar::PowerStarNrvWait::sInstance) || isNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance)) {
        ret = true;
    }

    return ret;
}

void PowerStar::offAppearDemo() {
    mIsInDemo = false;
}

void PowerStar::setupColor(LiveActor *pActor, const NameObj *a2, s32 a3) {
    bool hasPowerStar;
    s32 val = 0;
    hasPowerStar = MR::hasPowerStarInCurrentStageWithDeclarer(a2->mName, a3);

    if (MR::isPowerStarGreenInCurrentStageWithDeclarer(a2->mName, a3)) {
        val = 2;   
    }
    else if (MR::isPowerStarRedInCurrentStageWithDeclarer(a2->mName, a3)) {
        val = 3;
    }

    if (MR::isGrandStarInCurrentStageWithDeclarer(a2->mName, a3)) {
        ::setupColorGrandStar(pActor, hasPowerStar);
    }
    else {
        ::setupColor(pActor, hasPowerStar, val);
    }
}

void PowerStar::setupColorAtResultSequence(LiveActor *pPowerStar, bool a2) {
    bool hasStarAtRes;
    s32 val = 0;
    hasStarAtRes = MR::hasPowerStarAtResultSequence();

    if (MR::isPowerStarGreenAtResultSequence()) {
        val = 2;
    }
    else if (MR::isPowerStarRedAtResultSequence()) {
        val = 3;
    }

    if (a2) {
        ::setupColorGrandStar(pPowerStar, hasStarAtRes);
    }
    else {
        ::setupColor(pPowerStar, hasStarAtRes, val);
    }
}

void PowerStar::requestPointLight(const LiveActor *pActor, const NameObj *a2, s32 a3) {
    s32 colorIdx = 0;

    if (MR::hasPowerStarInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 4;
    }
    else if (MR::isPowerStarGreenInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 2;
    }
    else if (MR::isPowerStarRedInCurrentStageWithDeclarer(a2->mName, a3)) {
        colorIdx = 3;
    }

    ::requestPointLight(pActor, colorIdx);
}

void PowerStar::requestPointLightAtResultSequence(const LiveActor *pActor) {
    s32 val = 0;

    if (MR::hasPowerStarAtResultSequence()) {
        val = 4;
    }
    else if (MR::isPowerStarGreenAtResultSequence()) {
        val = 2;
    }
    else if (MR::isPowerStarRedAtResultSequence()) {
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

    return MR::isPowerStarRedInCurrentStage(a1) ? 3 : 0;
}

bool PowerStar::isCurrentStageKoopaVs3() {
    return MR::isStageKoopaVs3();
}

void PowerStar::initShadowPowerStar(LiveActor *pActor, bool a2) {
    MR::initShadowFromCSV(pActor, "Shadow");
    MR::invalidateShadowAll(pActor);
    MR::validateShadowGroup(pActor, a2 ? "円柱" : "通常");
}

void PowerStar::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
    if (MR::isPowerStarGetDemoWithLuigiCurrentGalaxy()) {
        pCollector->addArchive("LuigiNPC");
    }
}

void PowerStar::control() {
    if (!isNerve(&NrvPowerStar::PowerStarNrvWaitStartAppear::sInstance)) {
        TVec3f joint_pos;

        if (isNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance)) {
            MR::copyJointPos(mPowerStarModelObj, "PowerStar", &joint_pos);
        }
        else {
            MR::copyJointPos(this, "PowerStar", &joint_pos);
        }

        bool cond = (isNerve(&NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance) && MR::isStageKoopaVs3()) ? true : false;
        s32 val = cond ? 0x78 : -1;
        MR::requestPointLight(this, TVec3f(joint_pos), lightColor[mColorFrame], 1.0f, val);
    }
}

#ifdef NON_MATCHING
void PowerStar::calcAndSetBaseMtx() {
    f32 derp = 0.017453292f * mRotation.y;
    TVec3f stack_14(0.0f, 1.0f, 0.0f);
    TPos3f pos;
    pos.zeroTrans();
    TVec3f stack_8;
    stack_8.set(stack_14);
    PSVECMag(stack_8.toCVec());
    PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
    f32 _sin = sin(derp);
    f32 _cos = cos(derp);
    pos.mMtx[0][0] = _cos + ((1.0f - _cos) * (stack_8.x * stack_8.x));
    pos.mMtx[1][1] = _cos + ((1.0f - _cos) * (stack_8.y * stack_8.y));
    pos.mMtx[2][2] = _cos + ((1.0f - _cos) * (stack_8.z * stack_8.z));
    pos.mMtx[0][1] = (stack_8.y * ((1.0f - _cos) * stack_8.x)) - (_sin * stack_8.z);
    pos.mMtx[0][2] = (stack_8.z * ((1.0f - _cos) * stack_8.x)) - (_sin * stack_8.y);
    pos.mMtx[1][0] = (stack_8.y * ((1.0f - _cos) * stack_8.x)) - (_sin * stack_8.z);
    pos.mMtx[2][0] = (stack_8.z * ((1.0f - _cos) * stack_8.x)) - (_sin * stack_8.y);
    pos.mMtx[1][2] = (stack_8.z * ((1.0f - _cos) * stack_8.y)) - (_sin * stack_8.x);
    pos.mMtx[2][1] = (stack_8.z * ((1.0f - _cos) * stack_8.y)) - (_sin * stack_8.x);
    TPos3f mtx;
    mtx.concat(_B8, pos);
    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);
}
#endif

u32 PowerStar::receiveMsgPlayerAttack(u32 msg,  HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return 1;
    }

    return MR::isMsgJetTurtleAttack(msg);
}

u32 PowerStar::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    switch(msg) {
        case 0x98:
            return 1;
            break;
        case 0x92:
            if (isNerve(&NrvPowerStar::PowerStarNrvWait::sInstance)) {
                MR::startSystemSE("SE_SY_STAR_GET", -1, -1);
                MR::stopSoundPlayer("SE_PV_BURN_RUN", 0);
                MR::stopSoundPlayer("SE_PV_NEEDLE_DAMAGE_RUN", 0);
                MR::makeMtxTR((MtxPtr)&_E8, *MR::getPlayerRotate(), *MR::getPlayerPos());
                setNerve(&NrvPowerStar::PowerStarNrvStageClearDemo::sInstance);
                return 1;
            }
            break;
        case 0xA1:
            MR::setPlayerBaseMtx((MtxPtr)&_E8);
            return 1;
            break;
    }

    return 0;
}

void PowerStar::initMapToolInfo(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mPowerStarId);
    MR::getJMapInfoArg1NoInit(rIter, &_125);
    MR::getJMapInfoArg2NoInit(rIter, &_126);
    MR::getJMapInfoArg4NoInit(rIter, &_127);
    mAppearPosition.set(mPosition);
    _A0.set(mPosition);
    MR::registerPowerStar(this, mPowerStarId);
    MR::joinToGroupArray(this, rIter, "パワースター出現ポイントグループ", 0x10);

    if (mPowerStarId > 0) {
        if (!MR::hasPowerStarInCurrentStage(mPowerStarId)) {
            u32 frame;
            if (mIsGrandStar) {
                frame = 0;
            }
            else {
                frame = getBtpFrameCurrentStage(mPowerStarId);
            }

            mColorFrame = frame;
        }
        else {
            mColorFrame = 4;
        }
    }
}

void PowerStar::initModel() {
    initModelManagerWithAnm(isGrandStar() ? "GrandStar" : "PowerStar", 0, false);

    if (mIsGrandStar) {
        ::setupColorGrandStar(this, mColorFrame == 4);
    }
    else {
        ::setupColor(this, mColorFrame == 4, mColorFrame);
    }

    if (mIsGrandStar) {
        MtxPtr mtx = (MtxPtr)&_E8;
        const char* name = "グランドスターデモモデル";
        mPowerStarModelObj = MR::createModelObjIndirectMapObj(name, mIsGrandStar ? "GrandStar" : "PowerStar", mtx);
        ::setupColorGrandStar(mPowerStarModelObj, mColorFrame == 4);
    }
    else {
        MtxPtr mtx = (MtxPtr)&_E8;
        const char* name = "パワースターデモモデル";
        mPowerStarModelObj = MR::createModelObjNoSilhouettedMapObj(name, mIsGrandStar ? "GrandStar" : "PowerStar", mtx);
        ::setupColor(mPowerStarModelObj, mColorFrame == 4, mColorFrame);
    }

    MR::invalidateClipping(mPowerStarModelObj);
    mPowerStarModelObj->makeActorDead();

    if (MR::isPowerStarGetDemoWithLuigiCurrentGalaxy()) {
        mLuigiNPC = MR::createModelObjNpc("ルイージデモモデル", "LuigiNPC", (MtxPtr)&_E8);
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
    MR::addHitSensorMtx(this, body1Name, 0x67, 8, 60.0f, getBaseMtx(), body1Offs);
    TVec3f body2Offs = TVec3f(-95.0f, 35.0f, 0.0f);
    const char* body2Name = "body2";
    MR::addHitSensorMtx(this, body2Name, 0x67, 8, 60.0f, getBaseMtx(), body2Offs);
    TVec3f body3Offs = TVec3f(-55.0f, -75.0f, 0.0f);
    const char* body3Name = "body3";
    MR::addHitSensorMtx(this, body3Name, 0x67, 8, 60.0f, getBaseMtx(), body3Offs);
    TVec3f body4Offs = TVec3f(55.0f, -75.0f, 0.0f);
    const char* body4Name = "body4";
    MR::addHitSensorMtx(this, body4Name, 0x67, 8, 60.0f, getBaseMtx(), body4Offs);
    TVec3f body5Offs = TVec3f(95.0f, 35.0f, 0.0f);
    const char* body5Name = "body5";
    MR::addHitSensorMtx(this, body5Name, 0x67, 8, 60.0f, getBaseMtx(), body5Offs);
    TVec3f body6Offs = TVec3f(0.0f, 0.0f, 0.0f);
    const char* body6Name = "body6";
    MR::addHitSensorMtx(this, body6Name, 0x67, 8, 80.0f, getBaseMtx(), body6Offs);
}

void PowerStar::initShadow(const JMapInfoIter &rIter) {
    if (!mIsGrandStar) {
        bool arg_3 = false;
        MR::getJMapInfoArg3NoInit(rIter, &arg_3);
        bool thing = arg_3;
        MR::initShadowFromCSV(this, "Shadow");
        MR::invalidateShadowAll(this);
        MR::validateShadowGroup(this, thing ? "円柱" : "通常");
    }
    else {
        MR::initShadowFromCSV(this, "Shadow");
    }
}

void PowerStar::endAppearDemo() {
    if (!_11C) {
        MR::endDemo(this, cAppearDemoName);
    }

    MR::moveVolumeStageBGM(1.0f, 0x3C);
    MR::moveVolumeSubBGM(1.0f, 0x3C);
    setNerve(&NrvPowerStar::PowerStarNrvWait::sInstance);
}

void PowerStar::requestAppearOrWait() {
    if (MR::isDemoCast(this, 0)) {
        setNerve(&NrvPowerStar::PowerStarNrvWeak::sInstance);
    }
    else if (mIsInDemo) {
        if (_11C) {
            MR::invalidateClipping(this);
            MR::requestMovementOn(this);

            if (MR::isStageKoopaVs()) {
                setNerve(&NrvPowerStar::PowerStarNrvAppearDemoKoopa::sInstance);
            }
            else {
                setNerve(&NrvPowerStar::PowerStarNrvAppearDemoRise::sInstance);
            }
        }
        else { 
            requestAppear();
        }
    }
    else {
        setNerve(&NrvPowerStar::PowerStarNrvWait::sInstance);
    }
}

void PowerStar::calcAppearDemoRiseTrans(TVec3f *pOutTrans, f32 a2) const {
    if (_125) {
        MR::calcGravityVector(this, _AC, pOutTrans, 0, 0);
        pOutTrans->x += -a2;
        pOutTrans->y = (pOutTrans->y * -a2);
        pOutTrans->z = (pOutTrans->z * -a2);
    }
    else {
        TVec3f vec;
        vec.y = a2;
        vec.x = 0.0f;
        vec.z = 0.0f;
        _B8.mult(vec, *pOutTrans);
    }

    pOutTrans->add(_AC);
}

#ifdef NON_MATCHING
void PowerStar::processWait(f32 val) {
    f32 derp = (mRotation.y + val) - 0.0;
    f32 rot_y = fmod((360.0f + derp), 360.0f);
    mRotation.y = rot_y + 0.0;

    if (mIsGrandStar) {
        if (MR::changeShowModelFlagSyncNearClipping(this, 250.0f)) {
            MR::onDrawEffect(this);
        }
        else {
            MR::offDrawEffect(this);
        }
    }

    if (MR::isGalaxyGhostCometAppearInCurrentStage()) {
        if (MR::isJudgedToClipFrustum100m(mPosition, 5000.0f)) {
            MR::hideModelAndOnCalcAnimIfShown(this);
            MR::forceDeleteEffect(this, "Light");
        }
        else {
            MR::showModelIfHidden(this);
            MR::emitEffect(this, "Light");
        }
    }

    if (!_164) {
        if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST_W", -1, -1, -1);
        }
        else {
            MR::startLevelSound(this, "SE_OJ_LV_POW_STAR_EXIST", -1, -1, -1);
        }
    }
}
#endif