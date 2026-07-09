#include "Game/Screen/CinemaFrame.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace NrvCinemaFrame {
    NEW_NERVE(CinemaFrameNrvScreen, CinemaFrame, Screen);
    NEW_NERVE(CinemaFrameNrvFrame, CinemaFrame, Frame);
    NEW_NERVE(CinemaFrameNrvBlank, CinemaFrame, Blank);
    NEW_NERVE(CinemaFrameNrvScreenToFrame, CinemaFrame, ScreenToFrame);
    NEW_NERVE(CinemaFrameNrvFrameToBlank, CinemaFrame, FrameToBlank);
    NEW_NERVE(CinemaFrameNrvBlankToFrame, CinemaFrame, BlankToFrame);
    NEW_NERVE(CinemaFrameNrvFrameToScreen, CinemaFrame, FrameToScreen);
};  // namespace NrvCinemaFrame

CinemaFrame::CinemaFrame(bool isConnectToSceneLayout) : LayoutActor("シネマフレーム", true) {
    if (isConnectToSceneLayout) {
        MR::connectToScene(this, MR::MovementType_Layout, MR::CalcAnimType_Layout, -1, MR::DrawType_CinemaFrame);
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

void CinemaFrame::tryScreenToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance) || isNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance) ||
                     isNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);

    if (!isInvalid) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance);
    }
}

void CinemaFrame::tryFrameToBlank() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(&NrvCinemaFrame::CinemaFrameNrvBlank::sInstance) || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToBlank::sInstance);

    if (!isInvalid) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToBlank::sInstance);
    }
}

void CinemaFrame::tryBlankToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance) || isNerve(&NrvCinemaFrame::CinemaFrameNrvScreenToFrame::sInstance) ||
                     isNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);

    if (!isInvalid) {
        setNerve(&NrvCinemaFrame::CinemaFrameNrvBlankToFrame::sInstance);
    }
}

void CinemaFrame::tryFrameToScreen() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance) || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrameToScreen::sInstance);

    if (!isInvalid) {
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
    return isNerve(&NrvCinemaFrame::CinemaFrameNrvScreen::sInstance) || isNerve(&NrvCinemaFrame::CinemaFrameNrvFrame::sInstance) ||
           isNerve(&NrvCinemaFrame::CinemaFrameNrvBlank::sInstance);
}

void CinemaFrame::exeScreen() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        MR::setAnimFrameAndStop(this, MR::getAnimCtrl(this, 0)->getEnd() - 1.0f, 0);
        kill();
    }
}

void CinemaFrame::exeFrame() {
    MR::startAnimAtFirstStep(this, "Wait", 0);
}

void CinemaFrame::exeBlank() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Open", 0);
        MR::setAnimFrameAndStop(this, 0.0f, 0);
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
