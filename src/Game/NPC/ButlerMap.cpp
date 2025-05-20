#include "Game/NPC/ButlerMap.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/types.h"
#include <cstddef>

namespace NrvButlerMap {
    NEW_NERVE(ButlerMapNrvTalk, ButlerMap, Talk);
    NEW_NERVE(ButlerMapNrvShowGalaxyMap, ButlerMap, ShowGalaxyMap);
    NEW_NERVE(ButlerMapNrvLectureDemoShowMapBefore, ButlerMap, LectureDemoShowMapBefore);
    NEW_NERVE(ButlerMapNrvLectureDemoShowMap, ButlerMap, LectureDemoShowMap);
    NEW_NERVE(ButlerMapNrvLectureDemoShowMapAfter, ButlerMap, LectureDemoShowMapAfter);
    NEW_NERVE(ButlerMapNrvStarPieceReaction, ButlerMap, StarPieceReaction);
};

ButlerMap::ButlerMap(const char *pName) : NPCActor(pName){
    _15C = nullptr;
    _160 = false;
}

ButlerMap::~ButlerMap() {
    
}

void ButlerMap::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &_160);
    NPCActorCaps caps("ButlerMap");
    caps.mObjectName = "Butler";
    caps.setDefault();
    caps.mTalkNerve = &NrvButlerMap::ButlerMapNrvTalk::sInstance;
    caps._44 = "Body";
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = 0.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mUseShadow = true;
    caps._38 = false;
    NPCActor::initialize(rIter, caps);
    _130 = "Spin";
    _134 = "Trampled";
    _138 = "Pointing";
    _13C = "Spin";
    mParam._14 = "Wait";
    mParam._18 = "Wait";
    mParam._1C = "Talk";
    mParam._20 = "Talk";
    MR::createSceneObj(119);
    MR::tryRegisterDemoCast(this, rIter);
    MR::tryRegisterDemoCast(this, "バトラー報告", rIter);
    const char* car1 = cDemoNameMapLecture;
    TVec3f vec;
    vec.setPS2(gZeroVec);
    TalkMessageCtrl *talkMessage = MR::createTalkCtrlDirectOnRootNodeAutomatic(this, rIter, "AstroGalaxy_ButlerMap001", vec, MR::getJointMtx(this, "Body"));
    if (MR::tryInitDemoSheetTalkAnim(this, rIter, car1, "DemoButlerMapLecture", talkMessage)) {
        const char* car = cDemoNameMapLecture;
        MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &ButlerMap::startLectureDemo), car1, "開始");
        MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &ButlerMap::resetStatus), car, "バトラーリセット");
    }
    AstroDemoFunction::tryRegisterGrandStarReturnAndSimpleCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "バトラーグリーンドライバ説明", rIter);
    if (_160) {
        SphereSelectorFunction::registerTarget(this);
    }
    _15C = new ButlerStateStarPieceReaction(this, rIter, "AstroGalaxy_Butler000");
    _15C->init();
    if (MR::isButlerMapAppear()) {
        makeActorAppeared();
    }
    else {
        makeActorDead();
    }
}

void ButlerMap::appear() {
    if (MR::isButlerMapAppear()) {
        MR::setDefaultPose(this);
        LiveActor::appear();
        forceNerveToWait();
    }
}

void ButlerMap::kill() {
    if (!MR::isDead(this)) {
        MR::forceDeleteEffectAll(this);
        NPCActor::kill();
    }
}

void ButlerMap::startLectureDemo() {
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(&NrvButlerMap::ButlerMapNrvLectureDemoShowMapBefore::sInstance);
}

void ButlerMap::resetStatus() {
    MR::setDefaultPose(this);
    MR::startBckNoInterpole(this, "Wait");
}

void ButlerMap::control() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_BUTLER_TRAMPLED", -1, -1);
    }
    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_BUTLER_POINT", -1, -1);
    }
    NPCActor::control();
}

bool ButlerMap::receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }
    if (MR::isMsgStarPieceReflect(msg)) {
        return false;
    }
    if (MR::isMsgStarPieceAttack(msg)) {
        if (isNerve(mWaitNerve) || isNerve(&NrvButlerMap::ButlerMapNrvStarPieceReaction::sInstance)) {
            setNerve(&NrvButlerMap::ButlerMapNrvStarPieceReaction::sInstance);
        }
        return true;
    }
    else {
        return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
}

bool ButlerMap::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    if (SphereSelectorFunction::trySyncKillMsgSelectStart(this, msg)) {
        return true;
    }
    else {
        return MR::isMsgHitmarkEmit(msg);
    }
}

void ButlerMap::forceNerveToWait() {
    if (!NPCActor::isEmptyNerve()) {
        NPCActor::popNerve();
    }
    setNerve(mWaitNerve);
}

void ButlerMap::exeShowGalaxyMap() {
    if (MR::isFirstStep(this)) {
        if (_160) {
            MR::startGalaxyMapLayout();
        }
        else {
            MR::startAstroMapLayout();
        }
    }
    else {
        forceNerveToWait();
    }
}

void ButlerMap::exeLectureDemoShowMap() {
    if (MR::isFirstStep(this)) {
        MR::startAstroMapLayoutForNewDomeDiscover();
        MR::forceOpenWipeFade();
    }
    else {
        setNerve(&NrvButlerMap::ButlerMapNrvLectureDemoShowMapAfter::sInstance);
    }
}

void ButlerMap::exeLectureDemoShowMapAfter() {
    if (MR::isFirstStep(this)) {
        MR::resumeTimeKeepDemo(this);
    }
    if (!MR::isTimeKeepDemoActive()) {
        MR::validateClipping(this);
        forceNerveToWait();
    }
}

void ButlerMap::exeStarPieceReaction() {
    if (MR::isFirstStep(this)) {
        _15C->appear();
    }
    
    if (_15C->update()) {
        forceNerveToWait();
    }
}

void ButlerMap::exeTalk() {
    if (MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        setNerve(&NrvButlerMap::ButlerMapNrvShowGalaxyMap::sInstance);
    }
}

void ButlerMap::exeLectureDemoShowMapBefore() {
    if (MR::isDemoPartLastStep("マップ表示開始")) {
        MR::pauseTimeKeepDemo(this);
        setNerve(&NrvButlerMap::ButlerMapNrvLectureDemoShowMap::sInstance);
    }
}
