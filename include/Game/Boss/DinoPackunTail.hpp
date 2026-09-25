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
    DinoPackunTailNode*& getNodeRef(const u32& rIndex) const {
        return mNodes[rIndex];
    }

    /* 0x00 */ DinoPackunTailNode** mNodes;
    /* 0x04 */ u32 mMaxNodes;
    /* 0x08 */ u32 mNumNodes;
    /* 0x0C */ f32 _C;
    /* 0x10 */ f32 _10;
    /* 0x14 */ f32 _14;
    /* 0x18 */ f32 _18;
    /* 0x1C */ u8 _1C;
};
