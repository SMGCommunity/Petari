#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

class AstroDomeOrbit;
class GalaxyNamePlate;
class ModelObj;
class NameObjArchiveListCollector;
class ProjmapEffectMtxSetter;

class MiniatureGalaxy : public LiveActor {
public:
    MiniatureGalaxy(const char*);

    virtual ~MiniatureGalaxy();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initPartsModel();
    void setScale(f32);
    void tryZoomIn(bool);
    void tryZoomOut();
    void updateZoomInOut();
    void updateNamePlate();
    void appearUnknownModel(const char*);
    void showUnknownModel();
    void showSelectModel();
    void hideUnknownModel();
    void appearSelectModel();
    void disappearSelectModel();
    void showNamePlate(bool);
    bool isUseKoopaFaceModel() const;
    void playPointedME();
    void playNeedStarME();

    void exeWait();
    void exeSelected();
    void exeNotSelected();
    void exeConfirmed();
    void exeOpen();

    /* 0x08C */ s32 mType;
    /* 0x090 */ s32 mState;
    /* 0x094 */ ModelObj* mUnknownModel;
    /* 0x098 */ ModelObj* mShadowModel;
    /* 0x09C */ ModelObj* mSelectModel;
    /* 0x0A0 */ ModelObj* mStarPlateModel;
    /* 0x0A4 */ TMtx34f _A4;
    /* 0x0D4 */ TMtx34f _D4;
    /* 0x104 */ ProjmapEffectMtxSetter* mProjmapEffectMtxSetter;
    /* 0x108 */ TVec3f _108;
    /* 0x114 */ const char* mObjectName;
    /* 0x118 */ const char* mName;  // mGalaxyName
    /* 0x11C */ s32 mCanZoomIn;
    /* 0x120 */ s32 mZoomLevel;
    /* 0x124 */ J3DTexMtx _124;
    /* 0x1B8 */ J3DTexMtx _1B8;
    /* 0x24C */ AstroDomeOrbit* mOrbit;
    /* 0x250 */ GalaxyNamePlate* mNamePlate;
};
