#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;
class VolumeModelDrawer;

class BeamGoRoundBeam : public LiveActor {
public:
    BeamGoRoundBeam(MtxPtr);

    /* 0x08 */ virtual ~BeamGoRoundBeam();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    VolumeModelDrawer* mModelDrawer;  // 0x8C
    ModelObj* mBloomModel;            // 0x90
    MtxPtr mBeamJointMtx;             // 0x94
};

class BeamGoRoundPlanet : public MapObjActor {
public:
    BeamGoRoundPlanet(const char*);

    /* 0x08 */ virtual ~BeamGoRoundPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);

    void initBeam();
    void exeWait();

    BeamGoRoundBeam** mBeams;  // 0xC4
    CollisionParts* _C8;
    CollisionParts* _CC;
};
