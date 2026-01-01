#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class EarthenPipe : public LiveActor {
public:
    EarthenPipe(const char*);

    virtual ~EarthenPipe();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void makeActorAppeared();
    virtual MtxPtr getBaseMtx() const;
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    bool tryShowUp();
    bool tryHideDown();
    bool isNerveShowUp() const;
    void exeWait();
    void exeReady();
    void exePlayerIn();
    void exeTargetPipeShowUp();
    void exePlayerOut();
    void exeInvalid();
    void exeHide();
    void exeShow();
    inline void exeWaitToHideDown();
    inline void exeWaitToShowUp();
    void exeShowUp();
    void exeHideDown();
    void calcTrans(f32);
    void processBgmPlayerIn();
    void processBgmPlayerOut();

    TVec3f _8C;
    TVec3f _98;
    f32 _A4;
    bool mIsIgnoreGravity;  // 0xA8
    bool _A9;
    bool _AA;
    bool _AB;
    s32 mPipeMode;  // 0xAC
    EarthenPipe* _B0;
    MtxPtr mTopJointMtx;     // 0xB4
    MtxPtr mBottomJointMtx;  // 0xB8
    TPos3f _BC;
    LiveActor* mHostActor;  // 0xEC
    TPos3f _F0;
    TMtx34f _120;
    TMtx34f _150;
    f32 _180;
    f32 mHorizExitForce;  // 0x184
    f32 mVertExitForce;   // 0x188
    s32 mMusicChangeIdx;  // 0x18C
    s32 mMusicState;      // 0x190
    u8 _194;
    u8 _195;
    u8 _196;
    u8 _197;
    PartsModel* mPipeStreamModel;  // 0x198
    bool _19C;
    bool _19D;
    bool _19E;
    bool _19F;
    ActorCameraInfo* mCameraInfo;  // 0x1A0
};

class EarthenPipeMediator : public NameObj {
public:
    EarthenPipeMediator();

    struct Entry {
        EarthenPipe* _0;
        EarthenPipe* _4;  // 0x4
        s32 mPipeID;      // 0x8
    };

    virtual ~EarthenPipeMediator();

    void entry(EarthenPipe*, const JMapInfoIter&);

    s32 mNumEntries;      // 0xC
    Entry* mPipeEntries;  // 0x10
};
