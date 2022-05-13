#pragma once

#include "Game/NameObj/NameObj.h"

class DemoCastGroupHolder;
class DemoSimpleCastHolder;
class DemoStartRequestHolder;

class DemoDirector : public NameObj {
public:
    DemoDirector(const char *);

    virtual ~DemoDirector();
    virtual void movement();

    bool mIsActive;                             // _C
    s32 _10;
    u8 _14;
    DemoCastGroupHolder* _18;
    DemoCastGroupHolder* _1C;
    DemoSimpleCastHolder* _20;
    s32 _24;
    DemoStartRequestHolder* mStartReqHolder;    // _28
    NameObj* _2C;
    const char* _30;
    s32 _34;
    bool _38;
};