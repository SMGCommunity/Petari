#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class DesertMovingLand : public MapObjActor {
public:
    DesertMovingLand(const char* pName);
    virtual ~DesertMovingLand();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void connectToScene(const MapObjActorInitInfo& rIter);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    void startDemo();
    void updateDemoPlayerPos();
    void exeWait();
    void exeMoveDown();
    void exeMoveUp();
    void exeMoveSign();
    void exeStop();

    TVec3f _C4;
    TVec3f _D0;
    s32 _DC;
    s32 _E0;
    TMtx34f _E4;
};

namespace NrvDesertMovingLand {
    NERVE_DECL_EXE(HostTypeWaitTop, DesertMovingLand, Wait);
    NERVE_DECL_EXE(HostTypeWaitBottom, DesertMovingLand, Wait);
    NERVE_DECL_EXE(HostTypeMoveDown, DesertMovingLand, MoveDown);
    NERVE_DECL_EXE(HostTypeMoveUp, DesertMovingLand, MoveUp);
    NERVE_DECL_EXE(HostTypeMoveDownSign, DesertMovingLand, MoveSign);
    NERVE_DECL_EXE(HostTypeMoveUpSign, DesertMovingLand, MoveSign);
    NERVE_DECL_EXE(HostTypeStop, DesertMovingLand, Stop);
};  // namespace NrvDesertMovingLand

namespace {
    static const char* cDemoName = "砂漠ＵＦＯ上昇";
};
