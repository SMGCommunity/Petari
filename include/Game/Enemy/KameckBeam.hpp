#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener {
public:
    /// @brief Creates a new `KameckBeamEventListener`.
    KameckBeamEventListener();

    virtual void hitBeam(s32) = 0;
};

class KameckBeamCollisionFilter {
public:
    KameckBeamCollisionFilter(const TVec3f*, f32);

    virtual bool isInvalidParts(const CollisionParts*) const;

    /* 0x00 */ const TVec3f* _4;
    /* 0x08 */ f32 _8;
};

class KameckBeam : public LiveActor {
public:
    /// @brief Creates a new `KameckBeam`.
    /// @param pName A pointer to the null-terminated name of the object.
    KameckBeam(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setWandLocalPosition(const TVec3f&);
    void setBeamKind(s32);
    void setEventListener(KameckBeamEventListener*);
    void resetBeam();
    bool requestFollowWand(MtxPtr, f32);
    void requestShootToPlayerGround(f32);
    void requestShootToPlayerCenter(f32);
    void requestShoot(const TVec3f&, f32);
    bool requestStorm(HitSensor*, HitSensor*);
    bool tryShootEnd();
    bool tryChangeTurtle();
    bool tryChangeFire();
    void exeFollowWand();
    void exeShoot();
    void exeExplosion();
    void exeJetTurtle();
    void exeFire();
    void startBeamLevelSound();
    void emitBeamReadyEffect();
    void emitBeamEffect();

    /* 0x8C */ KameckBeamEventListener* mEventListener;
    /* 0x90 */ LiveActor* _90;
    /* 0x94 */ LiveActor* _94[3];
    /* 0xA0 */ MtxPtr _A0;
    /* 0xA4 */ TVec3f _A4;
    /* 0xB0 */ TVec3f mWandLocalPosition;
    /* 0xBC */ s32 mBeamKind;
    /* 0xC0 */ u8 _C0[4];
};

namespace MR {
    void setKameckBeamCollisionFilter(LiveActor*);
};  // namespace MR
