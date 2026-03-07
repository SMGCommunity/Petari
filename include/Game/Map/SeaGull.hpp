#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SeaGullGroup;

class SeaGull : public LiveActor {
public:
    SeaGull(SeaGullGroup*);

    virtual ~SeaGull();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeHoverFront();
    void exeHoverLeft();
    void exeHoverRight();
    void updateHover();

    SeaGullGroup* mSeaGullGroup;  // 0x8C
    s32 _90;
    bool _94;
    s32 _98;
    TVec3f* _9C;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f _B8;
    TVec3f _C4;
    s32 _D0;
    f32 _D4;
    u32 _D8;
    u32 _DC;
    s32 _E0;
};

class SeaGullGroup : public LiveActor {
public:
    SeaGullGroup(const char*);

    virtual ~SeaGullGroup();
    virtual void init(const JMapInfoIter&);

    TVec3f* updatePosInfo(s32*, bool) const;

    s32 _8C;
    TVec3f* _90;
};
