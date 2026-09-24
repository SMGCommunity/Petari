#include "Game/Screen/SurfingGuidance.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(SurfingGuidanceFadeIn, SurfingGuidance, FadeIn);
    NEW_NERVE(SurfingGuidanceLevelOffReady, SurfingGuidance, LevelOffReady);
    NEW_NERVE(SurfingGuidanceLevelOffHold, SurfingGuidance, LevelOffHold);
    NEW_NERVE(SurfingGuidanceLevelOffSuccess, SurfingGuidance, LevelOffSuccess);
    NEW_NERVE(SurfingGuidanceTurnLeftReady, SurfingGuidance, TurnLeftReady);
    NEW_NERVE(SurfingGuidanceTurnLeftHold, SurfingGuidance, TurnLeftHold);
    NEW_NERVE(SurfingGuidanceTurnLeftSuccess, SurfingGuidance, TurnLeftSuccess);
    NEW_NERVE(SurfingGuidanceTurnRightReady, SurfingGuidance, TurnRightReady);
    NEW_NERVE(SurfingGuidanceTurnRightHold, SurfingGuidance, TurnRightHold);
    NEW_NERVE(SurfingGuidanceTurnRightSuccess, SurfingGuidance, TurnRightSuccess);
    NEW_NERVE(SurfingGuidanceFadeOut, SurfingGuidance, FadeOut);
}  // namespace

SurfingGuidance::SurfingGuidance() : LayoutActor("サーフィンガイダンス", true), _20(), _24(), _2C() {
}

void SurfingGuidance::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("TiltGuidance", 3);
    initNerve(GET_NERVE_ANON(SurfingGuidanceFadeIn));

    _20 = 0;

    kill();
}

void SurfingGuidance_FORCE_MATCH_STRINGS(LayoutActor* pActor) {
    MR::startAnim(pActor, "OKAppear", 2);
    MR::startAnim(pActor, "OKEnd", 2);
}

void SurfingGuidance::activate() {
    appear();
}

void SurfingGuidance::levelOffReady() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffReady))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffReady));
    }
}

void SurfingGuidance::levelOffHold() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffHold))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffHold));
    }
}

void SurfingGuidance::levelOffSuccess() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffSuccess))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceLevelOffSuccess));
    }
}

void SurfingGuidance::turnLeftReady() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftReady))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftReady));
    }
}

void SurfingGuidance::turnLeftHold() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftHold))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftHold));
    }
}

void SurfingGuidance::turnLeftSuccess() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftSuccess))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnLeftSuccess));
    }
}

void SurfingGuidance::turnRightReady() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightReady))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightReady));
    }
}

void SurfingGuidance::turnRightHold() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightHold))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightHold));
    }
}

void SurfingGuidance::turnRightSuccess() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightSuccess))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceTurnRightSuccess));
    }
}

void SurfingGuidance::deactivate() {
    if (!isNerve(GET_NERVE_ANON(SurfingGuidanceFadeOut))) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceFadeOut));
    }
}

void SurfingGuidance::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startAnim(this, "Slope", 1);
        MR::startSystemSE("SE_SY_CTRL_GUIDE_APPEAR");
    }
}

void SurfingGuidance::exeLevelOffReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Slope", 1);
    }

    if (!MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 500);
        }
    }

    if (MR::getAnimFrame(this, 1) == 85.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 1000);
    }
}

void SurfingGuidance::exeLevelOffHold() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "SlopeHold", 1);
    }
}

void SurfingGuidance::exeLevelOffSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "SlopeOK", 1);
    }
}

void SurfingGuidance::exeTurnLeftReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnLeft", 1);
    }

    if (!MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 500);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 1000);
    }
}

void SurfingGuidance::exeTurnLeftHold() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnLeftHold", 1);
    }
}

void SurfingGuidance::exeTurnLeftSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnLeftOK", 1);
    }
}

void SurfingGuidance::exeTurnRightReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnRight", 1);
    }

    if (!MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 500);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 1000);
    }
}

void SurfingGuidance::exeTurnRightHold() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnRightHold", 1);
    }
}

void SurfingGuidance::exeTurnRightSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnRightOK", 1);
    }
}

void SurfingGuidance::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(GET_NERVE_ANON(SurfingGuidanceFadeIn));
        kill();
    }
}
