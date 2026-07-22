#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <revolution/types.h>

class HitSensor;
class JMapInfoIter;
class MapPartsRailGuideDrawer;
class MapPartsRailMover;
class MapPartsRailRotator;
class LiveActor;

namespace MR {
    const HitSensor* getBodySensor(LiveActor*);
    void setBodySensorType(LiveActor*, u32);
    void setBodySensorTypeMapObj(LiveActor*);
    void setBodySensorTypePress(LiveActor*);
    void setBodySensorTypeMoveCollision(LiveActor*);
    bool isBodySensorTypeMapObj(const LiveActor*);
    bool receiveMapPartsRotateMsg(LiveActor*, u32, MapPartsRailMover*, MapPartsRailRotator*);
    bool tryStartMapPartsRotateAtPoint(LiveActor*, MapPartsRailMover*, MapPartsRailRotator*);
    bool tryEndMapPartsRotateAtPoint(LiveActor*, MapPartsRailMover*, MapPartsRailRotator*);
    bool tryStartMapPartsRotateBetweenPoints(LiveActor*, MapPartsRailMover*, MapPartsRailRotator*);
    bool isMapPartsRailMovePassedStartPointRepeat(const MapPartsRailMover*);
    bool isMapPartsRailMovePassedEndPointRepeat(const MapPartsRailMover*);
    void getMapPartsObjectName(char*, u32, const JMapInfoIter&);
    void getMapPartsObjectNameIfExistShapeID(char*, u32, const JMapInfoIter&);
    void getMapPartsObjectName(char*, u32, const char*, s32);
    void initMapPartsClipping(LiveActor*, const JMapInfoIter&, TVec3f*, bool);
    bool isMapPartsPressOn(const JMapInfoIter&);
    MapPartsRailGuideDrawer* createMapPartsRailGuideDrawer(LiveActor*, const char*, const JMapInfoIter&);
    void initMapPartsShadow(LiveActor*, const JMapInfoIter&);

    bool getMapPartsArgMoveConditionType(s32*, const JMapInfoIter&);
    bool getMapPartsArgRotateSpeed(f32*, const JMapInfoIter&);
    bool getMapPartsArgRotateTime(s32*, const JMapInfoIter&);
    bool getMapPartsArgRotateAngle(f32*, const JMapInfoIter&);
    bool getMapPartsArgRotateAxis(s32*, const JMapInfoIter&);
    bool getMapPartsArgRotateAccelType(s32*, const JMapInfoIter&);
    bool getMapPartsArgRotateStopTime(s32*, const JMapInfoIter&);
    bool getMapPartsArgRotateType(s32*, const JMapInfoIter&);
    s32 getMapPartsArgShadowType(s32*, const JMapInfoIter&);
    bool getMapPartsArgSignMotionType(s32*, const JMapInfoIter&);
    bool getMapPartsArgMovePosture(s32*, const JMapInfoIter&);
    bool getMapPartsArgMoveStopType(s32*, const JMapInfoIter&);
    bool getMapPartsArgRailGuideType(s32*, const JMapInfoIter&);
    bool getMapPartsArgRailInitPosType(s32*, const JMapInfoIter&);
    bool getMapPartsArgMoveSpeed(f32*, const JMapInfoIter&);
    bool getMapPartsArgMoveSpeed(f32*, const LiveActor*);
    bool getMapPartsArgStopTime(s32*, const JMapInfoIter&);
    bool getMapPartsArgAccelTime(s32*, const JMapInfoIter&);
    bool getMapPartsArgSpeedCalcType(s32*, const JMapInfoIter&);
    bool getMapPartsArgRailRotateSpeed(f32*, const JMapInfoIter&);
    bool getMapPartsArgRailRotateTime(s32*, const JMapInfoIter&);

    bool isMapPartsSignMotionTypeMoveStart(s32);
    bool isMapPartsSignMotionTypeMoveWait(s32);
    bool hasMapPartsMoveStartSignMotion(s32);
    bool hasMapPartsVanishSignMotion(s32);
    bool isMapPartsShadowTypeNone(s32);
    bool hasMapPartsShadow(s32);
    bool isMoveStartTypeUnconditional(s32);
    bool isMoveStartTypePlayerOnStopEnd(s32);
    bool isMapPartsRailInitPosTypeRailPos(s32);
    bool isMapPartsRailInitPosTypeRailPoint(s32);
    bool isMapPartsRailInitPosTypePoint0(s32);
    bool isMapPartsRailSpeedCalcTypeTime(s32);

};  // namespace MR
