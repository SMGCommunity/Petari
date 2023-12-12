#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorCameraInfo.h"
#include "Game/LiveActor/PartsModel.h"

class EarthenPipe : public LiveActor {
public:
    EarthenPipe(const char *);

    virtual ~EarthenPipe();

    TVec3f _8C;
    TVec3f _98;

    bool mIsIgnoreGravity;              // _A8
    bool _A9;
    bool _AA;
    bool _AB;
    s32 mPipeMode;                      // _AC
    EarthenPipe* _B0;
    MtxPtr mTopJointMtx;                // _B4
    MtxPtr mBottomJointMtx;             // _B8

    TPos3f _F0;

    f32 mHorizExitForce;                // _184
    f32 mVertExitForce;                 // _188
    s32 mMusicChangeIdx;                // _18C
    s32 mMusicState;                    // _190

    PartsModel* mPipeStreamModel;       // _198
    bool _19C;
    bool _19D;
    bool _19E;
    bool _19F;
    ActorCameraInfo** mCameraInfo;      // _1A0

};