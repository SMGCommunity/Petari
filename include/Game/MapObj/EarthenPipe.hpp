#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class EarthenPipe : public LiveActor {
public:
    EarthenPipe(const char *);

    virtual ~EarthenPipe();
    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void makeActorAppeared();
    virtual MtxPtr getBaseMtx();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    bool tryShowUp();
    bool tryHideDown();
    bool isNerveShowUp();
    void exeWait();
    void exeReady();
    void exePlayerIn();
    void exeTargetPipeShowUp();
    void exePlayerOut();
    void exeInvalid();
    void exeShowUp();
    void exeHideDown();
    void calcTrans(f32);
    void processBgmPlayerIn();
    void processBgmPlayerOut();

    TVec3f _8C;
    TVec3f _98;
    f32 _A4;
    bool mIsIgnoreGravity;              // _A8
    bool _A9;
    bool _AA;
    bool _AB;
    s32 mPipeMode;                      // _AC
    EarthenPipe* _B0;
    MtxPtr mTopJointMtx;                // _B4
    MtxPtr mBottomJointMtx;             // _B8
    TPos3f _BC;
    u32 _EC;
    TPos3f _F0;
    TMtx34f _120;
    TMtx34f _150;
    f32 _180;
    f32 mHorizExitForce;                // _184
    f32 mVertExitForce;                 // _188
    s32 mMusicChangeIdx;                // _18C
    s32 mMusicState;                    // _190
    u8 _194;
    u8 _195;
    u8 _196;
    u8 _197;
    PartsModel* mPipeStreamModel;       // _198
    bool _19C;
    bool _19D;
    bool _19E;
    bool _19F;
    ActorCameraInfo** mCameraInfo;      // _1A0
};

class EarthenPipeMediator : public NameObj {
public:
    EarthenPipeMediator();

    struct Entry {
        EarthenPipe* _0;
        EarthenPipe* _4;     // _4
        s32 mPipeID;            // _8
    };

    virtual ~EarthenPipeMediator();

    void entry(EarthenPipe *, const JMapInfoIter &);

    s32 mNumEntries;                // _C
    Entry* mPipeEntries;            // _10
};