#pragma once

#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/ObjUtil.hpp"

class DemoCastGroupHolder;
class DemoSimpleCastHolder;
class DemoStartRequestHolder;

class DemoDirector : public NameObj {
public:
    DemoDirector(const char*);

    virtual ~DemoDirector();
    virtual void movement();

    bool registerDemoCast(LiveActor*, const JMapInfoIter&);
    bool registerDemoCast(LiveActor*, const char*, const JMapInfoIter&);
    void registerDemoSimpleCast(LiveActor*);
    void registerDemoSimpleCast(LayoutActor*);
    void registerDemoSimpleCast(NameObj*);

    void endDemo(NameObj*, const char*, bool);

    bool isExistTimeKeepDemo(const char*) const;

    char* getCurrentDemoName() const;

    bool mIsActive;           // 0xC
    DemoExecutor* mExecutor;  // 0x10
    u8 _14;
    DemoCastGroupHolder* _18;
    DemoCastGroupHolder* _1C;
    DemoSimpleCastHolder* _20;
    ResourceHolder* mResourceHolder;          // 0x24
    DemoStartRequestHolder* mStartReqHolder;  // 0x28
    NameObj* _2C;
    const char* _30;
    s32 _34;
    bool _38;
};
