#include "Game/MapObj/AstroCore.hpp"
#include "Game/MapObj/AstroDemoFunction.hpp"
#include <cstdio>

namespace {
    static f32 sSensorSizeTable[8] = {
        100.0f,
        300.0f,
        350.0f,
        420.0f,
        480.0f,
        540.0f,
        600.0f,
        0.0f
    };
};

AstroCore::AstroCore(const char *pName) : MapObjActor(pName) {

}

void AstroCore::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    MapObjActorUtil::setupInitInfoSimpleMapObj(&info);
    info.setupNerve(&NrvAstroCore::AstroCoreNrvWait::sInstance);
    info.setupHitSensor();
    TVec3f offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    info.setupHitSensorParam(8, sSensorSizeTable[0], offset);
    info.setupFarClipping(-1.0f);
    info.setupSound(4);
    info.setupNoAppearRiddleSE();
    initialize(rIter, info);
    AstroDemoFunction::tryRegisterGrandStarReturnWithFunctionAndSimpleCast(this, rIter, MR::FunctorV0M<AstroCore *, void (AstroCore::*)(void)>(this, &AstroCore::startDemo));
}

void AstroCore::exeGrow() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormalWeak();
        MR::emitEffect(this, "ShockWave");
    }

    if (MR::isStep(this, 40)) {
        MR::startSound(this, "SE_OJ_ASTRO_CORE_GROW", -1, -1);
        MR::tryRumblePadMiddle(this, 0);
        MR::shakeCameraNormalWeak();
        startAnimGrow();
    }
}

bool AstroCore::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) {
    return false;
}

void AstroCore::startDemo() {
    setNerve(&::NrvAstroCore::AstroCoreNrvGrow::sInstance);
}

void AstroCore::startAnimGrow() {
    s32 v3 = MR::clamp(AstroDemoFunction::getOpenedAstroDomeNum() + 1, 0, 6);
    s32 v4 = MR::clamp(v3, 0, 6);
    char buf[32];
    snprintf(buf, sizeof(buf), "Revival%d", v4);
    MR::startAllAnim(this, buf);
    getSensor(nullptr)->mRadius = sSensorSizeTable[v3];
}

void AstroCore::setStateBeforeGrow() {
    s32 v3 = MR::clamp(AstroDemoFunction::getOpenedAstroDomeNum(), 0, 6);
    s32 v4 = MR::clamp(v3, 0, 6);
    char buf[32];
    snprintf(buf, sizeof(buf), "Revival%d", v4);
    MR::startAllAnim(this, buf);
    MR::setAllAnimFrameAtEnd(this, buf);
    getSensor(nullptr)->mRadius = sSensorSizeTable[v3];
}

void AstroCore::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::isSensorPlayer(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

namespace NrvAstroCore {
    INIT_NERVE(AstroCoreNrvWait);
    INIT_NERVE(AstroCoreNrvGrow);
};

AstroCore::~AstroCore() {

}
