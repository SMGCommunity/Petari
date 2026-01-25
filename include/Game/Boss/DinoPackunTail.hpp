#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class DinoPackunTailNode;

class DinoPackunTail {
public:
    DinoPackunTail(u32);

    void addTailNode(DinoPackunTailNode*);
    void activate();
    void deactivate();
    void onMovement();
    void lockNodePosition(u32);
    void unlockNodePosition(u32);
    void lockEndNodePosition();
    void unlockEndNodePosition();
    void registerPreCalcJointCallBack();
    void registerJointCallBack();
    void updateJoint();
    void addAccelKeepBend();
    void addAccelKeepDistance();
    void addAccelToBck();
    void getTailNodePosition(TVec3f*, s32) const;
    void getEndTailPosition(TVec3f*) const;
    DinoPackunTailNode* getNode(u32) const;

    DinoPackunTailNode** mNodes;  // 0x0
    u32 mMaxNodes;                // 0x4
    u32 mNumNodes;                // 0x8
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    u8 _1C;
};
