#include "Game/NPC/HoneyQueen.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvHoneyQueen {
    NEW_NERVE(HoneyQueenNrvWait, HoneyQueen, Wait);
    NEW_NERVE(HoneyQueenNrvReady, HoneyQueen, Ready);
    NEW_NERVE(HoneyQueenNrvDemo, HoneyQueen, Demo);
    NEW_NERVE(HoneyQueenNrvFade, HoneyQueen, Fade);
    NEW_NERVE(HoneyQueenNrvTalk, HoneyQueen, Talk);
    NEW_NERVE(HoneyQueenNrvItch, HoneyQueen, Itch);
    NEW_NERVE(HoneyQueenNrvEvent, HoneyQueen, Event);
    NEW_NERVE(HoneyQueenNrvAfter, HoneyQueen, After);
};

HoneyQueen::HoneyQueen(const char *pName) : NPCActor(pName){
    mNpcModel = nullptr;
}

HoneyQueen::~HoneyQueen() {
    
}

void HoneyQueen::init(const JMapInfoIter &rIter) {
    NPCActorCaps caps = "HoneyQueen";
    caps.setDefault();
    caps.mWaitNerve = &NrvHoneyQueen::HoneyQueenNrvWait::sInstance;
    caps.mUseShadow = true;
    caps.mMessageOffset.x = 0.0f;
    caps.mMessageOffset.y = 950.0f;
    caps.mMessageOffset.z = 0.0f;
    caps._44 = "Center";
    caps._C = 0;
    caps._58 = 2;
    NPCActor::initialize(rIter, caps);
    mCenterPart = MR::createCollisionPartsFromLiveActor(this, "Center", getSensor("Body"), MR::getJointMtx(this, "Center"), (MR::CollisionScaleType)2);
    mCenterFurPart = MR::createCollisionPartsFromLiveActor(this, "CenterFur", getSensor("Body"), MR::getJointMtx(this, "Center"), (MR::CollisionScaleType)2);
    mFacePart = MR::createCollisionPartsFromLiveActor(this, "Face", getSensor("Body"), MR::getJointMtx(this, "Face"), (MR::CollisionScaleType)2);
    mLArm01Part = MR::createCollisionPartsFromLiveActor(this, "LArm01", getSensor("Body"), MR::getJointMtx(this, "LArm01"), (MR::CollisionScaleType)2);
    mLArm02Part = MR::createCollisionPartsFromLiveActor(this, "LArm02", getSensor("Body"), MR::getJointMtx(this, "LArm02"), (MR::CollisionScaleType)2);
    mLFoot01Part = MR::createCollisionPartsFromLiveActor(this, "LFoot001", getSensor("Body"), MR::getJointMtx(this, "LFoot001"), (MR::CollisionScaleType)2);
    mRArm01Part = MR::createCollisionPartsFromLiveActor(this, "RArm01", getSensor("Body"), MR::getJointMtx(this, "RArm01"), (MR::CollisionScaleType)2);
    mRArm02Part = MR::createCollisionPartsFromLiveActor(this, "RArm02", getSensor("Body"), MR::getJointMtx(this, "RArm02"), (MR::CollisionScaleType)2);
    mRFoot01Part = MR::createCollisionPartsFromLiveActor(this, "RFoot001", getSensor("Body"), MR::getJointMtx(this, "RFoot001"), (MR::CollisionScaleType)2);
    mTactilePart = MR::createCollisionPartsFromLiveActor(this, "Tactile", getSensor("Body"), MR::getJointMtx(this, "Tactile"), (MR::CollisionScaleType)2);
    MR::excludeCalcShadowToActorAll(this, this);
    mNpcModel = MR::createModelObjNpc("羽", "HoneyQueenWing", MR::getJointMtx(this, "Center"));
    mNpcModel->makeActorAppeared();
    MR::startBck(mNpcModel, "HoneyQueenWing", nullptr);
    MR::startBtk(mNpcModel, "HoneyQueenWing");
    for (s32 i = 0; i < 10; i++) {
        CollisionParts** centerParts = &mCenterPart;
        MR::validateCollisionParts(centerParts[i]);
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionFunctor(this, MR::Functor(this, &HoneyQueen::fadeOut), "フェードアウト");
        MR::registerDemoActionFunctor(this, MR::Functor(this, &HoneyQueen::fadeIn), "フェードイン");
        MR::registerDemoActionFunctor(this, MR::Functor(this, &HoneyQueen::talkEntry), "謁見");
        MR::tryRegisterDemoCast(mNpcModel, rIter);
        _188 = 1;
    }
    else {
        _188 = 0;
    }

    if (_188 == 1) {
        setNerve(&NrvHoneyQueen::HoneyQueenNrvDemo::sInstance);
        MR::listenStageSwitchOnA(this, MR::Functor(this, &HoneyQueen::switchFunc));
        MR::needStageSwitchWriteB(this, rIter);
    }
    else {
        MR::useStageSwitchWriteA(this, rIter);
    }
    mLodCtrl->setDistanceToLow(10000.0f);
    mLodCtrl->setDistanceToMiddle(5000.0f);
    mLodCtrl->setFarClipping(-1.0f);
    mLodCtrl->setClippingTypeSphereContainsModelBoundingBox(100.0f);
    MR::setDistanceToTalk(mMsgCtrl, 0.0f);
    MR::initFur(this);
    MR::initFur(mLodCtrl->_10);
    MR::initFur(mLodCtrl->_14);
    makeActorAppeared();
}

