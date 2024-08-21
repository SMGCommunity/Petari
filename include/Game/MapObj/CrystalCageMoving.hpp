#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

namespace {
    TVec3f sDummyModelOffset;
};

class CrystalCageMoving : public MapObjActor {
public:
    CrystalCageMoving(const char *);

    virtual ~CrystalCageMoving();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void control();
    virtual void updateHitSensor(HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);
    virtual void connectToScene(const MapObjActorInitInfo &);

    void exeBreakBig();
    void exeBreakSmall();
    void exeBreakAll();
    void exeDemoTicoMove();
    void exeDemoTicoStop();
    void exeDemoTicoChange();
    void crashMario(HitSensor *, HitSensor *);
    void initDummyModel(const JMapInfoIter &);
    void startBreakDemo();
    bool isNerveTypeEnd() const;

    ModelObj* mTicoModel;           // 0xC4
    TPos3f _C8;
    ActorCameraInfo* mCameraInfo;   // 0xF8
    TVec3f _FC;
    u8 _108;
};

namespace NrvCrystalCageMoving {
    NERVE(CrystalCageMovingNrvWaitBig);
    NERVE_EXECEND(CrystalCageMovingNrvBreakBig);
    NERVE(CrystalCageMovingNrvWaitSmall);
    NERVE(CrystalCageMovingNrvBreakSmall);
    NERVE(CrystalCageMovingNrvBreakAll);
    NERVE(CrystalCageMovingNrvDemoTicoMove);
    NERVE(CrystalCageMovingNrvDemoTicoStop);
    NERVE(CrystalCageMovingNrvDemoTicoChange);
};