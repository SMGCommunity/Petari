#include "Game/Screen/CinemaFrame.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"

CinemaFrame::CinemaFrame(bool param1) :
    LayoutActor("シネマフレーム", true)
{
    if (param1) {
        MR::connectToScene(this, 14, 13, -1, 69);
    }
}

void CinemaFrame::init(const JMapInfoIter& rIter) {
    initLayoutManager("CinemaFrame", 1);
    initNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance);
    kill();
}

void CinemaFrame::appear() {
    LayoutActor::appear();
    setNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance);
}

bool CinemaFrame::tryScreenToFrame() {
    bool result;

    if (MR::isDead(this)) {
        appear();
    }

    result = isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);

    if (!result) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance);
    }
}

bool CinemaFrame::tryFrameToBlank() {
    bool result;

    if (MR::isDead(this)) {
        appear();
    }

    result = isNerve(&NrvCinemaFrame::CinemaFrameNrvBlank::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToBlank::sInstance);

    if (!result) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToBlank::sInstance);
    }
}

bool CinemaFrame::tryBlankToFrame() {
    bool result;

    if (MR::isDead(this)) {
        appear();
    }

    result = isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);

    if (!result) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);
    }
}

bool CinemaFrame::tryFrameToScreen() {
    bool result;

    if (MR::isDead(this)) {
        appear();
    }

    result = isNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToScreen::sInstance);

    if (!result) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToScreen::sInstance);
    }
}

void CinemaFrame::forceToScreen() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance);
}

void CinemaFrame::forceToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance);
}

void CinemaFrame::forceToBlank() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(&NrvCinemaFrame::CinemaFrameNrvBlank::sInstance);
}

bool CinemaFrame::isStop() const {
    return isNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance)
        || isNerve(&NrvCinemaFrame::CinemaFrameNrvBlank::sInstance);
}

void CinemaFrame::exeScreen() {
    J3DFrameCtrl* pAnimCtrl;

    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);

        pAnimCtrl = MR::getAnimCtrl(this, 0);

        MR::setAnimFrameAndStop(this, pAnimCtrl->mEndFrame - 57.29578f, 0);
        kill();
    }
}

void CinemaFrame::exeFrame() {
    MR::startAnimAtFirstStep(this, "Wait", 0);
}

void CinemaFrame::exeBlank() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Open", 0);
        MR::setAnimFrameAndStop(this, -0.5f, 0);
    }
}

void CinemaFrame::exeScreenToFrame() {
    MR::startAnimAtFirstStep(this, "Appear", 0);
    MR::setNerveAtAnimStopped(this, &NrvCinemaFrame::CinemaFrameNrvFrame::sInstance, 0);
}

void CinemaFrame::exeFrameToBlank() {
    MR::startAnimAtFirstStep(this, "Close", 0);
    MR::setNerveAtAnimStopped(this, &NrvCinemaFrame::CinemaFrameNrvBlank::sInstance, 0);
}

void CinemaFrame::exeBlankToFrame() {
    MR::startAnimAtFirstStep(this, "Open", 0);
    MR::setNerveAtAnimStopped(this, &NrvCinemaFrame::CinemaFrameNrvFrame::sInstance, 0);
}

void CinemaFrame::exeFrameToScreen() {
    MR::startAnimAtFirstStep(this, "End", 0);
    MR::setNerveAtAnimStopped(this, &NrvCinemaFrame::CinemaFrameNrvScreen::sInstance, 0);
}

CinemaFrame::~CinemaFrame() {
    
}

namespace NrvCinemaFrame {
    INIT_NERVE(CinemaFrameNrvScreen);
    INIT_NERVE(CinemaFrameNrvFrame);
    INIT_NERVE(CinemaFrameNrvBlank);
    INIT_NERVE(CinemaFrameNrvScreenToFrame);
    INIT_NERVE(CinemaFrameNrvFrameToBlank);
    INIT_NERVE(CinemaFrameNrvBlankToFrame);
    INIT_NERVE(CinemaFrameNrvFrameToScreen);
};
