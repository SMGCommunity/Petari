#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CoconutTreeLeafGroup;

class CoconutTreeLeaf : public LiveActor {
public:
    CoconutTreeLeaf(const CoconutTreeLeafGroup* pGroup, MtxPtr pJointMtx, const TVec3f* pWindDirection);

    virtual ~CoconutTreeLeaf();
    virtual void init(const JMapInfoIter& rIter);
    virtual MtxPtr getBaseMtx() const;
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void update(f32 windAccel, f32 windRandomAccel);

    /* 0x8C */ const CoconutTreeLeafGroup* mGroup;
    /* 0x90 */ TVec3f mTipPosition;
    /* 0x9C */ TVec3f mRestTipPosition;
    /* 0xA8 */ TVec3f mRestUp;
    /* 0xB4 */ TVec3f mAxisX;
    /* 0xC0 */ TVec3f mAxisY;
    /* 0xCC */ TVec3f mAxisZ;
    /* 0xD8 */ MtxPtr mJointMtx;
    /* 0xDC */ s32 mWindStep;
    /* 0xE0 */ s32 mWindStartStep;
    /* 0xE4 */ TVec3f mWindAccel;
    /* 0xF0 */ const TVec3f* mWindDirection;
};

class CoconutTreeLeafGroup : public LiveActor {
public:
    CoconutTreeLeafGroup(const char* pName);

    virtual ~CoconutTreeLeafGroup();
    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /* 0x8C */ s32 mNumLeaves;
    /* 0x90 */ CoconutTreeLeaf** mLeaves;
    /* 0x94 */ TVec3f mWindDirection;
    /* 0xA0 */ f32 mDistanceToPlayer;
};
