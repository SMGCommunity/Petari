#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ModelObj;
class SimpleJ3DModelDrawer;
class SwingRopePoint;

class KirairaChain : public LiveActor {
public:
    KirairaChain(const LiveActor*);
    virtual ~KirairaChain();

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void control();

    void initPoints();
    void updatePoints();
    void restrictPointFromTop(s32, s32, const TVec3f&, f32);
    void restrictPointFromBottom(s32, s32, const TVec3f&, f32);
    void restrictPointFromTopAndCalcAxis(s32, s32, const TVec3f&, f32);
    void restrictPointFromBottomAndCalcAxis(s32, s32, const TVec3f&, f32);

    /* 0x8C */ const LiveActor* mHost;
    /* 0x90 */ bool mIsCut;
    /* 0x94 */ TVec3f mCutPos;
    /* 0xA0 */ TVec3f mRailStart;
    /* 0xAC */ TVec3f mRailEnd;
    /* 0xB8 */ TPos3f mFixPointBottomMtx;
    /* 0xE8 */ TPos3f mFixPointTopMtx;
    /* 0x118 */ TVec3f mFixPointTopDir;
    /* 0x124 */ TVec3f mFixPointBottomDir;
    /* 0x130 */ s32 mPointCount;
    /* 0x134 */ SwingRopePoint** mPoints;
    /* 0x138 */ s32 _138;
    /* 0x13C */ u8 _13C[4];
    /* 0x140 */ s32 mAccelPointIdx;
    /* 0x144 */ s32 mAccelTimer;
    /* 0x148 */ TVec3f mAccel;
    /* 0x154 */ TVec3f mClippingCenter;
    /* 0x160 */ SimpleJ3DModelDrawer* mModelDrawer;
    /* 0x164 */ ModelObj* mFixPointTop;
    /* 0x168 */ ModelObj* mFixPointBottom;
};
