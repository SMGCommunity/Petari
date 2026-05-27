#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/J3DGraphBase/J3DTexture.hpp"

class NameObjArchiveListCollector;
class AstroDomeOrbit;
class GalaxyNamePlate;

class MiniatureGalaxy : public LiveActor {
public:
    MiniatureGalaxy(const char*);
    ~MiniatureGalaxy();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    /* 0x8C */ s32 mGalaxyType;  // 0 = major, 1 = minor, 2 = bowser (obj_arg0)
    /* 0x90 */ s32 mState;       // 0 = hidden, 1 = can't open, 2 = can open, 3 = open
    /* 0x94 */ ModelObj* mUnknownModel;
    /* 0x98 */ ModelObj* mShadowModel;
    /* 0x9C */ ModelObj* mSelectModel;
    /* 0xA0 */ ModelObj* mStarPlateModel;
    /* 0xA4 */ TMtx34f _A4;
    /* 0xD4 */ TMtx34f _D4;
    /* 0x104 */ ProjmapEffectMtxSetter* mProjmapEffectMtxSetter;
    /* 0x108 */ TVec3f _108;
    /* 0x114 */ const char* mMiniatureName;
    /* 0x118 */ const char* mName;
    /* 0x11C */ s32 mCanZoomIn;
    /* 0x120 */ s32 mZoomLevel;
    /* 0x124 */ J3DTexMtx _124;
    /* 0x1B8 */ J3DTexMtx _1B8;
    /* 0x24C */ AstroDomeOrbit* mOrbit;
    /* 0x250 */ GalaxyNamePlate* mNamePlate;
};
