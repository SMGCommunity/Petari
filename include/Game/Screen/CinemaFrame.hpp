#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CinemaFrame : public LayoutActor {
public:
    CinemaFrame(bool param1);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();

    void tryScreenToFrame();
    void tryFrameToBlank();
    void tryBlankToFrame();
    void tryFrameToScreen();
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
