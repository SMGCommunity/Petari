#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AnimScaleController;
class JointController;
class JointControllerInfo;
class LodCtrl;
class NameObjArchiveListCollector;
class NPCActorItem;
class PartsModel;
class TalkMessageCtrl;

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
    /* 0x4 */ bool mModel;
    /* 0x8 */ const char* mObjectName;
    /* 0xC */ bool mMakeActor;
    /* 0xD */ bool mHostIO;
    /* 0xE */ bool mMessage;
    bool _F;
    const char* _10;
    /* 0x14 */ TVec3f mMessageOffset;
    /* 0x20 */ MtxPtr mTalkMtx;
    /* 0x24 */ const char* mTalkJointName;
    /* 0x28 */ bool mInterpole;
    /* 0x29 */ bool mConnectTo;
    /* 0x2A */ bool mLightCtrl;
    /* 0x2B */ bool mEffect;
    /* 0x2C */ bool mSound;
    /* 0x30 */ s32 mSoundSize;
    /* 0x34 */ bool mAttribute;
    /* 0x35 */ bool mPosition;
    /* 0x36 */ bool mLodCtrl;
    /* 0x37 */ bool mNerve;
    /* 0x38 */ bool mBinder;
    /* 0x3C */ f32 mBinderSize;
    /* 0x40 */ bool mSensor;
    /* 0x44 */ const char* mSensorJoint;
    /* 0x48 */ f32 mSensorSize;
    /* 0x4C */ TVec3f mSensorOffset;
    /* 0x58 */ s32 mSensorMax;
    /* 0x5C */ bool mShadow;
    /* 0x5D */ bool mUseShadow;
    u8 _5E;
    u8 _5F;
    f32 mShadowSize;
    /* 0x64 */ bool mRailRider;
    /* 0x65 */ bool mSwitchDead;
    /* 0x66 */ bool mSwitchAppear;
    u8 _67;
    /* 0x68 */ bool mPointer;
    const char* _6C;
    const char* _70;
    /* 0x74 */ TVec3f mStarPointerOffs;
    /* 0x80 */ f32 mPointerSize;
    /* 0x84 */ s32 mSceneConnectionType;
    /* 0x88 */ Nerve* mWaitNerve;
    /* 0x8C */ Nerve* mTalkNerve;
    /* 0x90 */ Nerve* mReactionNerve;
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

    bool isTrampledStart() const {
        return !_DD && _E2;
    }

    bool isReactionStart() const {
        return !_E0 && _E5;
    }

    bool isSpinAttackedStart() const {
        return !_DE && _E3;
    }

    bool isPointingStart() const {
        return !_DF && _E4;
    }

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

    inline void setDefaults(const char* pReaction, const char* pPointing, const char* pTrampled, const char* pSpin) {
        _130 = pSpin;
        _134 = pTrampled;
        _138 = pPointing;
        _13C = pReaction;
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

    inline void setTalkAction(const char* pActionName) {
        _11C = pActionName;
        _120 = pActionName;
    }

    TalkMessageCtrl* getMsgCtrl() const {
        return mMsgCtrl;
    }

    /* 0x8C */ LodCtrl* mLodCtrl;
    /* 0x90 */ TalkMessageCtrl* mMsgCtrl;
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
    /* 0xE8 */ MR::ActorTalkParam mParam;
    f32 _10C;
    f32 _110;
    f32 _114;
    f32 _118;
    const char* _11C;
    const char* _120;
    u8 _124;  // follow rail on ground
    u8 _125;
    u8 _126;
    u8 _127;
    u8 _128;
    f32 _12C;
    const char* _130;
    const char* _134;
    const char* _138;
    const char* _13C;
    /* 0x140 */ AnimScaleController* mScaleController;
    /* 0x144 */ JointController* mDelegator;
    /* 0x148 */ const Nerve* mCurNerve;
    /* 0x14C */ Nerve* mWaitNerve;
    /* 0x150 */ Nerve* mTalkNerve;
    /* 0x154 */ Nerve* mReactionNerve;
    s32 _158;
};
