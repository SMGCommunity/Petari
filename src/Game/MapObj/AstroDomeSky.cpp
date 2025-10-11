#include "Game/MapObj/AstroDomeSky.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "revolution/gx/GXStruct.h"
#include "revolution/types.h"

namespace NrvAstroDomeSky {
    NEW_NERVE(AstroDomeNrvHide, AstroDomeSky, Hide);
    NEW_NERVE(AstroDomeNrvAppear, AstroDomeSky, Appear);
    NEW_NERVE(AstroDomeNrvWait, AstroDomeSky, Wait);
    NEW_NERVE(AstroDomeNrvRotateAppear, AstroDomeSky, RotateAppear);
    NEW_NERVE(AstroDomeNrvRotateDisappear, AstroDomeSky, RotateDisappear);
    NEW_NERVE(AstroDomeNrvDisappear, AstroDomeSky, Disappear);
    NEW_NERVE(AstroDomeNrvReturnDemoWait, AstroDomeSky, ReturnDemoWait);
    NEW_NERVE(AstroDomeNrvJumpOutDemo, AstroDomeSky, JumpOutDemo);
}

namespace {
    const char* cAstroDomeSkyTable[] = {"AstroDomeSkyA", "AstroDomeSkyB", "AstroDomeSkyC"};
}

AstroDomeSky::AstroDomeSky(const char* pName) : LiveActor(pName) {
    _8C = 0.0f;
    _90.r = 0;
    _90.g = 0;
    _90.b = 0;
    _90.a = 255;
    _94 = nullptr;
}

void AstroDomeSky::init(const JMapInfoIter& rIter) {
    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    const char* arg2 = cAstroDomeSkyTable[arg - 1];
    initModelManagerWithAnm(arg2, 0 , true);
    _94 = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    MR::startBtk(this, arg2);
    MR::connectToScene(this, 36, 5, 35, 30);
    MR::invalidateClipping(this);
    initNerve(&NrvAstroDomeSky::AstroDomeNrvHide::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoActionNerve(this, &NrvAstroDomeSky::AstroDomeNrvReturnDemoWait::sInstance, "移動");
    MR::registerDemoActionNerve(this, &NrvAstroDomeSky::AstroDomeNrvDisappear::sInstance, "ドーム出現");
    MR::registerDemoCast(this, "マリオ飛び出し", rIter);
    MR::registerDemoSimpleCastAll(this);
    SphereSelectorFunction::registerTarget(this);
    makeActorAppeared();
}

void AstroDomeSky::draw() const {
    MR::fillScreen(_90);
}

bool AstroDomeSky::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::isMsgSelectStart(msg)) {
        setNerve(&NrvAstroDomeSky::AstroDomeNrvAppear::sInstance);
        return true;
    }
    else if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        setNerve(&NrvAstroDomeSky::AstroDomeNrvDisappear::sInstance);
        return true;
    }
    return false;
}

bool AstroDomeSky::tryRotateAppearence() {
    if (0.1f < __fabsf(SphereSelectorFunction::getHandleRotateSpeed())) {
        if (!isNerve(&NrvAstroDomeSky::AstroDomeNrvRotateDisappear::sInstance)) {
            if (isNerve(&NrvAstroDomeSky::AstroDomeNrvRotateAppear::sInstance) && !MR::isBrkStopped(this)) {
                _8C = 59.0f - MR::getBrkFrame(this);
            }
            else {
                _8C = 0.0f;
            }
            setNerve(&NrvAstroDomeSky::AstroDomeNrvRotateDisappear::sInstance);
            return true;
        }
    }
    else {
        if (isNerve(&NrvAstroDomeSky::AstroDomeNrvRotateDisappear::sInstance)) {
            if (!MR::isBrkStopped(this)) {
                _8C = 59.0f - MR::getBrkFrame(this);
            }
            else {
                _8C = 29.0f;
            }
            setNerve(&NrvAstroDomeSky::AstroDomeNrvRotateAppear::sInstance);
            return true;
        }
    }
    return false;
}

void AstroDomeSky::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
        MR::setBrkFrameAndStop(this, MR::getBrkFrameMax(this, "Disappear"));
        // Color8.set(GXColor)
    }
}

void AstroDomeSky::exeRotateAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Appear");
        MR::setBrkFrame(this, _8C);
        MR::setBrkRate(this, (59.0f - _8C) / 200.0f);
    }
    if (tryRotateAppearence()) {
        return;
    }
}

void AstroDomeSky::exeRotateDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Disappear");
        MR::setBrkFrame(this, _8C);
        MR::setBrkRate(this, (30.0f - _8C) / 20.0f);
    }
    if (30.0f <= MR::getBrkFrame(this)) {
        MR::setBrkFrameAndStop(this, 30.0f);
    }
    if (tryRotateAppearence()) {
        return;
    }
}
