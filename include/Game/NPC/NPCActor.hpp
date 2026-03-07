#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/JointController.hpp"

static const char* ReactionDefault = "Reaction";
static const char* PointingDefault = "Pointing";
static const char* TrampledDefault = "Trampled";
static const char* SpinDefault = "Spin";

class NPCActorCaps {
public:
    NPCActorCaps(const char*);

    void setDefault();
    void setIndirect();

    const char* _0;
    bool mModel;              // 0x4
    const char* mObjectName;  // 0x8
    bool mMakeActor;          // 0xC
    bool mHostIO;             // 0xD
    bool mMessage;            // 0xE
    bool _F;
    const char* _10;
    TVec3f mMessageOffset;       // 0x14
    MtxPtr mTalkMtx;             // 0x20
    const char* mTalkJointName;  // 0x24
    bool mInterpole;             // 0x28
    bool mConnectTo;             // 0x29
    bool mLightCtrl;             // 0x2A
    bool mEffect;                // 0x2B
    bool mSound;                 // 0x2C
    s32 mSoundSize;              // 0x30
    bool mAttribute;             // 0x34
    bool mPosition;              // 0x35
    bool mLodCtrl;               // 0x36
    bool mNerve;                 // 0x37
    bool mBinder;                // 0x38
    f32 mBinderSize;             // 0x3C
    bool mSensor;                // 0x40
    const char* mSensorJoint;    // 0x44
    f32 mSensorSize;             // 0x48
    TVec3f mSensorOffset;        // 0x4C
    s32 mSensorMax;              // 0x58
    bool mShadow;                // 0x5C
    u8 _5D;
    u8 _5E;
    u8 _5F;
    f32 mShadowSize;
    bool mRailRider;     // 0x64
    bool mSwitchDead;    // 0x65
    bool mSwitchAppear;  // 0x66
    u8 _67;
    bool mPointer;  // 0x68
    const char* _6C;
    const char* _70;
    TVec3f mStarPointerOffs;   // 0x74
    f32 mPointerSize;          // 0x80
    s32 mSceneConnectionType;  // 0x84
    Nerve* mWaitNerve;         // 0x88
    Nerve* mTalkNerve;         // 0x8C
    Nerve* mReactionNerve;     // 0x90
};

class NPCActor : public LiveActor {
public:
    NPCActor(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    static void addArchive(NameObjArchiveListCollector*, const NPCActorItem&);
    static void makeArchiveListDefault(NameObjArchiveListCollector*, const JMapInfoIter&);

    bool initTalkCtrl(const JMapInfoIter&, const char*, const TVec3f&, MtxPtr);
    bool initTalkCtrlDirect(const JMapInfoIter&, const char*, const TVec3f&, MtxPtr);
    bool calcJointScale(TPos3f*, const JointControllerInfo&);
    bool turnToPlayer();
    bool turnToPlayer(f32);
    bool turnToPlayer(f32, f32, f32);
    bool turnToDefault(f32);
    void setToDefault();
    void pushNerve(const Nerve*);
    const Nerve* popAndPushNerve(const Nerve*);
    const Nerve* popNerve();
    bool tryPullNullNerve();
    bool isEmptyNerve() const;
    bool isScaleAnim() const;
    bool isPointingSe() const;
    void updateReaction();
    void updateScaleCtrl();

    bool tryPushNullNerve();

    void exeReaction();
    void exeWait();
    void exeTalk();
    void exeNull();

    void setInitPose();

    void initialize(const JMapInfoIter&, const NPCActorCaps&);

    void equipment(const NPCActorItem&, bool);

    void setBaseMtx(const TPos3f&);
    void setBaseMtx(MtxPtr);

    inline void setDefaults() {
        _130 = "Spin";
        _134 = "Trampled";
        _138 = "Pointing";
        _13C = "Reaction";
    }

    inline void setDefaultsParam() {
        mParam._14 = "Wait";
        mParam._18 = "Turn";
        mParam._1C = "Talk";
        mParam._20 = "TalkTurn";
        _130 = "Spin";
        _134 = "Trampled";
        _138 = "Pointing";
        _13C = "Reaction";
    }

    inline void setDefaults2() {
        _130 = "Spin";
        _134 = "Trampled";
        _138 = "Pointing";
    }

    LodCtrl* mLodCtrl;          // 0x8C
    TalkMessageCtrl* mMsgCtrl;  // 0x90
    PartsModel* _94;
    PartsModel* _98;
    s32 _9C;
    TQuat4f _A0;
    TQuat4f _B0;
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
    MR::ActorTalkParam mParam;  // 0xE8
    f32 _10C;
    f32 _110;
    f32 _114;
    f32 _118;
    const char* _11C;
    const char* _120;
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
    AnimScaleController* mScaleController;          // 0x140
    JointControlDelegator< NPCActor >* mDelegator;  // 0x144
    const Nerve* mCurNerve;                         // 0x148
    Nerve* mWaitNerve;                              // 0x14C
    Nerve* mTalkNerve;                              // 0x150
    Nerve* mReactionNerve;                          // 0x154
    s32 _158;
};
