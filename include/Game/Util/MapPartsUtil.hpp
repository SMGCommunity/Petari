#pragma once

#include "Game/Util.hpp"
#include "JSystem/JGeometry.hpp"

class HitSensor;
class MapPartsRailGuideDrawer;
class MapPartsRailMover;
class MapPartsRailRotator;
class LiveActor;

namespace MR {
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

    void getMapPartsObjectName(char*, u32, const char*, s32);
    void getMapPartsObjectName(char*, u32, const JMapInfoIter&);
    void getMapPartsObjectNameIfExistShapeID(char*, u32, const JMapInfoIter&);

    bool hasMapPartsShadow(s32);

    bool hasMapPartsMoveStartSignMotion(s32);

    bool hasMapPartsVanishSignMotion(s32);

    bool isMapPartsShadowTypeNone(s32);

    void setBodySensorType(LiveActor*, u32);

    const HitSensor* getBodySensor(LiveActor*);

    void initMapPartsClipping(LiveActor*, const JMapInfoIter&, TVec3f*, bool);

    MapPartsRailGuideDrawer* createMapPartsRailGuideDrawer(LiveActor*, const char*, const JMapInfoIter&);

    bool isMapPartsPressOn(const JMapInfoIter&);

    void setBodySensorTypePress(LiveActor*);
    void setBodySensorTypeMoveCollision(LiveActor*);
    void setBodySensorTypeMapObj(LiveActor*);

    bool isMapPartsSignMotionTypeMoveWait(s32);

    bool isMoveStartTypeUnconditional(s32);

    bool receiveMapPartsRotateMsg(LiveActor*, u32, MapPartsRailMover*, MapPartsRailRotator*);

    void initMapPartsShadow(LiveActor*, const JMapInfoIter&);

    bool isMapPartsRailMovePassedEndPointRepeat(const MapPartsRailMover*);

    bool isMapPartsRailMovePassedStartPointRepeat(const MapPartsRailMover*);
};  // namespace MR
