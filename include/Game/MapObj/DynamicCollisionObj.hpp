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
    CollisionParts *mParts;     // _90
    u16 _94;
    u16 mNrTriangles;           // _96
    TVec3f *mPositions;         // _98
    TVec3f *_9C;
    TriangleIndexing *mIndices; // _A0
    u32 _A4;
    u32 _A8;
    u32 _AC;
    u8 _B0[0x6C];
    u32 _11C;
};
