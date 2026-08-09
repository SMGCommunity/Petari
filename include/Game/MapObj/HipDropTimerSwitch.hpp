#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class SpringValue;
class MapObjConnector;
class CollisionParts;

class HipDropTimerSwitch : public LiveActor {
public:
    HipDropTimerSwitch(const char*);

    virtual ~HipDropTimerSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void updateTimerSE();
    void exeOff();
    void exeSwitchDown();
    void exeOn();
    void exeSwitchUp();

    u32 _8C;
    /* 0x90 */ SpringValue* mSpring;
    /* 0x94 */ MapObjConnector* mMapObjConnector;
    /* 0x98 */ CollisionParts* mCollisionParts;
    TPos3f _9C;
    /* 0xCC */ s32 mTimerMax;      // In frames
    /* 0xD0 */ s32 mTimerSFXType;  // 0: A ticking, 1: B ticking
    u8 _D4;
    /* 0xD5 */ bool mWasLightPressed;
    /* 0xD6 */ bool mIsLightPressed;
};
