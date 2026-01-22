#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StarPiece;

class StarPieceGroup : public LiveActor {
public:
    StarPieceGroup(const char*);

    virtual void init(const JMapInfoIter&);
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
    void exeFlow();
    void exeGroup();

    /* 0x8C */ StarPiece** mPieces;
    /* 0x90 */ f32* mRailCoords;
    bool _94;
    bool _95;
    /* 0x98 */ s32 mNumPieces;
    /* 0x9C */ f32 mCircleRadius;
    /* 0xA0 */ bool mIsRail;
    /* 0xA1 */ bool mPlaceAtPathPoints;
    s32 _A4;
    /* 0xA8 */ f32 mRailSpeed;
    TVec3f _AC;
};
