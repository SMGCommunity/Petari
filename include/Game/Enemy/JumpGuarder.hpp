#pragma once

#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class PartsModel;
class MultiEventCamera;
class JumpGuarderBaby;

class JumpEmitter : public LiveActor {
public:
    JumpEmitter(const char*);

    virtual void kill();

    void initEventCamera(const JMapInfoIter&);
    void startEventCamera();
    void updateEventCamera();
    void endEventCamera();
    void updateRotate();

    /* 0x8C */ MtxPtr mBodyJointMtx;
    /* 0x90 */ TMtx34f _90;
    /* 0xC0 */ PartsModel* mHeadModel;
    /* 0xC4 */ TVec3f _C4;
    /* 0xD0 */ bool mIsEventCamera;
    /* 0xD4 */ ActorCameraInfo* mCameraInfo;
    /* 0xD8 */ MultiEventCamera* mMultEventCamera;
};

namespace MR {
    bool enableGroupAttack(LiveActor*, f32, f32);
};  // namespace MR

class JumpGuarder : public JumpEmitter {
public:
    JumpGuarder(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /// @brief Filters out their own babies.
    bool isHit(const LiveActor*) const;

    bool enableAttack();
    void exeHide();
    void exeUp();
    void exeWait();
    void exeDown();
    void exeHopStart();
    void exeHopWait();
    void exeHopJump();
    void exeHopEnd();
    void exePreOpen();
    void addBaby(JumpGuarderBaby*);
    void exeOpen();
    void exeClose();
    void exeInter();

    /// @brief Allocated array of `mNumBabies` babies.
    /* 0x0DC */ JumpGuarderBaby* mBabies;

    /* 0x0E0 */ s32 mNumBabies;
    /* 0x0E4 */ s32 mBumpCooldown;
    /* 0x0E8 */ JumpGuarderBaby* mActiveBabies[4];
    /* 0x0F8 */ s32 mNumActiveBabies;
    /* 0x0FC */ s32 mNumCoins;
    /* 0x100 */ f32 mBabyVelocity;
};

class JumpGuarderBaby : public BegomanBaby {
public:
    JumpGuarderBaby();
};
