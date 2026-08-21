#pragma once

#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class PartsModel;
class MultiEventCamera;

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
    /* 0xD0 */ u8 _D0;
    /* 0xD4 */ ActorCameraInfo* mCameraInfo;
    /* 0xD8 */ MultiEventCamera* mMultEventCamera;
};

namespace MR {
    bool enableGroupAttack(LiveActor*, f32, f32);
};  // namespace MR

class JumpGuarderBaby : public BegomanBaby {
public:
    JumpGuarderBaby();
};

class JumpGuarder : public JumpEmitter {
public:
    JumpGuarder(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
    void exeOpen();
    void exeClose();
    void exeInter();

    /* 0x0DC */ JumpGuarderBaby* mBabies;
    /* 0x0E0 */ s32 mNumBabies;
    /* 0x0E4 */ u32 _E4;
    /* 0x0E8 */ JumpGuarderBaby* _E8[4];
    /* 0x0F8 */ s32 _F8;
    /* 0x0FC */ s32 mNumCoins;
    /* 0x100 */ f32 _100;
};
