#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"

class CinemaFrame : public LayoutActor {
public:
    CinemaFrame(bool param1);

    virtual ~CinemaFrame();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    bool tryScreenToFrame();
    bool tryFrameToBlank();
    bool tryBlankToFrame();
    bool tryFrameToScreen();
    void forceToScreen();
    void forceToFrame();
    void forceToBlank();
    bool isStop() const;
    void exeScreen();
    void exeFrame();
    void exeBlank();
    void exeScreenToFrame();
    void exeFrameToBlank();
    void exeBlankToFrame();
    void exeFrameToScreen();
};

namespace NrvCinemaFrame {
    NERVE_DECL(CinemaFrameNrvScreen, CinemaFrame, CinemaFrame::exeScreen);
    NERVE_DECL(CinemaFrameNrvFrame, CinemaFrame, CinemaFrame::exeFrame);
    NERVE_DECL(CinemaFrameNrvBlank, CinemaFrame, CinemaFrame::exeBlank);
    NERVE_DECL(CinemaFrameNrvScreenToFrame, CinemaFrame, CinemaFrame::exeScreenToFrame);
    NERVE_DECL(CinemaFrameNrvFrameToBlank, CinemaFrame, CinemaFrame::exeFrameToBlank);
    NERVE_DECL(CinemaFrameNrvBlankToFrame, CinemaFrame, CinemaFrame::exeBlankToFrame);
    NERVE_DECL(CinemaFrameNrvFrameToScreen, CinemaFrame, CinemaFrame::exeFrameToScreen);
};
