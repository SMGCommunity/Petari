#include "Game/NPC/HoneyBee.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <cstddef>

namespace NrvHoneyBee {
    NEW_NERVE(HoneyBeeNrvWait, HoneyBee, Wait);
    NEW_NERVE(HoneyBeeNrvReaction, HoneyBee, Reaction);
    NEW_NERVE(HoneyBeeNrvFly, HoneyBee, Fly);
    NEW_NERVE(HoneyBeeNrvDropLecture, HoneyBee, DropLecture);
    NEW_NERVE(HoneyBeeNrvJumpLecture, HoneyBee, JumpLecture);
    NEW_NERVE(HoneyBeeNrvFlyLectureA, HoneyBee, FlyLectureA);
    NEW_NERVE(HoneyBeeNrvFlyLectureB, HoneyBee, FlyLectureB);
};

HoneyBee::HoneyBee(const char *pName) : NPCActor(pName){
    _15C = -1;
    _19C = 0;
    _1A0 = 0.0f;
    _160.identity();
    _190.zero();
}

HoneyBee::~HoneyBee() {
    
}

void HoneyBee::makeArchiveList(NameObjArchiveListCollector *pArchive, const JMapInfoIter &rIter) {
    const char* name;
    MR::getObjectName(&name, rIter);
    NPCActorItem npcItem = name;
    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    if (MR::getNPCItemData(&npcItem, arg)) {
        NPCActor::addArchive(pArchive, npcItem);
    }
}

void HoneyBee::init(const JMapInfoIter &rIter) {
    f32 radius;
    s32 arg;
    NPCActorCaps caps = "HoneyBee";
    caps.setDefault();
    caps._38 = 0;
    caps._6C = "Center";
    caps._44 = "Center";
    caps.mSensorSize = 70.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = -20.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvWait::sInstance;
    caps._70 = "Center";
    arg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    s32 switchTemp = _19C = arg;
    setDefaultsParam(); 
    _128 = 0;
    _12C = 450.0f;
    switch (switchTemp) {
        case 0:
            _128 = 1;
            break;

        case 1:
            caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvJumpLecture::sInstance;
            break;            

        case 2:
            caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFly::sInstance;
            break;

        case 3:
            {mParam.setNoTurnAction("SleepWait");}
            break;

        case 4:
            mParam.setNoTurnAction("StickWait");
            break;

        case 5:
            mParam.setSingleAction("GatekeeperWait");
            _128 = 1;
            break;

        case 6:
            caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvDropLecture::sInstance;
            break;

        case 7:
            mParam.setSingleAction("Flustered");
            _128 = 1;
            break;

        case 8:
            caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFlyLectureA::sInstance;
            caps.mTalkMtx = _160;
            caps.mMessageOffset.zero();
            break;
            
        case 9:
            caps.mWaitNerve = &NrvHoneyBee::HoneyBeeNrvFlyLectureB::sInstance;
            caps.mTalkMtx = _160;
            caps.mMessageOffset.zero();
            break;
            
        default:
            break;
    }

    NPCActor::initialize(rIter, caps);
    NPCActorItem item = ("HoneyBee");
    switch (_19C) {
        case 8:
        case 9:
            radius = 0.0f;
            _160.set(getBaseMtx());
            MR::extractMtxTrans(_160, &_190);
            MR::calcModelBoundingRadius(&radius, this);
            TVec3f vec;
            vec.x = 0.0f;
            vec.y = 0.0f;
            vec.z = 0.0f;
            MR::setShadowDropPositionAtJoint(this, nullptr, "Center", vec);
            MR::onCalcShadow(this, nullptr);
            MR::setClippingTypeSphere(this, radius, &_190);
            mLodCtrl->invalidate();
            break;
    }
    MR::getNPCItemData(&item, _19C);
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
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_HONEYBEE_TRAMPLED", -1, -1);        
    }

    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_HONEYBEE_POINT", -1, -1);       
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_HONEYBEE_SPIN", -1, -1);        
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_HONEYBEE_STAR_PIECE_HIT", -1, -1);       
    }

    NPCActor::control();
    f32 a = _160[1][3] - 80.0f;
    _160.set(MR::getJointMtx(this, "Center"));
    MR::extractMtxTrans(_160, &_190);
    _160[1][3] = 80.0f + (((90.0f * a) + _160[1][3]) / 10.0f);

    if (_19C != 3 && _19C != 4) {
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

    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY", -1, -1, -1);
    if (mMsgCtrl) {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }

    MR::moveCoordAndFollowTrans(this, 5.0f);
    if (_15C != MR::getCurrentRailPointNo(this)) {
        s32 arg = 0;
        s32 railPoint = MR::getCurrentRailPointNo(this);
        _15C = railPoint;
        MR::getRailPointArg0WithInit(this, railPoint, &arg);
        if (!arg) {
            if (!MR::isBckPlaying(this, "WalkWait")) {
                MR::startBck(this, "Wait", nullptr);
            }
        }
        else {
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

        if (MR::isGreaterStep(this, 120)) {
            MR::startBck(this, "HipDropWait", nullptr);
        }
    }
    else if (MR::isBckLooped(this)) {
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

        if (MR::isGreaterStep(this, 120)) {
            MR::startBck(this, "FlyLectureWait", nullptr);
        }
    }
    else if (MR::isBckLooped(this)) {
        setNerve(&NrvHoneyBee::HoneyBeeNrvJumpLecture::sInstance);
    }
}

void HoneyBee::exeFlyLectureA() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::startBck(this, "FlyLectureA", nullptr);
    }
    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY_SOFT", -1, -1, -1);
    MR::tryTalkNearPlayer(mMsgCtrl);
}

void HoneyBee::exeFlyLectureB() {
    if (MR::isFirstStep(this)) {
        MR::onCalcShadow(this, nullptr);
        MR::startBck(this, "FlyLectureB", nullptr);
    }
    MR::startLevelSound(this, "SE_SM_LV_HONEYBEE_FLY_SOFT", -1, -1, -1);
    MR::tryTalkNearPlayer(mMsgCtrl);
}

namespace MR {
    void ActorTalkParam::setNoTurnAction(const char *a) {
        _14 = a;
        _1C = a;
        _18 = 0;        
        _20 = 0;
        _0 = 0;
        _1 = 0;
    }

    void ActorTalkParam::setSingleAction(const char *a) {
        _14 = a;
        _18 = a;
        _1C = a;
        _20 = a;
    }    
};
