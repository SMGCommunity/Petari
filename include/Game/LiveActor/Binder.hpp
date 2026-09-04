#pragma once

#include "Game/Map/HitInfo.hpp"
#include "Game/Util/TriangleFilter.hpp"

class CollisionPartsFilterBase;

class Binder {
public:
    Binder(MtxPtr, const TVec3f*, const TVec3f*, f32, f32, u32);

    void setTriangleFilter(TriangleFilterBase*);
    void setCollisionPartsFilter(CollisionPartsFilterBase*);

    void setExCollisionParts(CollisionParts* pParts) {
        mExCollisionParts = pParts;
    }

    void clear();
    const HitInfo* getPlane(int) const;
    u32 copyPlaneArrayAndSortingSensor(HitInfo**, u32);
    static bool compSensor(HitInfo*, HitInfo*);
    const TVec3f bind(const TVec3f&);
    void moveAlongHittedPlanes(TVec3f*, TVec3f*, TVec3f*, const TVec3f&, const TVec3f&, HitInfo*, u32, bool*);
    u32 findBindedPos(TVec3f*, TVec3f*, bool*, HitInfo*, u32, bool, bool);
    bool moveWithCollisionParts(TVec3f*, TVec3f*);
    u32 storeCurrentHitInfo(HitInfo*, u32, bool);
    void obtainMomentFixReaction(HitInfo*, u32, TVec3f*, u32);
    void storeContactPlane(HitInfo*, u32);

    inline bool isBindedGround() const {
        return 0.0f <= _C8;
    }

    inline bool isBindedWall() const {
        return 0.0f <= _158;
    }

    inline bool isBindedRoof() const {
        return 0.0f <= _1E8;
    }

    /* 0x000 */ TriangleFilterBase* mTriangleFilter;
    /* 0x004 */ CollisionPartsFilterBase* mCollisionPartsFilter;
    /* 0x008 */ CollisionParts* mExCollisionParts;
    /* 0x00C */ MtxPtr _C;
    /* 0x010 */ const TVec3f* _10;
    /* 0x014 */ const TVec3f* _14;
    /* 0x018 */ f32 mRadius;
    /* 0x01C */ f32 mOffsetY;
    /* 0x020 */ const TVec3f* mOffsetVec;
    /* 0x024 */ u32 _24;
    /* 0x028 */ u32 mPlaneNum;
    /* 0x02C */ HitInfo* mPlane;
    /* 0x030 */ TVec3f mFixReactionVector;
    /* 0x03C */ HitInfo mGroundInfo;
    /* 0x0C8 */ f32 _C8;
    /* 0x0CC */ HitInfo mWallInfo;
    /* 0x158 */ f32 _158;
    /* 0x15C */ HitInfo mRoofInfo;
    /* 0x1E8 */ f32 _1E8;
    /* 0x1EC */ struct {
        bool _0 : 1;
        bool _1 : 1;
        bool _2 : 1;
        bool _3 : 1;
        bool _4 : 1;
        bool _5 : 1;
    } _1EC;
};
