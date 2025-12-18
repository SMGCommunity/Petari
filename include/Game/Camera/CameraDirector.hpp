#pragma once

#include "Game/Camera/CameraParamChunkID.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry.hpp"

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
}

class CameraParamChunkID_Tmp : public CameraParamChunkID {
public:
    inline CameraParamChunkID_Tmp() : CameraParamChunkID() {}

    virtual char* getBuffer(u32);

    char mBuffer[0x100];  // 0xC
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
    CameraMan* getCurrentCameraMan() const;
    void updateCameraMan();
    void calcPose();
    void calcSubjective();  // TODO
    bool isInterpolationOff();
    void switchAntiOscillation();
    void createViewMtx();
    void checkStartCondition();
    void startEvent(s32, const char*, const CameraTargetArg&, s32);
    void endEvent(s32, const char*, bool, s32);
    void endEventAtLanding(s32, const char*, s32);  // TODO, need more info on _5C
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
    void checkEndOfEventCamera();                                    // TODO
    void controlCameraSE();                                          // TODO
    void removeEndEventAtLanding(s32, const char*);                  // TODO
    void calcViewMtxFromPoseParam(TPos3f*, const CameraPoseParam*);  // TODO
    bool isPlayableCameraSE(bool);
    void resetCameraMan();
    void createStartAnimCamera();
    void createTalkCamera();
    void createSubjectiveCamera();

    // 0x15C[0] and _15C[1] seems to be a struct of size 0x88 with the following layout:
    // 0x00: mZoneID
    // 0x04: mName
    // 0x84: unknown

    /* 0x0C */ CameraTargetObj* mUsedTarget;
    /* 0x10 */ CameraManStack* mStack;
    /* 0x14 */ OnlyCamera* mOnlyCamera;
    /* 0x18 */ CameraPoseParam* mPoseParam1;
    /* 0x1C */ CameraPoseParam* mPoseParam2;
    /* 0x20 */ CameraHolder* mHolder;
    /* 0x24 */ CameraParamChunkHolder* mChunkHolder;
    /* 0x28 */ GameCameraCreator* mCameraCreator;
    /* 0x2C */ CameraRailHolder* mRailHolder;
    /* 0x30 */ CameraRegisterHolder* mRegisterHolder;
    /* 0x34 */ CameraTargetHolder* mTargetHolder;
    /* 0x38 */ CameraShaker* mShaker;
    /* 0x3C */ CameraViewInterpolator* mViewInterpolator;
    /* 0x40 */ CameraCover* mCover;
    /* 0x44 */ CameraRotChecker* mRotChecker;
    /* 0x48 */ CameraManGame* mCameraManGame;
    /* 0x4C */ CameraManEvent* mCameraManEvent;
    /* 0x50 */ CameraManPause* mCameraManPause;
    /* 0x54 */ CameraManSubjective* mCameraManSubjective;
    bool _58;
    u8 _59[3];
    s32 _5C[2][34];
    u32 _16C;
    bool _170;
    u8 _171[3];
    s32 _174;
    bool mStartCameraCreated;  // 0x178
    u8 _179[3];
    CameraTargetMtx* mTargetMatrix;  // 0x17C
    TMtx34f _180;
    bool mRequestCameraManReset;  // 0x1B0
    bool _1B1;
    bool mIsSubjectiveCamera;  // 0x1B2
    bool _1B3;
    s32 _1B4;
    u8 _1B8[4];
    f32 _1BC;
    TPos3f _1C0;
    bool _1F0;
    bool _1F1;
    bool _1F2;
    u8 _1F3;
};
