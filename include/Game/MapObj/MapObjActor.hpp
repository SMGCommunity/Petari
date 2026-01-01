#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class LodCtrl;
class MapPartsRailMover;
class MapPartsRotator;
class MapPartsRailRotator;
class MapPartsRailGuideDrawer;
class MapPartsRailPosture;
class ModelObj;
class MapPartsFunction;
class ProjmapEffectMtxSetter;

class MapObjActor : public LiveActor {
public:
    MapObjActor(const char*);
    MapObjActor(const char*, const char*);

    inline virtual ~MapObjActor();

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void connectToScene(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);
    virtual void makeSubModels(const JMapInfoIter&, const MapObjActorInitInfo&);

    void initialize(const JMapInfoIter&, const MapObjActorInitInfo&);
    bool isObjectName(const char*) const;
    bool tryCreateBreakModel(const MapObjActorInitInfo&);
    bool tryEmitWaitEffect();
    bool tryDeleteWaitEffect();
    void startMapPartsFunctions();
    void endMapPartsFunctions();
    void pauseMapPartsFunctions();
    void setStateWait();
    void updateProjmapMtx();
    void exeWait();
    void exeMove();
    void exeDone();

    const char* mObjectName;                    // 0x8C
    LodCtrl* mPlanetLodCtrl;                    // 0x90
    ModelObj* mBloomModel;                      // 0x94
    ModelObj* mModelObj;                        // 0x98
    ProjmapEffectMtxSetter* mMatrixSetter;      // 0x9C
    MapPartsRailMover* mRailMover;              // 0xA0
    MapPartsRotator* mRotator;                  // 0xA4
    MapPartsRailRotator* mRailRotator;          // 0xA8
    MapPartsRailPosture* mRailPosture;          // 0xAC
    MapPartsRailGuideDrawer* mRailGuideDrawer;  // 0xB0
    u8 _B4;
    u8 _B5;
    u8 _B6;
    Nerve* mWaitNrv;  // 0xB8
    Nerve* mMoveNrv;  // 0xBC
    Nerve* mDoneNrv;  // 0xC0
};

class MapObjActorUtil {
public:
    static void startAllMapPartsFunctions(const MapObjActor*);
    static void endAllMapPartsFunctions(const MapObjActor*);
    static void pauseAllMapPartsFunctions(const MapObjActor*);
    static void resumeAllMapPartsFunctions(const MapObjActor*);
    static bool isRotatorMoving(const MapObjActor*);
    static bool isRailMoverWorking(const MapObjActor*);
    static bool isRailMoverReachedEnd(const MapObjActor*);
    static f32 getSeesaw1AxisAngularSpeed(const MapObjActor*);
    static void forceRotateSeesaw1Axis(const MapObjActor*, f32);

    static void startRotator(const MapObjActor*);
    static void startRailMover(const MapObjActor*);
    static void endRotator(const MapObjActor*);
    static void pauseRotator(const MapObjActor*);
    static void resetRailMoverToInitPos(const MapObjActor*);
    static void startBreak(MapObjActor*);
    static bool tryStartBreak(MapObjActor*);
    static bool isBreakStopped(const MapObjActor*);
    static void killBloomModel(MapObjActor*);
    static void appearBloomModel(MapObjActor*);

    static void setupInitInfoTypical(MapObjActorInitInfo*, const char*);

    static void setupInitInfoSimpleMapObj(MapObjActorInitInfo*);

    static void setupInitInfoPlanet(MapObjActorInitInfo*);

    static void setupInitInfoSeesaw(MapObjActorInitInfo*, const JMapInfoIter&, const char*, f32);

    static void setupInitInfoColorChangeArg0(MapObjActorInitInfo*, const JMapInfoIter&);
    static void setupInitInfoTextureChangeArg1(MapObjActorInitInfo*, const JMapInfoIter&);
    static void setupInitInfoShadowLengthArg2(MapObjActorInitInfo*, const JMapInfoIter&);
};
