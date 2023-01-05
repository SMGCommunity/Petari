#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"

namespace NrvNPCActor {
    NERVE(NPCActorNrvWait);
    NERVE(NPCActorNrvTalk);
    NERVE(NPCActorNrvReaction);
};

class NPCActorCaps {
public:
    NPCActorCaps(const char *);

    void setDefault();
    void setIndirect();

    const char* _0;
    bool mInitModel;                // _4
    const char* mObjectName;        // _8
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    const char* _10;
    TVec3f mMessageOffset;          // _14
    MtxPtr mTalkMtx;                // _20
    const char* mTalkJointName;     // _24
    u8 _28;
    bool mConnectsToScene;          // _29
    bool mUseLightCtrl;             // _2A
    bool mUseEffect;                // _2B
    bool mUseSounds;                // _2C
    u8 _2D;
    u8 _2E;
    u8 _2F;
    u32 _30;
    bool mCanBeHitByShell;          // _34
    u8 _35;
    u8 _36;
    bool mUsesNerves;               // _37
    u8 _38;
    u8 _39;
    u8 _3A;
    u8 _3B;
    f32 _3C;
    bool mUseHitSensors;            // _40
    u8 _41;
    u8 _42;
    u8 _43;
    u32 _44;
    f32 mSensorSize;                // _48
    TVec3f mSensorOffset;           // _4C
    u32 _58;
    u8 _5C;
    bool mUseShadow;                // _5D
    u8 _5E;
    u8 _5F;
    f32 _60;
    bool mUseRail;                  // _64
    u8 _65;
    u8 _66;
    u8 _67;
    bool mUseStarPointer;           // _68
    u32 _6C;
    u32 _70;
    TVec3f mStarPointerOffs;        // _74
    f32 _80;
    u32 mSceneConnectionType;       // _84
    NrvNPCActor::NPCActorNrvWait* mWaitNerve;           // _88
    NrvNPCActor::NPCActorNrvTalk* mTalkNerve;           // _8C
    NrvNPCActor::NPCActorNrvReaction* mReactionNerve;   // _90
};

class NPCActor : public LiveActor {
public:
    NPCActor(const char *);

    virtual ~NPCActor() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);
    static void addArchive(NameObjArchiveListCollector *, const NPCActorItem &);
    static void makeArchiveListDefault(NameObjArchiveListCollector *, const JMapInfoIter &);

    void setInitPose();

    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    u32 _9C;
    TVec4f _A0;
    TVec4f _B0;
    TVec3f _C0;
    TVec3f _CC;
    u8 _D8;
    u8 _D9;
    u8 _DA;
    u8 _DB;
    u8 _DC;
    u8 _DD;
    u8 _DE;
    u8 _DF;
    u8 _E0;
    u8 _E1;
    u8 _E2;
    u8 _E3;
    u8 _E4;
    u8 _E5;
    u8 _E6;
    u8 _E7;
    u8 _E8;
    u8 _E9;
    f32 _EC;
    f32 _F0;
    f32 _F4;
    f32 _F8;
    const char* _FC;
    u32 _100;
    u32 _104;
    u32 _108;
    u32 _10C;
    u32 _110;
    u32 _114;
    u32 _118;
    u32 _11C;
    u32 _120;
    u8 _124;
    u8 _125;
    u8 _126;
    u8 _127;
    u8 _128;
    f32 _12C;
    u32 _130;
    u32 _134;
    u32 _138;
    u32 _13C;
    u32 _140;
    u32 _144;
    u32 _148;
    NrvNPCActor::NPCActorNrvWait* mWaitNerve;           // _14C
    NrvNPCActor::NPCActorNrvTalk* mTalkNerve;           // _150
    NrvNPCActor::NPCActorNrvReaction* mReactionNerve;   // _154
    u32 _158;
};

class NPCActorItem {
public:
    NPCActorItem(const char *pName);

    const char* mName;  // _0
    u32* _4;
    u32* _8;
    const char* _C;
    u32* _10;
};