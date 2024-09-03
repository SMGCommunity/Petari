#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Util/JointController.hpp"

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
    bool mInitModel;                // 0x4
    const char* mObjectName;        // 0x8
    u8 _C;
    u8 _D;
    u8 _E;
    u8 _F;
    const char* _10;
    TVec3f mMessageOffset;          // 0x14
    MtxPtr mTalkMtx;                // 0x20
    const char* mTalkJointName;     // 0x24
    u8 _28;
    bool mConnectsToScene;          // 0x29
    bool mUseLightCtrl;             // 0x2A
    bool mUseEffect;                // 0x2B
    bool mUseSounds;                // 0x2C
    u8 _2D;
    u8 _2E;
    u8 _2F;
    u32 _30;
    bool mCanBeHitByShell;          // 0x34
    u8 _35;
    u8 _36;
    bool mUsesNerves;               // 0x37
    u8 _38;
    u8 _39;
    u8 _3A;
    u8 _3B;
    f32 _3C;
    bool mUseHitSensors;            // 0x40
    u8 _41;
    u8 _42;
    u8 _43;
    u32 _44;
    f32 mSensorSize;                // 0x48
    TVec3f mSensorOffset;           // 0x4C
    u32 _58;
    u8 _5C;
    bool mUseShadow;                // 0x5D
    u8 _5E;
    u8 _5F;
    f32 _60;
    bool mUseRail;                  // 0x64
    u8 _65;
    u8 _66;
    u8 _67;
    bool mUseStarPointer;           // 0x68
    u32 _6C;
    u32 _70;
    TVec3f mStarPointerOffs;        // 0x74
    f32 _80;
    u32 mSceneConnectionType;       // 0x84
    NrvNPCActor::NPCActorNrvWait* mWaitNerve;           // 0x88
    NrvNPCActor::NPCActorNrvTalk* mTalkNerve;           // 0x8C
    NrvNPCActor::NPCActorNrvReaction* mReactionNerve;   // 0x90
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

    bool initTalkCtrl(const JMapInfoIter &, const char *, const TVec3f &, MtxPtr);
    bool initTalkCtrlDirect(const JMapInfoIter &, const char *, const TVec3f &, MtxPtr);
    bool calcJointScale(TPos3f *, const JointControllerInfo &);
    void turnToPlayer();
    void turnToPlayer(f32);
    void turnToPlayer(f32, f32, f32);
    void turnToDefault(f32);
    void setToDefault();
    void pushNerve(const Nerve *);
    Nerve* popAndPushNerve(const Nerve *);
    Nerve* popNerve();
    bool tryPulNullNerve();
    bool isEmptyNerve() const;
    bool isScaleAnim() const;
    bool isPointingSe() const;
    void updateReaction();
    void updateScaleCtrl();

    void exeWait();
    void exeTalk();

    void setInitPose();

    LodCtrl* mLodCtrl;                                  // 0x8C
    TalkMessageCtrl* mMsgCtrl;                          // 0x90
    PartsModel* _94;
    PartsModel* _98;
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
    const char* _130;
    const char* _134;
    const char* _138;
    const char* _13C;
    AnimScaleController* mScaleController;              // 0x140
    u32 _144;
    Nerve* mCurNerve;                                   // 0x148
    NrvNPCActor::NPCActorNrvWait* mWaitNerve;           // 0x14C
    NrvNPCActor::NPCActorNrvTalk* mTalkNerve;           // 0x150
    NrvNPCActor::NPCActorNrvReaction* mReactionNerve;   // 0x154
    u32 _158;
};

class NPCActorItem {
public:
    NPCActorItem(const char *pName);

    const char* mName;  // 0x0
    u32* _4;
    u32* _8;
    const char* _C;
    u32* _10;
};