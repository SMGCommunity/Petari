#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class SpringValue;
class MapObjConnector;
class CollisionParts;

class HipDropTimerSwitch : public LiveActor {
public:
    HipDropTimerSwitch(const char*);

    /* 0x08 */ virtual ~HipDropTimerSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
