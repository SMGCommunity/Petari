#include "Game/MapObj/AstroMapObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"
#include <cstddef>

namespace NrvAstroMapObj {

    NEW_NERVE(AstroMapObjNrvDead, AstroMapObj, Wait);
    NEW_NERVE(AstroMapObjNrvAlive, AstroMapObj, Wait);
    NEW_NERVE(AstroMapObjNrvAliveAfterDemo, AstroMapObj, Wait)
    NEW_NERVE(AstroMapObjNrvRevival, AstroMapObj, Revival);
    NEW_NERVE(AstroMapObjNrvOpen, AstroMapObj, Open);
}

namespace {
    s32 sAstroPathAppearStep = 90;
}

AstroMapObj::AstroMapObj(const char* pName) : MapObjActor(pName) {
    _C4 = 0;
    _C8 = 0;
    _CC = 0;
    _D0 = 0;
    _D4 = 0;
    _D8 = -1;
}

void AstroMapObj::init(const JMapInfoIter& rIter) {     // Pain
    MR::getObjectName(0, rIter);
    MapObjActorInitInfo mapObjInitInfo = MapObjActorInitInfo();
    AstroMapObjFunction::getDomeIdFromArg0(rIter);
    mapObjInitInfo.setupHioNode("地形オブジェ");
    mapObjInitInfo.setupDefaultPos();
    mapObjInitInfo.setupModelName(AstroMapObjFunction::getModelName("A", 0));
    mapObjInitInfo.setupConnectToScene();
    mapObjInitInfo.setupFarClipping(1.52345667);
    mapObjInitInfo.setupEffect("SomeEffect");
    mapObjInitInfo.setupSound(1);
    mapObjInitInfo.setupNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance);
    if (MR::isEqualString(_CC, "AstroRotateStepA") || MR::isEqualString(_CC, "AstroRotateStepB") || MR::isEqualString(_CC, "AstroDecoratePartsA")) {
        mapObjInitInfo.setupHitSensor();
    }
}

void AstroMapObj::control() {
    MapObjActor::control();
    if (_C8) {
        AstroMapObjFunction::trySetAndShowNamePlate(_C8, _C4, mPosition, _CC, _D8, checkStrings(_CC));
    }
}

void AstroMapObj::setStateDead() {
    setStateDoorOpenOrClose();
    MR::tryStartAllAnim(this, "Revival");
    MR::setAllAnimFrameAndStop(this, "Revival", 0.0f);
    setStateDoorOpenOrClose();
    if (mRotator) {
        endMapPartsFunctions();
    }
}

void AstroMapObj::exeWait() {
    if (MR::isFirstStep(this)) {
        if (LiveActor::isNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance)) {
            setStateDead();
        }
        else if (LiveActor::isNerve(&NrvAstroMapObj::AstroMapObjNrvAlive::sInstance)) {
            setStateAlive();
        }
    }
    if (AstroMapObj::isPlayMachineSE() && (isNerve(&NrvAstroMapObj::AstroMapObjNrvAlive::sInstance) || isNerve(&NrvAstroMapObj::AstroMapObjNrvAliveAfterDemo::sInstance))) {
        if (MR::isEqualString(_CC, "AstroRotateStepA")) {
            MR::startLevelSound(this, "SE_OJ_LV_ASTRO_ENGINE_1", -1, -1, -1);
        }
        else if (MR::isEqualString(_CC, "AstroRotateStepB")) {
            MR::startLevelSound(this, "SE_OJ_LV_ASTRO_ENGINE_2", -1, -1, -1);
        }
    }
}

void AstroMapObj::exeOpen() {
    if (MR::isFirstStep(this)) {
        tryStartAllAnimAndEffect("Open");
        if (MR::isEqualString(_CC, "AstroDomeEntrance")) {
            MR::startSound(this, "SE_OJ_ASTRO_ENTRANCE_OPEN", -1, -1);
        }
    }
    if (MR::isBtkStopped(this)) {
        MR::validateCollisionParts(_D4);
        MR::invalidateCollisionParts(_D0);
        setNerve(&NrvAstroMapObj::AstroMapObjNrvAliveAfterDemo::sInstance);
    }
}

void AstroMapObj::exeDead() {
    
}

void AstroMapObj::exeRevival() {
    if (MR::isFirstStep(this)) {
        tryStartAllAnimAndEffect("Revival");
        if (MR::isEqualString(_CC, "AstroStarPlate")) {
            MR::startSound(this, "SE_OJ_ASTRO_PLATE_REVIVAL", -1, -1);
        }
        else if (MR::isEqualString(mObjectName, "AstroRotateStepA")) {
            MR::startSound(this, "SE_OJ_ASTRO_STEP_REVIVAL", -1, -1);
        }
        else if (MR::isEqualString(mObjectName, "AstroRotateStepB")) {
            MR::startSound(this, "SE_OJ_ASTRO_STEP_REVIVAL", -1, -1);
        }
    }
    if (MR::isEqualString(_CC, "AstroRotateStepA")) {
        MR::startLevelSound(this, "SE_OJ_LV_ASTRO_ENGINE_1", -1, -1, -1);
    }
    else if (MR::isEqualString(_CC, "AstroRotateStepB")) {
        MR::startLevelSound(this, "SE_OJ_LV_ASTRO_ENGINE_2", -1, -1, -1);
    }
    else if (MR::isEqualString(_CC, "AstroStarPlate") && MR::isGreaterStep(this, sAstroPathAppearStep)) {
        MR::startAtmosphereLevelSE("SE_AT_LV_ASTRO_PATH_APPEAR", -1, -1);
    }
    if (AstroMapObj::isEndRevival()) {
        setNerve(&NrvAstroMapObj::AstroMapObjNrvAlive::sInstance);
    }
}

bool AstroMapObj::receiveOtherMsg(u32, HitSensor*, HitSensor*) {
    return false;
}

AstroMapObj::~AstroMapObj() {}

// Unused?
void AstroMapObj::initCaseNoUseSwitchB(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseUseSwitchB(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseNoUseSwitchA(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseUseSwitchA(const MapObjActorInitInfo&) {}

AstroSimpleObj::AstroSimpleObj(const char* pName) : MapObjActor(pName) {
    _C4 = 0;
    _C8 = 0;
}

AstroSimpleObj::~AstroSimpleObj() {

}
