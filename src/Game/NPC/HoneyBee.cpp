#include "Game/NPC/HoneyBee.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace {
    static const s32 sFlyLectureWaitStep = 120;
    static const s32 sHipLectureWaitStep = 120;
    static const f32 sFlyMoveSpeed = 5.0f;
    // static const f32 sMessageOffsetY = _;
};  // namespace

namespace NrvHoneyBee {
    NEW_NERVE(HoneyBeeNrvWait, HoneyBee, Wait);
    NEW_NERVE(HoneyBeeNrvReaction, HoneyBee, Reaction);
    NEW_NERVE(HoneyBeeNrvFly, HoneyBee, Fly);
    NEW_NERVE(HoneyBeeNrvDropLecture, HoneyBee, DropLecture);
    NEW_NERVE(HoneyBeeNrvJumpLecture, HoneyBee, JumpLecture);
    NEW_NERVE(HoneyBeeNrvFlyLectureA, HoneyBee, FlyLectureA);
    NEW_NERVE(HoneyBeeNrvFlyLectureB, HoneyBee, FlyLectureB);
};  // namespace NrvHoneyBee

HoneyBee::HoneyBee(const char* pName) : NPCActor(pName), _15C(-1), mArg0(), _1A0() {
    _160.identity();
    _190.zero();
}

void HoneyBee::makeArchiveList(NameObjArchiveListCollector* pArchive, const JMapInfoIter& rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);
    NPCActorItem npcItem = objName;

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (MR::getNPCItemData(&npcItem, arg0)) {
        NPCActor::addArchive(pArchive, npcItem);
    }
}

void HoneyBee::init(const JMapInfoIter& rIter) {
    f32 radius;
    NPCActorCaps caps = "HoneyBee";
    caps.setDefault();
    caps.mBinder = false;
    caps._6C = "Center";
    caps.mSensorJoint = "Center";
    caps.mSensorSize = 70.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = -20.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvWait::sInstance;
    caps._70 = "Center";

    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    mArg0 = arg0;

    setDefaultsParam();
    _128 = 0;
    _12C = 450.0f;

    switch (mArg0) {
    case InitAction_Wait:
        _128 = 1;
        break;
    case InitAction_JumpLecture:
        caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvJumpLecture::sInstance;
        break;
    case InitAction_Fly:
        caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFly::sInstance;
        break;
    case InitAction_Sleep:
        mParam.setNoTurnAction("SleepWait");
        break;
    case InitAction_Stick:
        mParam.setNoTurnAction("StickWait");
        break;
    case InitAction_Gatekeeper:
        mParam.setSingleAction("GatekeeperWait");
        _128 = 1;
        break;
    case InitAction_DropLecture:
        caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvDropLecture::sInstance;
        break;
    case InitAction_Flustered:
        mParam.setSingleAction("Flustered");
        _128 = 1;
        break;
    case InitAction_FlyLectureA:
        caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFlyLectureA::sInstance;
        caps.mTalkMtx = _160;
        caps.mMessageOffset.zero();
        break;
    case InitAction_FlyLectureB:
        caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFlyLectureB::sInstance;
        caps.mTalkMtx = _160;
        caps.mMessageOffset.zero();
        break;
    default:
        break;
    }

    NPCActor::initialize(rIter, caps);
    NPCActorItem item = ("HoneyBee");

    switch (mArg0) {
    case InitAction_FlyLectureA:
    case InitAction_FlyLectureB:
        radius = 0.0f;
        _160.set(getBaseMtx());
        MR::extractMtxTrans(_160, &_190);
        MR::calcModelBoundingRadius(&radius, this);
        MR::setShadowDropPositionAtJoint(this, nullptr, "Center", TVec3f(0.0f, 0.0f, 0.0f));
        MR::onCalcShadow(this, nullptr);
        MR::setClippingTypeSphere(this, radius, &_190);
        mLodCtrl->invalidate();
        break;
    }

    MR::getNPCItemData(&item, mArg0);
    NPCActor::equipment(item, false);
    MR::initFur(this);
    MR::initFur(mLodCtrl->_10);
    MR::initFur(mLodCtrl->_14);
}

void HoneyBee::calcAndSetBaseMtx() {
    MR::calcAndSetFloatBaseMtx(this, _1A0);
}

void HoneyBee::control() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_HONEYBEE_TRAMPLED");
    }

    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_HONEYBEE_POINT");
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_HONEYBEE_SPIN");
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_HONEYBEE_STAR_PIECE_HIT");
    }

    NPCActor::control();
    f32 a = _160[1][3] - 80.0f;
    _160.set(MR::getJointMtx(this, "Center"));
    MR::extractMtxTrans(_160, &_190);
    _160[1][3] = 80.0f + (((9.0f * a) + _160[1][3]) / 10.0f);

    if (mArg0 != InitAction_Sleep && mArg0 != InitAction_Stick) {
        _1A0 = MR::calcFloatOffset(this, _1A0, 120.0f);
    }
}

void HoneyBee::exeWait() {
    if (!MR::tryStartReactionAndPushNerve(this, &NrvHoneyBee::HoneyBeeNrvReaction::sInstance)) {
        MR::tryTalkNearPlayerAndStartMoveTalkAction(this);
    }
}

void HoneyBee::exeReaction() {
    MR::tryStartReactionAndPopNerve(this);
}

void HoneyBee::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
    }

    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY");

    if (mMsgCtrl != nullptr) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }

    MR::moveCoordAndFollowTrans(this, ::sFlyMoveSpeed);

    if (_15C != MR::getCurrentRailPointNo(this)) {
        s32 arg0 = 0;
        _15C = MR::getCurrentRailPointNo(this);
        MR::getRailPointArg0WithInit(this, _15C, &arg0);

        if (arg0 == 0) {
            if (!MR::isBckPlaying(this, "WalkWait")) {
                MR::startBck(this, "Wait", nullptr);
            }
        } else {
            if (!MR::isBckPlaying(this, "FlyWait")) {
                MR::startBck(this, "FlyWait", nullptr);
            }
        }
    }
}

void HoneyBee::exeDropLecture() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isBckPlaying(this, "Wait")) {
        if (mMsgCtrl && MR::tryTalkNearPlayer(mMsgCtrl)) {
            NPCActor::turnToPlayer();
        }

        if (MR::isGreaterStep(this, ::sHipLectureWaitStep)) {
            MR::startBck(this, "HipDropWait", nullptr);
        }
    } else if (MR::isBckLooped(this)) {
        setNerve(&NrvHoneyBee::HoneyBeeNrvDropLecture::sInstance);
    }
}

void HoneyBee::exeJumpLecture() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isBckPlaying(this, "Wait")) {
        if (mMsgCtrl && MR::tryTalkNearPlayer(mMsgCtrl)) {
            NPCActor::turnToPlayer();
        }

        if (MR::isGreaterStep(this, ::sFlyLectureWaitStep)) {
            MR::startBck(this, "FlyLectureWait", nullptr);
        }
    } else if (MR::isBckLooped(this)) {
        setNerve(&NrvHoneyBee::HoneyBeeNrvJumpLecture::sInstance);
    }
}

void HoneyBee::exeFlyLectureA() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::startBck(this, "FlyLectureA", nullptr);
    }

    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY_SOFT");
    MR::tryTalkNearPlayer(mMsgCtrl);
}

void HoneyBee::exeFlyLectureB() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::startBck(this, "FlyLectureB", nullptr);
    }

    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY_SOFT");
    MR::tryTalkNearPlayer(mMsgCtrl);
}
