#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LodCtrl;
class MapPartsRailMover;
class ModelObj;
class NameObjArchiveListCollector;

class RainCloud : public LiveActor {
public:
    RainCloud(const char*);

    /* 0x08 */ virtual ~RainCloud();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeEnd();
    void exeSoftTouch();
    void exeHardTouch();
    void switchEffect();
    void updateRainCylinder();
    bool isNextStartOK() const;

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    ModelObj* mCloudCylinder;  // 0x8C
    TPos3f _90;
    TPos3f _C0;
    MapPartsRailMover* mRailMover;  // 0xF0
    LodCtrl* _F4;
    TVec3f _F8;
    f32 _104;
    f32 _108;
    bool mIsOnPlayer;  // 0x10C
    bool _10D;
};
