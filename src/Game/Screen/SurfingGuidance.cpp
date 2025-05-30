#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/SurfingGuidance.hpp"
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
};

SurfingGuidance::SurfingGuidance() :
    LayoutActor("サーフィンガイダンス", true),
    _20(0),
    _24(0.0f),
    _2C(0)
{
    
}

void SurfingGuidance::init(const JMapInfoIter &rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("TiltGuidance", 3);
    initNerve(&SurfingGuidanceFadeIn::sInstance);

    _20 = 0;

    kill();
}

void SurfingGuidance::activate() {
    appear();
}

void SurfingGuidance::levelOffReady() {
    if (!isNerve(&SurfingGuidanceLevelOffReady::sInstance)) {
        setNerve(&SurfingGuidanceLevelOffReady::sInstance);
    }
}

void SurfingGuidance::levelOffHold() {
    if (!isNerve(&SurfingGuidanceLevelOffHold::sInstance)) {
        setNerve(&SurfingGuidanceLevelOffHold::sInstance);
    }
}

void SurfingGuidance::levelOffSuccess() {
    if (!isNerve(&SurfingGuidanceLevelOffSuccess::sInstance)) {
        setNerve(&SurfingGuidanceLevelOffSuccess::sInstance);
    }
}

void SurfingGuidance::turnLeftReady() {
    if (!isNerve(&SurfingGuidanceTurnLeftReady::sInstance)) {
        setNerve(&SurfingGuidanceTurnLeftReady::sInstance);
    }
}

void SurfingGuidance::turnLeftHold() {
    if (!isNerve(&SurfingGuidanceTurnLeftHold::sInstance)) {
        setNerve(&SurfingGuidanceTurnLeftHold::sInstance);
    }
}

void SurfingGuidance::turnLeftSuccess() {
    if (!isNerve(&SurfingGuidanceTurnLeftSuccess::sInstance)) {
        setNerve(&SurfingGuidanceTurnLeftSuccess::sInstance);
    }
}

void SurfingGuidance::turnRightReady() {
    if (!isNerve(&SurfingGuidanceTurnRightReady::sInstance)) {
        setNerve(&SurfingGuidanceTurnRightReady::sInstance);
    }
}

void SurfingGuidance::turnRightHold() {
    if (!isNerve(&SurfingGuidanceTurnRightHold::sInstance)) {
        setNerve(&SurfingGuidanceTurnRightHold::sInstance);
    }
}

void SurfingGuidance::turnRightSuccess() {
    if (!isNerve(&SurfingGuidanceTurnRightSuccess::sInstance)) {
        setNerve(&SurfingGuidanceTurnRightSuccess::sInstance);
    }
}

void SurfingGuidance::deactivate() {
    if (!isNerve(&SurfingGuidanceFadeOut::sInstance)) {
        setNerve(&SurfingGuidanceFadeOut::sInstance);
    }
}

void SurfingGuidance::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
        MR::startAnim(this, "Slope", 1);
        MR::startSystemSE("SE_SY_CTRL_GUIDE_APPEAR", -1, -1);
    }
}

void SurfingGuidance::exeLevelOffReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Slope", 1);
    }

    if (!MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 500, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 85.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 1000, -1);
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
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 500, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 1000, -1);
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
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 500, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 1000, -1);
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
        setNerve(&SurfingGuidanceFadeIn::sInstance);
        kill();
    }
}

SurfingGuidance::~SurfingGuidance() {
    
}
