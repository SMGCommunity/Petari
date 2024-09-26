#include "Game/Screen/SurfingGuidance.hpp"
#include "Game/Util.hpp"

SurfingGuidance::SurfingGuidance() : LayoutActor("サーフィンガイダンス", true) {
    _20 = 0;
    _24 = 0.0f;
    _2C = 0;
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

    if (MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 0x1F4, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 85.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 0x3E8, -1);
    }
}

void SurfingGuidance::exeTurnLeftReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnLeft", 1);
    }

    if (MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 0x1F4, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 0x3E8, -1);
    }
}

void SurfingGuidance::exeTurnRightReady() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "TurnRight", 1);
    }

    if (MR::isFirstStep(this)) {
        if (MR::getAnimFrame(this, 1) == 0.0f) {
            MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE", 0x1F4, -1);
        }
    }

    if (MR::getAnimFrame(this, 1) == 35.0f) {
        MR::startSystemSE("SE_SY_CTRL_GUIDE_CHANGE2", 0x3E8, -1);
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

namespace {
    INIT_NERVE(SurfingGuidanceFadeIn);
    INIT_NERVE(SurfingGuidanceLevelOffReady);
    INIT_NERVE(SurfingGuidanceLevelOffHold);
    INIT_NERVE(SurfingGuidanceLevelOffSuccess);
    INIT_NERVE(SurfingGuidanceTurnLeftReady);
    INIT_NERVE(SurfingGuidanceTurnLeftHold);
    INIT_NERVE(SurfingGuidanceTurnLeftSuccess);
    INIT_NERVE(SurfingGuidanceTurnRightReady);
    INIT_NERVE(SurfingGuidanceTurnRightHold);
    INIT_NERVE(SurfingGuidanceTurnRightSuccess);
    INIT_NERVE(SurfingGuidanceFadeOut);

    void SurfingGuidanceFadeOut::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);
        guide->exeFadeOut();
    }

    void SurfingGuidanceTurnRightSuccess::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "TurnRightOK", 1);
        }
    }

    void SurfingGuidanceTurnRightHold::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "TurnRightHold", 1);
        }
    }

    void SurfingGuidanceTurnRightReady::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);
        guide->exeTurnRightReady();
    }

    void SurfingGuidanceTurnLeftSuccess::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "TurnLeftOK", 1);
        }
    }

    void SurfingGuidanceTurnLeftHold::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "TurnLeftHold", 1);
        }
    }

    void SurfingGuidanceTurnLeftReady::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);
        guide->exeTurnLeftReady();
    }

    void SurfingGuidanceLevelOffSuccess::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "SlopeOK", 1);
        }
    }

    void SurfingGuidanceLevelOffHold::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);

        if (MR::isFirstStep(guide)) {
            MR::startAnim(guide, "SlopeHold", 1);
        }
    }

    void SurfingGuidanceLevelOffReady::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);
        guide->exeLevelOffReady();
    }

    void SurfingGuidanceFadeIn::execute(Spine *pSpine) const {
        SurfingGuidance* guide = reinterpret_cast<SurfingGuidance*>(pSpine->mExecutor);
        guide->exeFadeIn();
    }
}

