#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;

class DynamicCollisionObj : public LiveActor {
public:
    class TriangleIndexing {
    public:
        inline TriangleIndexing() {

        }

        u16 mIndex[3];
    };

    DynamicCollisionObj(const char *);
    virtual ~DynamicCollisionObj();

    virtual void init(const JMapInfoIter &);

    void syncCollision();
    void updateTriangle();
    void updateCollisionHeader();
    void createCollision();

    void *_8C;
    CollisionParts *mParts;     // 0x90
    u16 _94;
    u16 mNrTriangles;           // 0x96
    TVec3f *mPositions;         // 0x98
    TVec3f *_9C;
    TriangleIndexing *mIndices; // 0xA0
    u32 _A4;
    u32 _A8;
    u32 _AC;
    u8 _B0[0x6C];
    u32 _11C;
};
