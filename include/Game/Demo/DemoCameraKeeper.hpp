#pragma once

#include <revolution/types.h>

class ActorCameraInfo;
class DemoExecutor;
class JMapInfoIter;
class LiveActor;

class DemoCameraInfo {
public:
    /// @brief Creates a new `DemoCameraInfo`.
    DemoCameraInfo();

    /* 0x00 */ const char* mPartName;
    /* 0x04 */ const char* mCameraTargetName;
    /* 0x08 */ s32 mCameraTargetCastID;
    /* 0x0C */ const char* mAnimCameraName;
    /* 0x10 */ s32 mAnimCameraStartFrame;
    /* 0x14 */ s32 mAnimCameraEndFrame;
    /* 0x18 */ bool mIsContinuous;
    /* 0x1C */ const char* _1C;
    /* 0x20 */ ActorCameraInfo* _20;
    /* 0x24 */ LiveActor* _24;
};

class DemoCameraKeeper {
public:
    DemoCameraKeeper(DemoExecutor*, const JMapInfoIter&);

    void initCast(LiveActor*, const JMapInfoIter&);
    void start();
    void update();
    void end();
    void initActorCamera(DemoCameraInfo*, const JMapInfoIter&);
    void executeType(const DemoCameraInfo*);
    void executeFirst(const DemoCameraInfo*);
    void executeLast(const DemoCameraInfo*);
    void endCurrentCamera();

    /* 0x00 */ DemoExecutor* mExecutor;
    /* 0x04 */ s32 _4;
    /* 0x08 */ DemoCameraInfo* _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ const DemoCameraInfo* _10;
};