void HoneyQueen::control() {
    NPCActor::control();
    MR::startLevelSound(this, "SE_SM_LV_HONEYQUEEN_FLY", -1, -1, -1);
}

void HoneyQueen::calcAnim() {
    LiveActor::calcAnim();
    for (s32 i = 0; i < 10; i++) {
        CollisionParts** centerParts = &mCenterPart;        
        centerParts[i]->setMtx();
    }
}

void HoneyQueen::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensor(pSender, "TouchJump")) {
            MR::sendMsgTouchJump(pReceiver, pSender);
        }
    }
}

bool HoneyQueen::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg)) {
        return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
    return false;
}

void HoneyQueen::fadeOut() {
    MR::setPlayerStateWait();
    MR::closeWipeFade(-1);
}

void HoneyQueen::fadeIn() {
    MR::openWipeFade(-1);
    setNerve(&NrvHoneyQueen::HoneyQueenNrvFade::sInstance);
}

void HoneyQueen::talkEntry() {
    MR::tryStartBckAndBtp(this, "Talk", nullptr);
    MR::tryTalkTimeKeepDemoMarioPuppetable(mMsgCtrl);
    setNerve(&NrvHoneyQueen::HoneyQueenNrvTalk::sInstance);
}

void HoneyQueen::switchFunc() {
    setNerve(&NrvHoneyQueen::HoneyQueenNrvEvent::sInstance);
}

void HoneyQueen::exeWait() {
    if (MR::tryTalkNearPlayerAtEnd(mMsgCtrl)) {
        setNerve(&NrvHoneyQueen::HoneyQueenNrvReady::sInstance);
    }

    if (MR::isTalkTalking(mMsgCtrl)) {
        MR::tryStartBckAndBtp(this, "Talk", nullptr);
    }
    else {
        MR::tryStartBckAndBtp(this, "Wait", nullptr);
    }
}

void HoneyQueen::exeReady() {
    if (!MR::isNearPlayer(mMsgCtrl, -1.0f)) {
        setNerve(&NrvHoneyQueen::HoneyQueenNrvWait::sInstance);
    }
}

void HoneyQueen::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBckAndBtp(this, "Wait", nullptr);
    }
}

void HoneyQueen::exeFade() {
    NPCActor::turnToPlayer();
}

void HoneyQueen::exeTalk() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBckAndBtp(this, "Talk", nullptr);
    }

    if (MR::isDemoLastStep()) {
        setNerve(&NrvHoneyQueen::HoneyQueenNrvItch::sInstance);
    }
}

void HoneyQueen::exeItch() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBckAndBtp(this, "EventWait", nullptr);        
    }
    TVec3f vec;
    vec.set<f32>(((2.0f * (_B0.x * _B0.z)) + (2.0f * (_B0.h * _B0.y))), (2.0f * (_B0.y * _B0.z)) - (2.0f * (_B0.h * _B0.x)), (1.0f - (2.0f * (_B0.x * _B0.x))) - (2.0f * (_B0.y * _B0.y)));
    MR::faceToVector(&_A0, vec, 0.2f);
    MR::tryTalkNearPlayer(mMsgCtrl);
}

void HoneyQueen::exeEvent() {
    if (MR::tryTalkForceAtEnd(mMsgCtrl)) {
        MR::onSwitchB(this);
        setNerve(&NrvHoneyQueen::HoneyQueenNrvAfter::sInstance);
    }
}

void HoneyQueen::exeAfter() {
    MR::tryTalkNearPlayer(mMsgCtrl);
}
