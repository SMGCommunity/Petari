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

CinemaFrame::CinemaFrame(bool isConnectToScene) : LayoutActor("シネマフレーム", true) {
    if (isConnectToScene) {
        MR::connectToScene(this, MR::MovementType_Layout, MR::CalcAnimType_Layout, MR::DrawBufferType_None, MR::DrawType_CinemaFrame);
    }
}

void CinemaFrame::init(const JMapInfoIter& rIter) {
    initLayoutManager("CinemaFrame", 1);
    initNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreen));
    kill();
}

void CinemaFrame::appear() {
    LayoutActor::appear();
    setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreenToFrame));
}

void CinemaFrame::tryScreenToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrame)) || isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreenToFrame)) ||
                     isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlankToFrame));

    if (!isInvalid) {
        setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreenToFrame));
    }
}

void CinemaFrame::tryFrameToBlank() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlank)) || isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrameToBlank));

    if (!isInvalid) {
        setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrameToBlank));
    }
}

void CinemaFrame::tryBlankToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrame)) || isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreenToFrame)) ||
                     isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlankToFrame));

    if (!isInvalid) {
        setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlankToFrame));
    }
}

void CinemaFrame::tryFrameToScreen() {
    if (MR::isDead(this)) {
        appear();
    }

    bool isInvalid = isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreen)) || isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrameToScreen));

    if (!isInvalid) {
        setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrameToScreen));
    }
}

void CinemaFrame::forceToScreen() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreen));
}

void CinemaFrame::forceToFrame() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrame));
}

void CinemaFrame::forceToBlank() {
    if (MR::isDead(this)) {
        appear();
    }

    setNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlank));
}

bool CinemaFrame::isStop() const {
    return isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvScreen)) || isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvFrame)) ||
           isNerve(GET_NERVE(CinemaFrame, CinemaFrameNrvBlank));
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
    MR::setNerveAtAnimStopped(this, GET_NERVE(CinemaFrame, CinemaFrameNrvFrame), 0);
}

void CinemaFrame::exeFrameToBlank() {
    MR::startAnimAtFirstStep(this, "Close", 0);
    MR::setNerveAtAnimStopped(this, GET_NERVE(CinemaFrame, CinemaFrameNrvBlank), 0);
}

void CinemaFrame::exeBlankToFrame() {
    MR::startAnimAtFirstStep(this, "Open", 0);
    MR::setNerveAtAnimStopped(this, GET_NERVE(CinemaFrame, CinemaFrameNrvFrame), 0);
}

void CinemaFrame::exeFrameToScreen() {
    MR::startAnimAtFirstStep(this, "End", 0);
    MR::setNerveAtAnimStopped(this, GET_NERVE(CinemaFrame, CinemaFrameNrvScreen), 0);
}
