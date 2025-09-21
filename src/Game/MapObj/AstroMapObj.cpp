#include "Game/MapObj/AstroMapObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/SimpleMapObj.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"
#include <cstddef>

#pragma region AstroMapObj

namespace NrvAstroMapObj {

    NEW_NERVE(AstroMapObjNrvDead, AstroMapObj, Wait);
    NEW_NERVE(AstroMapObjNrvAlive, AstroMapObj, Wait);
    NEW_NERVE(AstroMapObjNrvAliveAfterDemo, AstroMapObj, Wait);
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
    MR::getObjectName(&_CC, rIter);
    MapObjActorInitInfo mapObjInitInfo = MapObjActorInitInfo();
    _D8 = AstroMapObjFunction::getDomeIdFromArg0(rIter);
    mapObjInitInfo.setupHioNode("地形オブジェ");
    mapObjInitInfo.setupDefaultPos();
    mapObjInitInfo.setupModelName(AstroMapObjFunction::getModelName(_CC, _D8));
    mapObjInitInfo.setupConnectToScene();
    mapObjInitInfo.setupFarClipping(-1.0f);
    mapObjInitInfo.setupEffect(_CC);
    mapObjInitInfo.setupSound(1);
    mapObjInitInfo.setupNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance);
    if (checkOtherStrings(_CC)) {
        mapObjInitInfo.setupRotator();
    }
    if (checkStrings(_CC)) {
        mapObjInitInfo.setupHitSensor();
    }
    initialize(rIter, mapObjInitInfo);
    selectNrvWait();
    if (MR::isDemoCast(this, 0)) {
        MR::FunctorV0M<AstroMapObj*, void (AstroMapObj::*)()> demoFunctor = MR::Functor<AstroMapObj>(this, &AstroMapObj::startDemo); // Probably it's not startDemo
        MR::registerDemoActionFunctor(this, demoFunctor, 0);
    }
    MR::FunctorV0M<AstroMapObj*, void (AstroMapObj::*)()> otherFunct = MR::Functor<AstroMapObj>(this, &AstroMapObj::control);   // Same
    AstroMapObjFunction::tryRegisterMultiDemoAndFunction(_CC, this, rIter, otherFunct);
    MR::registerDemoSimpleCastAll(this);
    if (moreInlines(_CC)) {
        _C4 = AstroMapObjFunction::createAstroNamePlateParser();
        _C8 = new GalaxyNamePlate(0, 1);
        _C8->initWithoutIter();
        if (checkStrings(_CC)) {
            _D0 = MR::createCollisionPartsFromLiveActor(this, "Close", getSensor("body"), MR::CollisionScaleType(2));
            _D4 = MR::createCollisionPartsFromLiveActor(this, "Open", getSensor("body"), MR::CollisionScaleType(2));
        }
        AstroMapObjFunction::initilizePeculiar(_CC, this, rIter);
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

void AstroMapObj::setStateAlive() {
    MR::tryStartAllAnim(this, "Revival");
    MR::setAllAnimFrameAtEnd(this, "Revival");
    setStateDoorOpenOrClose();
    tryStartAllAnimAndEffect("AliveWait");
    if (mRotator) {
        startMapPartsFunctions();
    }
}

void AstroMapObj::setStateDoorOpenOrClose() {
    if (!isNerve(&NrvAstroMapObj::AstroMapObjNrvAliveAfterDemo::sInstance)) {
        if (checkStrings(_CC)) {
            if (AstroMapObjFunction::isAlreadyOpen(_CC, _D8)) {
                MR::tryStartAllAnim(this, "Open");
                MR::setAllAnimFrameAtEnd(this, "Open");
                MR::validateCollisionParts(_D4);
                MR::invalidateCollisionParts(_D0);
            }
            else {
                if (!LiveActor::isNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance)) {
                    MR::tryStartAllAnim(this, "Open");
                    MR::setAllAnimFrameAndStop(this, "Open", 0.0);
                }
                    MR::invalidateCollisionParts(_D4);
                    MR::validateCollisionParts(_D0);
            }
        }
    }
}

void AstroMapObj::selectNrvWait() {
    if (AstroMapObjFunction::isAlreadyRevival(_CC, _D8)) {
        setNerve(&NrvAstroMapObj::AstroMapObjNrvAlive::sInstance);
    }
    else {
        setNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance);
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

void AstroMapObj::startDemo() {
    if (isNerve(&NrvAstroMapObj::AstroMapObjNrvDead::sInstance)) {
        if (AstroMapObjFunction::isEnableRevival(_CC, _D8)) {
            setNerve(&NrvAstroMapObj::AstroMapObjNrvRevival::sInstance);
        }
    }
    else if (AstroMapObjFunction::isEnableOpen(_CC, _D8)) {
        setNerve(&NrvAstroMapObj::AstroMapObjNrvOpen::sInstance);
    }
}

void AstroMapObj::tryStartAllAnimAndEffect(const char* v1) {
    if (MapObjActor::isObjectName("AstroDomeEntranceKitchen") && MR::isEqualString(v1, "AliveWait")) {
        MR::emitEffect(this, "KitchenSmoke");
    }
    if (MR::isRegisteredEffect(this, v1)) {
        MR::emitEffect(this, v1);
    }
    MR::tryStartAllAnim(this, v1);
}

bool AstroMapObj::isEndRevival() const {
    if (MR::isExistBck(this, 0)) {
        return MR::isBckStopped(this);
    }
    if (MR::isExistBrk(this, 0)) {
        return MR::isBrkStopped(this);
    }
    return true;
}

bool AstroMapObj::isPlayMachineSE() const {
    if (MR::isDemoActive("ロゼッタ状況説明デモ") && MR::isDemoPartActive("状況説明[絵本表示]")) {
        return false;
    }
    if (MR::isDemoActive("グランドスター帰還[２回目以降]")) {
        return false;
    }
    return MR::isDemoActive("パワースター帰還") == 0;
}

AstroMapObj::~AstroMapObj() {}

// Unused?
void AstroMapObj::initCaseNoUseSwitchB(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseUseSwitchB(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseNoUseSwitchA(const MapObjActorInitInfo&) {}
void AstroMapObj::initCaseUseSwitchA(const MapObjActorInitInfo&) {}

#pragma endregion
#pragma region AstroSimpleObj

AstroSimpleObj::AstroSimpleObj(const char* pName) : SimpleMapObjFarMax(pName) {
    _C4 = 0;
    _C8 = 0;
}

void AstroSimpleObj::init(const JMapInfoIter& rIter) {
    SimpleMapObjFarMax::init(rIter);
    _C4 = AstroMapObjFunction::createAstroNamePlateParser();
    _C8 = new GalaxyNamePlate(0, 1);
    _C8->initWithoutIter();
}

void AstroSimpleObj::control() {
    MapObjActor::control();
    if (_C8) {
        AstroMapObjFunction::trySetAndShowNamePlate(_C8, _C4, mPosition, mObjectName, -1, false);
    }
}

AstroSimpleObj::~AstroSimpleObj() {

}

#pragma endregion
