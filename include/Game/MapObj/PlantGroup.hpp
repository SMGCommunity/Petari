#pragma once

#include "Game/MapObj/CutBushModelObj.hpp"

class PlantMember;

class PlantGroup : public LiveActor {
public:
    PlantGroup(const char*);

    virtual ~PlantGroup();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initMember(s32, const JMapInfoIter&);
    s32 placeOnCollisionFormCircle(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    f32 calcBoundingSphereRadius(const TVec3f&);
    void emitHintEffect();

    /* 0x8C */ PlantMember** mMembers;
    /* 0x90 */ s32 mMemberCount;
    /* 0x94 */ s32 mPlantType;
    /* 0x98 */ bool mIsStarPiece;
    /* 0x9C */ TVec3f mHintPosition;
    /* 0xA8 */ TVec3f mHintRotation;
    /* 0xB4 */ s32 mHintTimer;
    /* 0xB8 */ s32 mHintIndex;
};

class PlantMember : public CutBushModelObj {
public:
    PlantMember(const char* pName, const char* pModelName, bool useLight, MtxPtr pMtx) : CutBushModelObj(pName, pModelName, useLight, pMtx) {
    }

    virtual ~PlantMember();
    virtual void init(const JMapInfoIter&);

    bool tryEmitHint();
    void exeWait();
    void exeHint();
    void exeShakeWeak();
    void exeShakeMiddle();
    void exeShakeStrong();
    bool generateItem(PlantGroup*);
    void initPosture();
    bool tryShake(HitSensor*);
    bool tryPush(const TVec3f&, f32, s32);
    void animControl(PlantGroup*);

    /* 0x90 */ f32 _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ s32 mTouchType;
    /* 0x9C */ bool _9C;
    /* 0x9D */ bool mHasItem;
};
