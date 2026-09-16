#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;
struct KCLFile;

class DynamicCollisionObj : public LiveActor {
public:
    class TriangleIndexing {
    public:
        inline TriangleIndexing() {
        }

        u16 mIndex[3];
    };

    DynamicCollisionObj(const char* pName);

    virtual ~DynamicCollisionObj() {
    }

    virtual void init(const JMapInfoIter& rIter);

    void syncCollision();
    void updateTriangle();
    void updateCollisionHeader();
    void createCollision();

    /* 0x8C */ KCLFile* mKCLFile;
    /* 0x90 */ CollisionParts* mParts;
    /* 0x94 */ u16 _94;
    /* 0x96 */ u16 mPositionNum;
    /* 0x98 */ TVec3f* mPositions;
    /* 0x9C */ TVec3f* _9C;
    /* 0xA0 */ TriangleIndexing* mIndices;
    /* 0xA4 */ u32 _A4;
    /* 0xA8 */ u32 _A8;
    /* 0xAC */ u32 _AC;
    /* 0xB0 */ u8 _B0[0x6C];
    /* 0x11C */ u32 _11C;
};
