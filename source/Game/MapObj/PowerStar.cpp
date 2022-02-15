#include "Game/MapObj/PowerStar.h"
#include "Game/Util.h"

#ifdef __cplusplus
extern "C" {
f64 fmod(f64, f64);
}
#endif
  
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