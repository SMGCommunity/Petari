#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class ModelObj;

class CrystalCageMoving : public MapObjActor {
public:
    CrystalCageMoving(const char*);

    /* 0x08 */ virtual ~CrystalCageMoving();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);

    void exeWaitBig();
    void exeBreakBig();
    void exeWaitSmall();
    void exeBreakSmall();
    void exeBreakAll();
    void exeDemoTicoMove();
    void exeDemoTicoStop();
    void exeDemoTicoChange();
    void endBreakBig();
    void crashMario(HitSensor*, HitSensor*);
    void initDummyModel(const JMapInfoIter&);
    void startBreakDemo();
    bool isNerveTypeEnd() const;

    ModelObj* mTicoModel;  // 0xC4
    TPos3f _C8;
    ActorCameraInfo* mCameraInfo;  // 0xF8
    TVec3f _FC;
    u8 _108;
};
