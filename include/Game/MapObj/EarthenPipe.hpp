#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class PartsModel;

class EarthenPipe : public LiveActor {
public:
    EarthenPipe(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim() {};
    virtual void makeActorAppeared();
    virtual MtxPtr getBaseMtx() const {
        return mTopJointMtx;
    }
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
    void exeWaitToHideDown();
    void exeWaitToShowUp();
    void exeShowUp();
    void exeHideDown();
    void calcTrans(f32);
    void processBgmPlayerIn();
    void processBgmPlayerOut();

    /* 0x08C */ TVec3f _8C;
    /* 0x098 */ TVec3f _98;
    /* 0x0A4 */ f32 _A4;
    /* 0x0A8 */ bool mIsIgnoreGravity;
    /* 0x0A9 */ bool _A9;
    /* 0x0AA */ bool _AA;
    /* 0x0AB */ bool _AB;
    /* 0x0AC */ s32 mPipeMode;
    /* 0x0B0 */ EarthenPipe* _B0;
    /* 0x0B4 */ MtxPtr mTopJointMtx;
    /* 0x0B8 */ MtxPtr mBottomJointMtx;
    /* 0x0BC */ TPos3f _BC;
    /* 0x0EC */ LiveActor* mHostActor;
    /* 0x0F0 */ TPos3f _F0;
    /* 0x120 */ TMtx34f _120;
    /* 0x150 */ TPos3f _150;
    /* 0x180 */ f32 _180;
    /* 0x184 */ f32 mHorizExitForce;
    /* 0x188 */ f32 mVertExitForce;
    /* 0x18C */ s32 mMusicChangeIdx;
    /* 0x190 */ s32 mMusicState;
    /* 0x194 */ u8 _194;
    /* 0x195 */ u8 _195;
    /* 0x196 */ u8 _196;
    /* 0x197 */ u8 _197;
    /* 0x198 */ PartsModel* mPipeStreamModel;
    /* 0x19C */ bool _19C;
    /* 0x19D */ bool _19D;
    /* 0x19E */ bool _19E;
    /* 0x19F */ bool _19F;
    /* 0x1A0 */ ActorCameraInfo* mCameraInfo;
};

class EarthenPipeMediator : public NameObj {
public:
    EarthenPipeMediator();

    struct Entry {
        /* 0x0 */ EarthenPipe* _0;
        /* 0x4 */ EarthenPipe* _4;
        /* 0x8 */ s32 mPipeID;
    };

    void entry(EarthenPipe*, const JMapInfoIter&);

    /* 0x0C */ s32 mNumEntries;
    /* 0x10 */ Entry* mPipeEntries;
};
