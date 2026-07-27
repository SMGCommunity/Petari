#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActorGroup;

class KoopaBattleMapPlanet : public LiveActor {
public:
    KoopaBattleMapPlanet(const char*, const char*, bool, bool, bool);

    virtual ~KoopaBattleMapPlanet();
    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const { return (MtxPtr)&mBaseMtx; }
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void startAnimPowerStarAppear();
    void createPlate();

    void exeWait();
    void exeDemoGrandStarAppear();

private:
    /* 0x8C */ const char* mModelName;
    /* 0x90 */ TPos3f mBaseMtx;
    /* 0xC0 */ TVec3f _C0;
    /* 0xCC */ LiveActorGroup* mDamagePlateGroup;
    /* 0xD0 */ LiveActorGroup* mCoinPlateGroup;
    /* 0xD4 */ LiveActorGroup* mPlateGroup;
    /* 0xD8 */ LiveActor* mDamagePlate;
    /* 0xDC */ bool _DC;
    /* 0xDD */ bool _DD;
    /* 0xDE */ bool _DE;
};
