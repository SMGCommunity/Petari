#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class ModelObj;

class CrystalCageMoving : public MapObjActor {
public:
    CrystalCageMoving(const char*);

    virtual ~CrystalCageMoving();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void updateHitSensor(HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void connectToScene(const MapObjActorInitInfo&);

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

    /* 0x0C4 */ ModelObj* mTicoModel;
    /* 0x0C8 */ TPos3f _C8;
    /* 0x0F8 */ ActorCameraInfo* mCameraInfo;
    /* 0x0FC */ TVec3f _FC;
    /* 0x108 */ u8 _108;
};
