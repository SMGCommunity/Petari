#pragma once

#include "Game/Camera/CameraParamChunkID.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TPosition3.h"

class CameraCover;
class CameraHolder;
class CameraManGame;
class CameraManEvent;
class CameraManPause;
class CameraManSubjective;
class CameraParamChunkHolder;
class CameraPoseParam;
class CameraRailHolder;
class CameraRegisterHolder;
class CameraRotChecker;
class CameraTargetHolder;
class CameraShaker;
class CameraTargetMtx;
class CameraViewInterpolator;
class GameCameraCreator;
class OnlyCamera;

class CameraParamChunkID_Tmp : public CameraParamChunkID {
public:
    inline CameraParamChunkID_Tmp() : CameraParamChunkID() {
        
    }
     
    virtual char *getBuffer(unsigned long);

    char mBuffer[0x100]; // _C
};

class CameraDirector : public NameObj {
public:
    CameraDirector(const char *);
    virtual ~CameraDirector();    

    virtual void init(const JMapInfoIter &);
    //virtual void movement();

    //setTarget(CameraTargetObj *);
    //getTarget();
    //push(CameraMan *);
    //pop();
    //backLastMtx();
    //getCurrentCameraMan() const;
    //updateCameraMan();
    //calcPose();
    //calcSubjective();
    //isInterpolationOff();
    //switchAntiOscillation();
    //createViewMtx();
    //checkStartCondition();
    //startEvent(long, const char *, const CameraTargetArg &, long);
    //endEvent(long, const char *, bool, long);
    //endEventAtLanding(long, const char *, long);
    //getEventParameter(long, const char *);
    //requestToResetCameraMan();
    void setInterpolation(unsigned long);
    //cover(unsigned long);
    //closeCreatingCameraChunk();
    //initCameraCodeCollection(const char *, long);
    //registerCameraCode(unsigned long);
    //termCameraCodeCollection();
    //declareEvent(long, const char *);
    //started();
    //setTargetActor(const LiveActor *);
    //setTargetPlayer(const MarioActor *);
    //isRotatingHard() const;
    //isSubjectiveCamera() const;
    //isEnableToControl() const;
    //isEnableToRoundLeft() const;
    //isEnableToRoundRight() const;
    //isEnableToReset() const;
    //isEventCameraActive(long, const char *) const;
    //isEventCameraActive() const;
    //startStartPosCamera(bool);
    //isInterpolatingNearlyEnd() const;
    //isForceCameraChange() const;
    f32 getDefaultFovy() const;
    //startStartAnimCamera();
    //isStartAnimCameraEnd() const;
    //getStartAnimCameraFrame() const;
    //endStartAnimCamera();
    //startTalkCamera(const TVec3f &, const TVec3f &, float, float, long);
    //endTalkCamera(bool, long);
    //startSubjectiveCamera(long);
    //endSubjectiveCamera(long);
    //isAnimCameraEnd(long, const char *) const;
    //getAnimCameraFrame(long, const char *) const;
    //pauseOnAnimCamera(long, const char *);
    //pauseOffAnimCamera(long, const char *);
    //zoomInGameCamera();
    //zoomOutGameCamera();
    //checkEndOfEventCamera();
    //controlCameraSE();
    //removeEndEventAtLanding(long, const char *);
    //calcViewMtxFromPoseParam(TPos3f *, const CameraPoseParam *);
    //isPlayableCameraSE(bool);
    //resetCameraMan();
    //createStartAnimCamera();
    //createTalkCamera();
    //createSubjectiveCamera();

    u32 _C;
    u32 _10;
    OnlyCamera *mOnlyCamera;                    // _14
    CameraPoseParam *mPoseParam1;               // _18
    CameraPoseParam *mPoseParam2;               // _1C
    CameraHolder *mHolder;                      // _20
    CameraParamChunkHolder *mChunkHolder;       // _24
    GameCameraCreator *mCameraCreator;          // _28
    CameraRailHolder *mRailHolder;              // _2C
    CameraRegisterHolder *mRegisterHolder;      // _30
    CameraTargetHolder *mTargetHolder;          // _34
    CameraShaker *mShaker;                      // _38
    CameraViewInterpolator *mViewInterpolator;  // _3C
    CameraCover *mCover;                        // _40
    CameraRotChecker *mRotChecker;              // _44
    CameraManGame *mCameraManGame;              // _48
    CameraManEvent *mCameraManEvent;            // _4C
    CameraManPause *mCameraManPause;            // _50
    CameraManSubjective *mCameraManSubjective;  // _54
    u8 _58;
    u8 _59[3];
    u8 _5C[0x110];
    u32 _16C;
    u8 _170;
    u8 _171[3];
    u32 _174;
    u8 _178;
    u8 _179[3];
    CameraTargetMtx *mTargetMatrix;             // _17C
    TMtx34f _180;
    u8 _1B0;
    u8 _1B1;
    u8 _1B2;
    u8 _1B3;
    u32 _1B4;
    u8 _1B8[4];
    f32 _1BC;
    TMtx34f _1C0;
    u8 _1F0;
    u8 _1F1;
    u8 _1F2;
    u8 _1F3;
};
