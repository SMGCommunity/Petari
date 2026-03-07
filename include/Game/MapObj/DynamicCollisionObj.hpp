#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;
struct KCLFile;

class DynamicCollisionObj : public LiveActor {
public:
    class TriangleIndexing {
    public:
        inline TriangleIndexing() {}

        u16 mIndex[3];
    };

    DynamicCollisionObj(const char*);
    virtual ~DynamicCollisionObj();

    virtual void init(const JMapInfoIter&);

    void syncCollision();
    void updateTriangle();
    void updateCollisionHeader();
    void createCollision();

    KCLFile* mKCLFile;           //  0x8C
    CollisionParts* mParts;      //  0x90
    u16 _94;                     //  0x94 : Always set to 2 by AreaPolygon::init
    u16 mPositionNum;            //  0x96
    TVec3f* mPositions;          //  0x98
    TVec3f* _9C;                 //  0x9C
    TriangleIndexing* mIndices;  //  0xA0
    u32 _A4;                     //  0xA4
    u32 _A8;                     //  0xA8
    u32 _AC;                     //  0xAC
    u8 _B0[0x6C];                //  0xB0 : Seems completely unused
    u32 _11C;                    // 0x11C : Only used by FollowCollisionArea
};
