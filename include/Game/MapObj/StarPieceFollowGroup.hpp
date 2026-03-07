#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class StarPiece;

class StarPieceFollowGroup : public LiveActor {
public:
    StarPieceFollowGroup(const char*);

    virtual void init(const JMapInfoIter& rIter);

    bool beginFollowPieces();
    void placementPiece(s32);
    void updateAxises(const TVec3f&, const TVec3f&, const TVec3f&, f32);
    void followPieces();
    void allKillPieces();
    void onSwitchA();
    void exeFollowToPlayer();
    void exeFollowEnd();

    /* 0x8C */ StarPiece** mPieces;
    TVec3f* _90;
    /* 0x94 */ s32 mNumPiecesTotal;
    /* 0x98 */ s32 mNumPiecesUse;
    /* 0x9C */ f32 mRadius;
    // appears to go unused
    u32 _A0;
    /* 0xA4 */ f32 mAngle;
    /* 0xA8 */ s32 mTimeLimit;
    /* 0xAC */ TVec3f mLocalXAxis;
    /* 0xB8 */ TVec3f mLocalZAxis;
    /* 0xC4 */ TVec3f mLocalYAxis;
};
