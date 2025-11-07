#pragma once

#include "Game/Screen/LayoutActor.hpp"

class TalkMessageCtrl;

class TalkBalloon : public LayoutActor {
public:
    TalkBalloon(const char *);

    virtual void open(TalkMessageCtrl *);
    virtual void close();
    virtual void updateBalloon();
    virtual void updateTalking();
    virtual void pauseOff();
    virtual bool turnPage();
    virtual bool hasNextPage();
    virtual void skipMessage();
    virtual bool isTextAppearedAll();

    TalkMessageCtrl *_20;
};
