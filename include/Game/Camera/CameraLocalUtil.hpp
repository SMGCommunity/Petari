#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class Camera;
class CameraDirector;
class CameraMan;
class CameraTargetObj;

namespace CameraLocalUtil {
    const TVec3f& getWatchPos(const CameraMan*);
    const TVec3f& getPos(const CameraMan*);
    const TVec3f& getUpVec(const CameraMan*);
    const TVec3f& getWatchUpVec(const CameraMan*);
    f32 getFovy(const CameraMan*);
    const TVec3f& getGlobalOffset(const CameraMan*);
    const TVec3f& getLocalOffset(const CameraMan*);
    f32 getRoll(const CameraMan*);
    void setWatchPos(CameraMan*, const TVec3f&);
    void setPos(CameraMan*, const TVec3f&);
    void setUpVec(CameraMan*, const TVec3f&);
    void setWatchUpVec(CameraMan*, const TVec3f&);
    void setFovy(CameraMan*, f32);
    void setGlobalOffset(CameraMan*, const TVec3f&);
    void setLocalOffset(CameraMan*, const TVec3f&);
    void setFrontOffset(CameraMan*, f32);
    void setUpperOffset(CameraMan*, f32);
    void setRoll(CameraMan*, f32);
    CameraTargetObj* getTarget(const CameraMan*);
    const MtxPtr getMtxReg(const char*);
    const TVec3f* getVecReg(const char*);
    const char* getDummyVecRegName();
    bool isForceCameraChange();
    CameraDirector* getCameraDirector();
    void setUsedTarget(const CameraMan*, CameraTargetObj*);
    CameraTargetObj* getTarget(const Camera*);
    const TVec3f& getWatchPos(const Camera*);
    const TVec3f& getPos(const Camera*);
    const TVec3f& getUpVec(const Camera*);
    const TVec3f& getWatchUpVec(const Camera*);
    f32 getFovy(const Camera*);
    const TVec3f& getGlobalOffset(const Camera*);
    const TVec3f& getLocalOffset(const Camera*);
    f32 getRoll(const Camera*);
    void setWatchPos(Camera*, const TVec3f&);
    void setPos(Camera*, const TVec3f&);
    void setUpVec(Camera*, const TVec3f&);
    void setWatchUpVec(Camera*, const TVec3f&);
    void setUpVecAndWatchUpVec(Camera*, const TVec3f&);
    void setFovy(Camera*, f32);
    void setGlobalOffset(Camera*, const TVec3f&);
    void setLocalOffset(Camera*, const TVec3f&);
    void setFrontOffset(Camera*, f32);
    void setUpperOffset(Camera*, f32);
    void setRoll(Camera*, f32);
    void recalcUpVec(TVec3f*, const TVec3f&);
    void makeWatchOffset(TVec3f*, Camera*, CameraTargetObj*, f32);
    void makeWatchPoint(TVec3f*, Camera*, CameraTargetObj*, f32);
    void makeWatchOffsetImm(TVec3f*, Camera*, CameraTargetObj*);
    void makeWatchPointImm(TVec3f*, Camera*, CameraTargetObj*);
    bool tryCameraReset();
    bool tryCameraResetTrigger();
    bool testCameraPadButtonReset();
    bool testCameraPadTriggerReset();
    bool testCameraPadTriggerRoundLeft();
    bool testCameraPadTriggerRoundRight();
    void slerpCamera(TQuat4f*, const TQuat4f&, const TQuat4f&, f32, bool);
    void makeTowerCameraMtx(TPos3f*, const TPos3f&, const TVec3f&, const TVec3f&, const TVec3f&);
    void arrangeDistanceByFovy(Camera*, TVec3f, f32);
    void arrangeDistanceByPushAndPull(Camera*, f32, f32);
    void calcSafePose(CameraMan*, Camera*);
};  // namespace CameraLocalUtil
