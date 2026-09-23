#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapObjActorInitInfo;
class MapPartsRailMover;
class MapPartsRotatorBase;
class MapPartsRailRotator;
class MapPartsRailGuideDrawer;
class MapPartsRailPosture;
class ModelObj;
class MapPartsFunction;
class ProjmapEffectMtxSetter;

class MapObjActor : public LiveActor {
public:
    MapObjActor(const char* pName);
    MapObjActor(const char* pName, const char* pObjName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void connectToScene(const MapObjActorInitInfo& rInfo);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo& rInfo);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo& rInfo);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo);
    virtual void makeSubModels(const JMapInfoIter& rIter, const MapObjActorInitInfo& rInfo) {
    }

    void initialize(const JMapInfoIter& rIter, const MapObjActorInitInfo& rInfo);
    bool isObjectName(const char* pName) const;
    bool tryCreateBreakModel(const MapObjActorInitInfo& rInfo);
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

    /* 0x8C */ const char* mObjectName;
    /* 0x90 */ LodCtrl* mPlanetLodCtrl;
    /* 0x94 */ ModelObj* mBloomModel;
    /* 0x98 */ ModelObj* mModelObj;
    /* 0x9C */ ProjmapEffectMtxSetter* mMatrixSetter;
    /* 0xA0 */ MapPartsRailMover* mRailMover;
    /* 0xA4 */ MapPartsRotatorBase* mRotator;
    /* 0xA8 */ MapPartsRailRotator* mRailRotator;
    /* 0xAC */ MapPartsRailPosture* mRailPosture;
    /* 0xB0 */ MapPartsRailGuideDrawer* mRailGuideDrawer;
    /* 0xB4 */ u8 _B4;
    /* 0xB5 */ u8 _B5;
    /* 0xB6 */ u8 _B6;
    /* 0xB8 */ Nerve* mWaitNrv;
    /* 0xBC */ Nerve* mMoveNrv;
    /* 0xC0 */ Nerve* mDoneNrv;
};

class MapObjActorUtil {
public:
    static void startAllMapPartsFunctions(const MapObjActor* pActor);
    static void endAllMapPartsFunctions(const MapObjActor* pActor);
    static void pauseAllMapPartsFunctions(const MapObjActor* pActor);
    static void resumeAllMapPartsFunctions(const MapObjActor* pActor);
    static bool isRotatorMoving(const MapObjActor* pActor);
    static bool isRailMoverWorking(const MapObjActor* pActor);
    static bool isRailMoverReachedEnd(const MapObjActor* pActor);
    static f32 getSeesaw1AxisAngularSpeed(const MapObjActor* pActor);
    static void forceRotateSeesaw1Axis(const MapObjActor* pActor, f32);

    static void startRotator(const MapObjActor* pActor);
    static void startRailMover(const MapObjActor* pActor);
    static void endRotator(const MapObjActor* pActor);
    static void pauseRotator(const MapObjActor* pActor);
    static void resetRailMoverToInitPos(const MapObjActor* pActor);
    static void startBreak(MapObjActor* pActor);
    static bool tryStartBreak(MapObjActor* pActor);
    static bool isBreakStopped(const MapObjActor* pActor);
    static void killBloomModel(MapObjActor* pActor);
    static void appearBloomModel(MapObjActor* pActor);

    static void setupInitInfoTypical(MapObjActorInitInfo* pInfo, const char* pName);
    static void setupInitInfoColorChangeArg0(MapObjActorInitInfo* pInfo, const JMapInfoIter& rIter);
    static void setupInitInfoTextureChangeArg1(MapObjActorInitInfo* pInfo, const JMapInfoIter& rIter);
    static void setupInitInfoShadowLengthArg2(MapObjActorInitInfo* pInfo, const JMapInfoIter& rIter);
    static void setupInitInfoSeesaw(MapObjActorInitInfo* pInfo, const JMapInfoIter& rIter, const char* pName, f32);
    static void setupInitInfoSimpleMapObj(MapObjActorInitInfo* pInfo);
    static void setupInitInfoPlanet(MapObjActorInitInfo* pInfo);
};
