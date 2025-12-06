#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class RainCloud : public LiveActor {
public:
    RainCloud(const char*);

    virtual ~RainCloud();
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

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
