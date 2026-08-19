#pragma once

#include "Game/Camera/CameraHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraCover;
class CameraDirector;
class CameraHolder;
class CameraMan;
class CameraManGame;
class CameraManEvent;
class CameraManPause;
class CameraManSubjective;
class CameraParamChunkEvent;
class CameraParamChunkHolder;
class CameraPoseParam;
class CameraRailHolder;
class CameraRegisterHolder;
class CameraRotChecker;
class CameraTargetHolder;
class CameraShaker;
class CameraTargetArg;
class CameraTargetMtx;
class CameraTargetObj;
class CameraViewInterpolator;
class GameCameraCreator;
class LiveActor;
class MarioActor;
class OnlyCamera;

namespace MR {
    CameraDirector* getCameraDirector();
};  // namespace MR

struct CameraEvent {
    /* 0x00 */ s32 mZoneID;
    /* 0x04 */ char mName[128];
    /* 0x84 */ s32 mFrame;
};

#define CAMERA_MAN_CAPACITY 8

class CameraDirector : public NameObj {
public:
    class CameraManStack {
    public:
        inline CameraManStack() {
            mElements = new CameraMan*[CAMERA_MAN_CAPACITY];
            mCapacity = CAMERA_MAN_CAPACITY;
            mCount = 0;
            mElements = new CameraMan*[CAMERA_MAN_CAPACITY];
        }

        CameraMan** mElements;
        u32 mCapacity;
        u32 mCount;
    };

    CameraDirector(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void setTarget(CameraTargetObj*);
    CameraTargetObj* getTarget();
    void push(CameraMan*);
    CameraMan* pop();
    void backLastMtx();
    void updateTarget();
    CameraMan* getCurrentCameraMan() const;
    void updateCameraMan();
    void calcPose();
    void calcSubjective();
    bool isInterpolationOff() NO_INLINE;
    void switchAntiOscillation();
    void createViewMtx();
    void checkStartCondition();
    void startEvent(s32, const char*, const CameraTargetArg&, s32);
    void endEvent(s32, const char*, bool, s32);
    void endEventAtLanding(s32, const char*, s32);
    CameraParamChunkEvent* getEventParameter(s32, const char*);
    void requestToResetCameraMan();
    void setInterpolation(u32);
    void cover(u32);
    void closeCreatingCameraChunk();
    void initCameraCodeCollection(const char*, s32);
    void registerCameraCode(u32);
    void termCameraCodeCollection();
    void declareEvent(s32, const char*);
    void started();
    void setTargetActor(const LiveActor*);
    void setTargetPlayer(const MarioActor*);
    bool isRotatingHard() const;
    bool isSubjectiveCamera() const;
    bool isEnableToControl() const;
    bool isEnableToRoundLeft() const;
    bool isEnableToRoundRight() const;
    bool isEnableToReset() const;
    bool isEventCameraActive(s32, const char*) const;
    bool isEventCameraActive() const;
    void startStartPosCamera(bool);
    bool isInterpolatingNearlyEnd() const;
    bool isForceCameraChange() const;
    f32 getDefaultFovy() const;
    void startStartAnimCamera();
    bool isStartAnimCameraEnd() const;
    u32 getStartAnimCameraFrame() const;
    void endStartAnimCamera();
    void startTalkCamera(const TVec3f&, const TVec3f&, f32, f32, s32);
    void endTalkCamera(bool, s32);
    void startSubjectiveCamera(s32);
    void endSubjectiveCamera(s32);
    bool isAnimCameraEnd(s32, const char*) const;
    u32 getAnimCameraFrame(s32, const char*) const;
    void pauseOnAnimCamera(s32, const char*);
    void pauseOffAnimCamera(s32, const char*);
    void zoomInGameCamera();
    void zoomOutGameCamera();
    void checkEndOfEventCamera();
    void controlCameraSE();
    void removeEndEventAtLanding(s32, const char*);
    void calcViewMtxFromPoseParam(TPos3f*, const CameraPoseParam*);
    bool isPlayableCameraSE(bool);
    void resetCameraMan();
    void createStartAnimCamera();
    void createTalkCamera();
    void createSubjectiveCamera();

    CameraTargetObj* getTargetObj() const {
        return mTargetObj;
    }

    CameraHolder* getHolder() const {
        return mHolder;
    }

    void requestLocalOffsetReset() {
        mRequestCameraLocalOffsetReset = true;
    }

    /* 0x00C */ CameraTargetObj* mTargetObj;
    /* 0x010 */ CameraManStack* mStack;
    /* 0x014 */ OnlyCamera* mOnlyCamera;
    /* 0x018 */ CameraPoseParam* mPoseParam1;
    /* 0x01C */ CameraPoseParam* mPoseParam2;
    /* 0x020 */ CameraHolder* mHolder;
    /* 0x024 */ CameraParamChunkHolder* mChunkHolder;
    /* 0x028 */ GameCameraCreator* mCameraCreator;
    /* 0x02C */ CameraRailHolder* mRailHolder;
    /* 0x030 */ CameraRegisterHolder* mRegisterHolder;
    /* 0x034 */ CameraTargetHolder* mTargetHolder;
    /* 0x038 */ CameraShaker* mShaker;
    /* 0x03C */ CameraViewInterpolator* mViewInterpolator;
    /* 0x040 */ CameraCover* mCover;
    /* 0x044 */ CameraRotChecker* mRotChecker;
    /* 0x048 */ CameraManGame* mCameraManGame;
    /* 0x04C */ CameraManEvent* mCameraManEvent;
    /* 0x050 */ CameraManPause* mCameraManPause;
    /* 0x054 */ CameraManSubjective* mCameraManSubjective;
    /* 0x058 */ bool _58;
    /* 0x05C */ CameraEvent mEvents[2];
    /* 0x16C */ u32 mEventNum;
    /* 0x170 */ bool mIsStartCameraActive;
    /* 0x174 */ s32 mStartTime;
    /* 0x178 */ bool mStartCameraCreated;
    /* 0x17C */ CameraTargetMtx* mCameraTargetMtx;
    /* 0x180 */ TPos3f mTargetMtx;
    /* 0x1B0 */ bool mRequestCameraManReset;
    /* 0x1B1 */ bool mRequestCameraLocalOffsetReset;
    /* 0x1B2 */ bool mIsSubjectiveCamera;
    /* 0x1B3 */ bool mIsStartSubjectiveCamera;
    /* 0x1B4 */ s32 mSubjectiveFrame;
    /* 0x1B8 */ u32 _1B8;
    /* 0x1BC */ f32 mNearZ;
    /* 0x1C0 */ TPos3f mViewMtx;
    /* 0x1F0 */ bool mIsSubjectiveCalced;
    /* 0x1F1 */ bool _1F1;
    /* 0x1F2 */ bool mIsCameraNG;
    /* 0x1F3 */ u8 _1F3;
};
