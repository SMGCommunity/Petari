#include "Game/NPC/TicoDomeLecture.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "math_types.hpp"

namespace NrvTicoDomeLecture {
    NEW_NERVE(TicoDomeLectureNrvWait, TicoDomeLecture, Wait);
    NEW_NERVE(TicoDomeLectureNrvMove, TicoDomeLecture, Move);
    NEW_NERVE(TicoDomeLectureNrvMetamorphosis, TicoDomeLecture, Metamorphosis);
};  // namespace NrvTicoDomeLecture

namespace {
    const static Vec cMoveEndPos = {0.0f, -70.0f, 0.0f};
    const static Vec cMoveEndRotate = {30.0f, 130.0f, 0.0f};
};  // namespace

TicoDomeLecture::TicoDomeLecture(const char* pName) : LiveActor(pName), _8C(gZeroVec), _98(gZeroVec) {
}

void TicoDomeLecture::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _8C.set(mPosition);
    _98.set(mRotation);
    initModelManagerWithAnm("Tico", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::startBrkAndSetFrameAndStop(this, "ColorChange", 1.0f);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    MR::tryRegisterDemoCast(this, rIter);
    MR::tryRegisterDemoActionNerve(this, GET_NERVE(TicoDomeLecture, TicoDomeLectureNrvMove), nullptr);
    MR::needStageSwitchWriteDead(this, rIter);
    initNerve(GET_NERVE(TicoDomeLecture, TicoDomeLectureNrvWait));
    makeActorDead();
}

void TicoDomeLecture::appear() {
    mPosition.set(_8C);
    mRotation.set(_98);
    MR::offSwitchDead(this);
    LiveActor::appear();
    setNerve(GET_NERVE(TicoDomeLecture, TicoDomeLectureNrvWait));
}

void TicoDomeLecture::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait");
    }

    MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT");
}

void TicoDomeLecture::exeMove() {
    const char* demoName = "チコ移動";
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly");
    }

    MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT");
    MR::startLevelSound(this, "SE_SM_LV_TICO_FLY_DEMO");
    if (MR::isDemoPartActive(demoName)) {
        f32 easeOut = MR::calcNerveEaseOutRate(this, MR::getDemoPartTotalStep(demoName) - 1);
        mPosition.lerp(_8C, ::cMoveEndPos, easeOut);
        mRotation.lerp(_98, ::cMoveEndRotate, easeOut);
        if (MR::isDemoPartLastStep(demoName)) {
            setNerve(GET_NERVE(TicoDomeLecture, TicoDomeLectureNrvMetamorphosis));
        }
    }
}

void TicoDomeLecture::exeMetamorphosis() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Metamorphosis");
        MR::startSound(this, "SE_SM_TICO_METAMORPHOSE");
    }

    if (MR::isBckStopped(this)) {
        MR::onSwitchDead(this);
        kill();
    }
}

TicoDomeLecture::~TicoDomeLecture() {
}
