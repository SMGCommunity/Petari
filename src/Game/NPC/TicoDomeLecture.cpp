#include "Game/NPC/TicoDomeLecture.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "math_types.hpp"

namespace NrvTicoDomeLecture {
    NEW_NERVE(TicoDomeLectureNrvWait, TicoDomeLecture, Wait);
    NEW_NERVE(TicoDomeLectureNrvMove, TicoDomeLecture, Move);
    NEW_NERVE(TicoDomeLectureNrvMetamorphosis, TicoDomeLecture, Metamorphosis);
};  // namespace NrvTicoDomeLecture

namespace {
    const static Vec cMoveEndPos = {0.0f, -70.0f, 0.0f};
    const static Vec cMoveEndRotate = {30.0f, 130.0f, 0.0f};
}  // namespace

TicoDomeLecture::TicoDomeLecture(const char* pName) : LiveActor(pName), _8C(gZeroVec), _98(gZeroVec) {}

void TicoDomeLecture::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _8C.set< f32 >(mPosition);
    _98.set< f32 >(mRotation);
    initModelManagerWithAnm("Tico", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
    MR::startBrkAndSetFrameAndStop(this, "ColorChange", 1.0f);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    MR::initShadowFromCSV(this, "Shadow");
    MR::tryRegisterDemoCast(this, rIter);
    MR::tryRegisterDemoActionNerve(this, &NrvTicoDomeLecture::TicoDomeLectureNrvMove::sInstance, nullptr);
    MR::needStageSwitchWriteDead(this, rIter);
    initNerve(&NrvTicoDomeLecture::TicoDomeLectureNrvWait::sInstance);
    makeActorDead();
}

void TicoDomeLecture::appear() {
    mPosition.set< f32 >(_8C);
    mRotation.set< f32 >(_98);
    MR::offSwitchDead(this);
    LiveActor::appear();
    setNerve(&NrvTicoDomeLecture::TicoDomeLectureNrvWait::sInstance);
}

void TicoDomeLecture::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT", -1, -1, -1);
}

void TicoDomeLecture::exeMove() {
    const char* demoName = "チコ移動";
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly", nullptr);
    }

    MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT", -1, -1, -1);
    MR::startLevelSound(this, "SE_SM_LV_TICO_FLY_DEMO", -1, -1, -1);
    if (MR::isDemoPartActive(demoName)) {
        f32 easeOut = MR::calcNerveEaseOutRate(this, MR::getDemoPartTotalStep(demoName) - 1);
        TVec3f v5(cMoveEndPos);
        JMAVECLerp(&_8C, &v5, &mPosition, easeOut);
        TVec3f v4(cMoveEndRotate);
        JMAVECLerp(&_98, &v4, &mRotation, easeOut);
        if (MR::isDemoPartLastStep(demoName)) {
            setNerve(&NrvTicoDomeLecture::TicoDomeLectureNrvMetamorphosis::sInstance);
        }
    }
}

void TicoDomeLecture::exeMetamorphosis() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Metamorphosis", nullptr);
        MR::startSound(this, "SE_SM_TICO_METAMORPHOSE", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        MR::onSwitchDead(this);
        kill();
    }
}

TicoDomeLecture::~TicoDomeLecture() {}
