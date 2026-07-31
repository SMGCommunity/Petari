#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StarPiece;

class StarPieceMother : public LiveActor {
public:
    StarPieceMother(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();

    void exeMoveOnRail();
    void placementPieceOnRail(f32);
    void onSwitchA();
    void offSwitchA();
    void attackSensor(HitSensor*, HitSensor*);
    void emitStarPieces();
    void exeWait();

private:
    /* 0x8C */ StarPiece** mStarPieceArray;
    /* 0x90 */ f32* _90;
    /* 0x94 */ s32 _94;
    /* 0x98 */ u32 _98;
    /* 0x9C */ u32 _9C;
    /* 0xA0 */ f32 _A0;
};
