#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CinemaFrame : public LayoutActor {
public:
    CinemaFrame(bool);

    virtual ~CinemaFrame();
    virtual void init(const JMapInfoIter &);
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
};
