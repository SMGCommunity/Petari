#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"
#include "Game/Util/ObjUtil.hpp"

class DemoCastGroupHolder;
class DemoSimpleCastHolder;
class DemoStartRequestHolder;

class DemoDirector : public NameObj {
public:
    DemoDirector(const char *);

    virtual ~DemoDirector();
    virtual void movement();

    void registerDemoSimpleCast(LiveActor *);
    void registerDemoSimpleCast(LayoutActor *);
    void registerDemoSimpleCast(NameObj *);

    bool mIsActive;                             // _C
    DemoExecutor* mExecutor;                    // _10
    u8 _14;
    DemoCastGroupHolder* _18;
    DemoCastGroupHolder* _1C;
    DemoSimpleCastHolder* _20;
    ResourceHolder* mResourceHolder;            // _24
    DemoStartRequestHolder* mStartReqHolder;    // _28
    NameObj* _2C;
    const char* _30;
    s32 _34;
    bool _38;
};