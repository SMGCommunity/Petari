#pragma once

#include <revolution.h>
#include "Actor/LiveActor/LiveActor.h"

class StarPiece;

class StarPieceGroup : public LiveActor
{
public:
    StarPieceGroup(const char *);

    virtual ~StarPieceGroup();
    virtual void init(const JMapInfo &);
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    bool isExistAnyStarPiece();
    void forceReplaceStarPieceAll();
    void forceKillStarPieceAll(bool);
    void placementAllPiece();
    void placementPieceOnCircle();
    void placementPieceOnRail();
    void placementPieceOnRailPoint();
    void onSwitchA();
    void offSwitchA();
    void onSwitchB();

    StarPiece* mStarPieces; // _8C
    u32 _90;
    u8 _94;
    u8 _95;
    u8 _96;
    u8 _97;
    u32 mNumStarbits; // _98
    f32 _9C; // obj_arg1
    u8 _A0;
    u8 _A1;
    u8 _A2;
    u8 _A3;
    s32 _A4; // obj_arg5
    f32 _A8;
    f32 _AC;
    f32 _B0;
    f32 _B4;
};