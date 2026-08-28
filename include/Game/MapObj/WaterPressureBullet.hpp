#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;

class WaterPressureBullet : public LiveActor {
public:
    WaterPressureBullet(const char*);

    /* 0x08 */ virtual ~WaterPressureBullet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void shotWaterBullet(LiveActor*, const TPos3f&, f32, bool, bool, bool, ActorCameraInfo**);
    void exeFly();
    void exeSpinKill();
    bool startHostCamera() const;
    bool endHostCamera() const;
    bool inviteMario(HitSensor*);
    void updateSuffererMtx();

    TVec3f _8C;
    TVec3f _98;
    LiveActor* _A4;
    f32 _A8;
    LiveActor* mHostActor;  // 0xAC
    bool _B0;
    bool _B1;
    bool _B2;
    u8 _B3;
    ActorCameraInfo** mCameraInfo;  // 0xB4
};
