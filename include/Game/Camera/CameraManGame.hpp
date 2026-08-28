#pragma once

#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Camera/CameraMan.hpp"

class Camera;
class CameraHolder;
class CameraParamChunk;
class CameraParamChunkHolder;
class CameraParamChunkID;
class CamKarikariEffector;
class CamHeliEffector;

class CameraManGame : public CameraMan {
public:
    CameraManGame(CameraHolder*, CameraParamChunkHolder*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x24 */ virtual void calc();
    /* 0x28 */ virtual void notifyActivate();
    /* 0x2C */ virtual void notifyDeactivate(){};
    /* 0x30 */ virtual bool isInterpolationOff() const;
    /* 0x34 */ virtual bool isCollisionOff() const;
    /* 0x38 */ virtual bool isZeroFrameMoveOff() const;
    /* 0x3C */ virtual bool isSubjectiveCameraOff() const;
    /* 0x40 */ virtual bool isCorrectingErpPositionOff() const;
    /* 0x44 */ virtual bool isEnableToReset() const;
    /* 0x48 */ virtual bool isEnableToRoundLeft() const;
    /* 0x4C */ virtual bool isEnableToRoundRight() const;

    void closeCreatingCameraChunk();
    void startStartPosCamera(bool);
    void endStartPosCamera();
    void zoomIn();
    void zoomOut();
    void selectCameraChunk();
    void setChunk(const CameraParamChunkID&);
    void setNullCamera();
    CameraParamChunk* tryToReplaceChunkToDefault(CameraParamChunk*);
    void requestResetIfNecessary(CameraParamChunk*);
    bool isNecessaryToReset(CameraParamChunk*) const;
    bool isCurrentChunkEnableEndInterp() const;
    void replaceCurrentChunkAndCamera(CameraParamChunk*);
    void applyParameter();
    void checkReset();
    void setSafePose();
    void keepAwayWatchPos(TVec3f*, const TVec3f&);
    void calcSafeUpVec(TVec3f*, const TVec3f&, const TVec3f&);
    void createDefaultCamera();
    void createDefaultWaterCamera();
    void createDefaultWaterSurfaceCamera();
    void createDefaultFooFighterCamera();
    void createStartAnimCamera();
    void createZoomCamera();
    void checkStateShift();
    bool tryShiftToGCapture();
    bool tryShiftToSwimOrWaterSurface();
    bool tryShiftToFooFighter();
    void updateNormal();
    void updateSwim();
    void updateWaterSurface();
    void updateGCapture();
    void updateFooFighter();
    bool setCubeChunk(CubeCameraArea::ECategory);
    bool tryStartPosCamera();
    bool tryZoomCamera();
    bool isZoomCamera() const;

    /* 0x48 */ CameraHolder* mHolder;
    /* 0x4C */ CameraParamChunkHolder* mChunkHolder;
    /* 0x50 */ Camera* mCamera;
    /* 0x54 */ CameraParamChunk* mChunk;
    /* 0x58 */ bool mRequestReset;
    /* 0x5C */ u32 _5C;
    /* 0x60 */ CamKarikariEffector* mKarikari;
    /* 0x64 */ CamHeliEffector* mHeli;
    /* 0x68 */ u32 mTypeState;
    /* 0x6C */ bool mIsStartPosActive;
    /* 0x70 */ s32 mInterpolateOffCounter;
    /* 0x74 */ bool mZoomedIn;
};
