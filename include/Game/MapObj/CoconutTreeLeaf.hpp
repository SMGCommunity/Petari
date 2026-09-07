#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoconutTreeLeafGroup;

class CoconutTreeLeaf : public LiveActor {
public:
    CoconutTreeLeaf(const CoconutTreeLeafGroup*, MtxPtr, const TVec3f*);

    virtual ~CoconutTreeLeaf();
    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const;
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void update(f32, f32);

    /* 0x8C */ CoconutTreeLeafGroup* mGroup;
    TVec3f _90;
    TVec3f _9C;
    TVec3f _A8;
    TVec3f _B4;
    TVec3f _C0;
    TVec3f _CC;
    MtxPtr _D8;
    s32 _DC;
    s32 _E0;
    TVec3f _E4;
    TVec3f _F0;
};

class CoconutTreeLeafGroup : public LiveActor {
public:
    CoconutTreeLeafGroup(const char*);

    virtual ~CoconutTreeLeafGroup();
    virtual void init(const JMapInfoIter&);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /* 0x8C */ s32 mNumLeaves;
    /* 0x90 */ CoconutTreeLeaf** mLeaves;
};
