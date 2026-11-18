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

// TODO should this be somewhere else?
namespace MR {
    class ActorReactionParam {
    public:
        ActorReactionParam(){};

        inline void setDefaultAction() {
            _0 = "Spin";
            _4 = "Trampled";
            _8 = "Pointing";
            _C = "Reaction";
        }

        /* 0x0 */ const char* _0;
        /* 0x4 */ const char* _4;
        /* 0x8 */ const char* _8;
        /* 0xC */ const char* _C;
    };
};  // namespace MR

class NPCActorCaps {
public:
    NPCActorCaps(const char*);

    void setDefault();
    void setIndirect();

    /* 0x00 */ const char* _0;
    /* 0x04 */ bool mModel;
    /* 0x08 */ const char* mObjectName;
    /* 0x0C */ bool mMakeActor;
    /* 0x0D */ bool mHostIO;
    /* 0x0E */ bool mMessage;
    /* 0x0F */ bool _F;
    /* 0x10 */ const char* _10;
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
    /* 0x60 */ f32 mShadowSize;
    /* 0x64 */ bool mRailRider;
    /* 0x65 */ bool mSwitchDead;
    /* 0x66 */ bool mSwitchAppear;
    /* 0x67 */ bool _67;
    /* 0x68 */ bool mPointer;
    /* 0x6C */ const char* _6C;
    /* 0x70 */ const char* _70;
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

    inline void setDefaults(const char* pReaction, const char* pPointing, const char* pTrampled, const char* pSpin) {
        mReactionParam._0 = pSpin;
        mReactionParam._4 = pTrampled;
        mReactionParam._8 = pPointing;
        mReactionParam._C = pReaction;
    }

    inline void setDefaults2(const char* pReaction, const char* pPointing, const char* pSpin, const char* pTrampled) {
        mReactionParam._4 = pTrampled;
        mReactionParam._0 = pSpin;
        mReactionParam._8 = pPointing;
        mReactionParam._C = pReaction;
    }

    inline void setDefaults3(const char* pPointing, const char* pReaction, const char* pSpin, const char* pTrampled) {
        mReactionParam._4 = pTrampled;
        mReactionParam._0 = pSpin;
        mReactionParam._C = pReaction;
        mReactionParam._8 = pPointing;
    }

    inline void setDefaults4(const char* a1, const char* a2, const char* a3, const char* a4) {
        mReactionParam._0 = a4;
        mReactionParam._C = a2;
        mReactionParam._4 = a3;
        mReactionParam._8 = a1;
    }

    inline void setDefaultsParam() {
        mParam._14 = "Wait";
        mParam._18 = "Turn";
        mParam._1C = "Talk";
        mParam._20 = "TalkTurn";

        mReactionParam._0 = "Spin";
        mReactionParam._4 = "Trampled";
        mReactionParam._8 = "Pointing";
        mReactionParam._C = "Reaction";
    }

    inline void setDefaults2() {
        mReactionParam._0 = "Spin";
        mReactionParam._4 = "Trampled";
        mReactionParam._8 = "Pointing";
    }

    inline void setTalkAction(const char* pActionName) {
        _11C = pActionName;
        _120 = pActionName;
    }

    inline void setTalkAction(const char* pActionName, const char* pActionName2) {
        _11C = pActionName;
        _120 = pActionName2;
    }

    TalkMessageCtrl* getMsgCtrl() const {
        return mMsgCtrl;
    }

    /* 0x08C */ LodCtrl* mLodCtrl;
    /* 0x090 */ TalkMessageCtrl* mMsgCtrl;
    /* 0x094 */ PartsModel* _94;
    /* 0x098 */ PartsModel* _98;
    /* 0x09C */ s32 _9C;
    /* 0x0A0 */ TQuat4f _A0;
    /* 0x0B0 */ TQuat4f _B0;
    /* 0x0C0 */ TVec3f _C0;
    /* 0x0CC */ TVec3f _CC;
    /* 0x0D8 */ bool _D8;
    /* 0x0D9 */ bool _D9;
    /* 0x0DA */ bool _DA;
    /* 0x0DB */ bool _DB;
    /* 0x0DC */ bool _DC;
    /* 0x0DD */ bool _DD;
    /* 0x0DE */ bool _DE;
    /* 0x0DF */ bool _DF;
    /* 0x0E0 */ bool _E0;
    /* 0x0E1 */ bool _E1;
    /* 0x0E2 */ bool _E2;
    /* 0x0E3 */ bool _E3;
    /* 0x0E4 */ bool _E4;
    /* 0x0E5 */ bool _E5;
    /* 0x0E6 */ bool _E6;
    /* 0x0E7 */ bool _E7;
    /* 0x0E8 */ MR::ActorTalkParam mParam;
    /* 0x10C */ f32 _10C;
    /* 0x110 */ f32 _110;
    /* 0x114 */ f32 _114;
    /* 0x118 */ f32 _118;
    /* 0x11C */ const char* _11C;
    /* 0x120 */ const char* _120;
    /* 0x124 */ bool _124;  // follow rail on ground
    /* 0x125 */ bool _125;
    /* 0x126 */ bool _126;
    /* 0x127 */ bool _127;
    /* 0x128 */ bool _128;
    /* 0x12C */ f32 _12C;
    /* 0x130 */ MR::ActorReactionParam mReactionParam;
    /* 0x140 */ AnimScaleController* mScaleController;
    /* 0x144 */ JointController* mDelegator;
    /* 0x148 */ const Nerve* mCurNerve;
    /* 0x14C */ Nerve* mWaitNerve;
    /* 0x150 */ Nerve* mTalkNerve;
    /* 0x154 */ Nerve* mReactionNerve;
    /* 0x158 */ s32 _158;
};
