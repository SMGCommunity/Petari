#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class CollisionParts;
class VolumeModelDrawer;

class BeamGoRoundBeam : public LiveActor {
public:
    BeamGoRoundBeam(MtxPtr);

    virtual ~BeamGoRoundBeam();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    /* 0x8C */ VolumeModelDrawer* mModelDrawer;
    /* 0x90 */ ModelObj* mBloomModel;
    /* 0x94 */ MtxPtr mBeamJointMtx;
};

class BeamGoRoundPlanet : public MapObjActor {
public:
    BeamGoRoundPlanet(const char*);

    virtual ~BeamGoRoundPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void connectToScene(const MapObjActorInitInfo&);

    void initBeam();
    void exeWait();

    /* 0xC4 */ BeamGoRoundBeam** mBeams;
    /* 0xC8 */ CollisionParts* _C8;
    /* 0xCC */ CollisionParts* _CC;
};
