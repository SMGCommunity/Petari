#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/MapObjActorInitInfo.h"

class LodCtrl;
class MapPartsRailMover;
class MapPartsRotator;
class MapPartsRailRotator;
class MapPartsRailGuideDrawer;
class MapPartsRailPosture;
class ModelObj;
class MapPartsFunction;

namespace NrvMapObjActor {
    NERVE(HostTypeWait);
    NERVE(HostTypeMove);
    NERVE(HostTypeDone);
};

namespace {
    const char* cBrkNameColorChange = "ColorChange";
    const char* cBtpNameTexChange = "TexChange";
    const char* cBtkNameTexChange = "TexChange";
    const char* cEffectNameAppear = "Appear";
    const char* cBckNameMove = "Move";
    const char* cFollowJointName = "Move";
    const char* cEffectNameBreak = "Break";
    const char* cBckNameBreak = "Break";
};

class MapObjActor : public LiveActor {
public:
    MapObjActor(const char *);
    MapObjActor(const char *, const char *);

    virtual ~MapObjActor();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void connectToScene(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);
    virtual void makeSubModels(const JMapInfoIter &, const MapObjActorInitInfo &) {

    }

    void initialize(const JMapInfoIter &, const MapObjActorInitInfo &);
    bool isObjectName(const char *) const;
    bool tryCreateBreakModel(const MapObjActorInitInfo &);
    bool tryEmitWaitEffect();
    bool tryDeleteWaitEffect();
    void startMapPartsFunctions();
    void endMapPartsFunctions();
    void pauseMapPartsFunctions();
    void setStateWait();
    void updateProjmapMtx();
    void exeWait();
    void exeMove();

    const char* mObjectName;                    // _8C
    LodCtrl* mPlanetLodCtrl;                    // _90
    ModelObj* mBloomModel;                      // _94
    ModelObj* mModelObj;                        // _98
    u32 _9C;
    MapPartsRailMover* mRailMover;              // _A0
    MapPartsRotator* mRotator;                  // _A4
    MapPartsRailRotator* mRailRotator;          // _A8
    MapPartsRailPosture* mRailPosture;          // _AC
    MapPartsRailGuideDrawer* mRailGuideDrawer;  // _B0
    u8 _B4;
    u8 _B5;
    u8 _B6;
    NrvMapObjActor::HostTypeWait* mWaitNrv;     // _B8
    NrvMapObjActor::HostTypeMove* mMoveNrv;     // _BC
    NrvMapObjActor::HostTypeDone* mDoneNrv;     // _C0
};
