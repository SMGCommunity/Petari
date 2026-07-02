#include "Game/Boss/DodoryuDemo.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const s32 sOpeningHideFrame = 60;
    // static const s32 sOpeningAppearFrame = _;
    // static const s32 sOpeningAppearRumbleStep = _;
    // static const s32 sOpeningHideRumbleStep = _;
    static const s32 sOpeningAppearSmileSoundStep = 55;
    // static const s32 sAppearRumbleStep = _;
    // static const s32 sDownRumbleStep = _;
    static const s32 sDieSoundStep = 242;
};  // namespace

namespace {
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningDemoWait, DodoryuDemoOpening, DemoWait);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningUnderground, DodoryuDemoOpening, Underground);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningUndergroundHide, DodoryuDemoOpening, UndergroundHide);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningAppear, DodoryuDemoOpening, Appear);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningWalk, DodoryuDemoOpening, Walk);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningHideReady, DodoryuDemoOpening, HideReady);
    NEW_NERVE(DodoryuDemoOpeningNrvOpeningHide, DodoryuDemoOpening, Hide);
    NEW_NERVE(DodoryuDemoAppearNrvAppearDemoWait, DodoryuDemoAppear, DemoWait);
    NEW_NERVE(DodoryuDemoAppearNrvAppearPlay, DodoryuDemoAppear, Play);
    NEW_NERVE(DodoryuDemoAngryNrvAngryDemoWait, DodoryuDemoAngry, DemoWait);
    NEW_NERVE(DodoryuDemoAngryNrvAngryPlay, DodoryuDemoAngry, Play);
    NEW_NERVE(DodoryuDemoDownNrvDownDemoWait, DodoryuDemoDown, DemoWait);
    NEW_NERVE(DodoryuDemoDownNrvDownPlay, DodoryuDemoDown, Play);
};  // namespace

namespace {
    void resetCastDisposition(Dodoryu* pDodoryu) {
        TPos3f mtx;

        pDodoryu->killAllHillPieces();
        MR::findNamePos("ドドリュウ再セット", mtx.toMtxPtr());
        pDodoryu->setMtx(mtx);
        pDodoryu->snapToGround();

        MR::findNamePos("マリオ再セット", mtx.toMtxPtr());
        MR::setPlayerBaseMtx(mtx.toMtxPtr());
        MR::setPlayerStateWait();
    }
};  // namespace

DodoryuDemoOpening::DodoryuDemoOpening(Dodoryu* pHost, const char* pName) : DodoryuStateBase(pHost, pName), mActorCameraInfo() {
    initWithoutIter();
}

void DodoryuDemoOpening::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuDemoOpeningNrvOpeningUnderground::sInstance);
    MR::initAnimCamera(mHost, &mActorCameraInfo, "OpeningDemo");
    makeActorAppeared();
}

void DodoryuDemoOpening::control() {
    if (tryFinish()) {
        return;
    }
}

void DodoryuDemoOpening::start() {
    setNerve(&::DodoryuDemoOpeningNrvOpeningDemoWait::sInstance);
}

void DodoryuDemoOpening::end() {
    MR::endAnimCamera(mHost, &mActorCameraInfo, "OpeningDemo", -1, true);
    MR::endDemo(mHost, "ドドリュウ開始デモ");

    TPos3f playerResetMtx;
    mHost->getPlayerResetMtx(&playerResetMtx, 0);
    mHost->_150 = false;

    MR::setPlayerBaseMtx(playerResetMtx.toMtxPtr());
    MR::setPlayerStateWait();
    MR::showPlayer();
}

void DodoryuDemoOpening::exeDemoWait() {
    if (!MR::tryStartDemo(mHost, "ドドリュウ開始デモ")) {
        return;
    }

    MR::invalidateShadowAll(mHost);
    MR::startSystemSE("SE_DM_MEET_BOSS");
    MR::startStageBGM("BGM_PINCH_3", false);
    MR::startAnimCameraTargetOther(mHost, &mActorCameraInfo, "OpeningDemo", CameraTargetArg(mHost->_148), 0, 1.0f);
    mHost->pauseOff();
    MR::hidePlayer();
    MR::startBck(mHost, "EffectWalk", nullptr);
    mHost->startLeadHillBck("EffectWalk");
    MR::startBva(mHost, "Normal");
    MR::requestMovementOnImageEffect();
    MR::setImageEffectControlAuto();
    mHost->shiftMoveStateRail(10.0f);
    mHost->killAllHillPieces();
    mHost->stopHillForce();
    MR::moveCoordToStartPos(mHost);
    mHost->resetRabbit();
    setNerve(&::DodoryuDemoOpeningNrvOpeningUnderground::sInstance);
}

void DodoryuDemoOpening::exeUnderground() {
    if (MR::isFirstStep(this)) {
        mHost->startHill();

        Dodoryu* pHost = mHost;
        pHost->_150 = true;
        pHost->_154 = 0;
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_U_SHALLOW");
    turnToRabbit();

    if (MR::isGreaterEqualStep(this, ::sOpeningHideFrame)) {
        setNerve(&::DodoryuDemoOpeningNrvOpeningUndergroundHide::sInstance);
    }
}

void DodoryuDemoOpening::exeUndergroundHide() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "EffectWalkHide", nullptr);
        mHost->startLeadHillBck("EffectWalkHide");
    }

    turnToRabbit();

    if (MR::isBckStopped(mHost)) {
        setNerve(&::DodoryuDemoOpeningNrvOpeningAppear::sInstance);
    }
}

void DodoryuDemoOpening::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "OpeningAppear", nullptr);
        MR::startBtk(mHost, "OpeningAppear");
        mHost->startLeadHillBck("Appear");
        MR::startSound(mHost, "SE_BV_DODORYU_APPEAR_1");
        MR::startSound(mHost, "SE_BM_DODORYU_APPEAR_HEAD");
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_GROUND");

    if (MR::isStep(this, ::sOpeningAppearSmileSoundStep)) {
        MR::startSound(mHost, "SE_BV_DODORYU_FAINT_SMILE");
    }

    turnToRabbit();

    if (MR::isBckStopped(mHost)) {
        setNerve(&::DodoryuDemoOpeningNrvOpeningWalk::sInstance);
    }
}

void DodoryuDemoOpening::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "OpeningWalk", nullptr);
        MR::startBtk(mHost, "OpeningWalk");
        mHost->startLeadHillBck("Walk");
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_GROUND");
    turnToRabbit();

    if (MR::isGreaterEqualStep(this, 60)) {
        mHost->_150 = false;

        setNerve(&::DodoryuDemoOpeningNrvOpeningHideReady::sInstance);
    }
}

void DodoryuDemoOpening::exeHideReady() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "Hide", nullptr);
        mHost->startLeadHillBck("Walk");
    }

    MR::startLevelSound(mHost, "SE_BM_LV_DODORYU_MOVE_GROUND");
    turnToRabbit();

    if (MR::isBckStopped(mHost)) {
        MR::startBck(mHost, "EffectWalkAppear", nullptr);
        setNerve(&::DodoryuDemoOpeningNrvOpeningHide::sInstance);
    }
}

void DodoryuDemoOpening::exeHide() {
    turnToRabbit();

    if (MR::isBckOneTimeAndStopped(mHost)) {
        MR::startBck(mHost, "EffectWalk", nullptr);

        Dodoryu* pHost = mHost;
        pHost->_150 = true;
        pHost->_154 = 0;
        mHost->startLeadHillBck("EffectWalk");
    }
}

bool DodoryuDemoOpening::tryFinish() {
    if (!isNerve(&::DodoryuDemoOpeningNrvOpeningDemoWait::sInstance) && MR::isAnimCameraEnd(mHost, &mActorCameraInfo, "OpeningDemo")) {
        mHost->displayRabbitMessage();
        mHost->nextState();

        return true;
    }

    return false;
}

// DodoryuDemoOpening::turnToRabbit

DodoryuDemoAppear::DodoryuDemoAppear(Dodoryu* pHost, const char* pName) : DodoryuStateBase(pHost, pName), mActorCameraInfo() {
    initWithoutIter();
}

void DodoryuDemoAppear::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuDemoAppearNrvAppearPlay::sInstance);
    MR::initAnimCamera(mHost, &mActorCameraInfo, "AppearDemo");
    makeActorAppeared();
}

void DodoryuDemoAppear::start() {
    setNerve(&::DodoryuDemoAppearNrvAppearDemoWait::sInstance);
}

void DodoryuDemoAppear::end() {
    MR::endAnimCamera(mHost, &mActorCameraInfo, "AppearDemo", -1, true);
    MR::endDemo(mHost, "ドドリュウ出現デモ");
    MR::showPlayer();
}

void DodoryuDemoAppear::exeDemoWait() {
    if (!MR::tryStartDemo(mHost, "ドドリュウ出現デモ")) {
        return;
    }

    MR::startAnimCameraTargetSelf(mHost, &mActorCameraInfo, "AppearDemo", 0, 1.0f);
    MR::stopStageBGM(60);
    MR::invalidateShadowAll(mHost);
    mHost->leaveRabbit();
    MR::requestMovementOnImageEffect();
    MR::setImageEffectControlAuto();
    ::resetCastDisposition(mHost);
    setNerve(&::DodoryuDemoAppearNrvAppearPlay::sInstance);
}

void DodoryuDemoAppear::exePlay() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "AppearDemo", nullptr);
        MR::startBtp(mHost, "AppearDemo");
        MR::startBtk(mHost, "AppearDemo");
        MR::startBrk(mHost, "Normal");
        MR::startBva(mHost, "Normal");
        MR::hidePlayer();
    }

    if (MR::isBckStopped(mHost)) {
        MR::startBtp(mHost, "Normal");
        mHost->nextState();
        MR::startBossBGM(MR::BossBgmID_DodoryuA);

        Dodoryu* pHost = mHost;
        TVec3f jointPos;
        MR::copyJointPos(mHost, "Nose", &jointPos);
        MR::appearStarPiece(pHost, jointPos, 8, 20.0f, 40.0f, false);

        MR::startSound(mHost, "SE_OJ_STAR_PIECE_BURST");
    }
}

DodoryuDemoAngry::DodoryuDemoAngry(Dodoryu* pHost, const char* pName) : DodoryuStateBase(pHost, pName), mActorCameraInfo() {
    initWithoutIter();
}

void DodoryuDemoAngry::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuDemoAngryNrvAngryPlay::sInstance);
    MR::initAnimCamera(mHost, &mActorCameraInfo, "AngryDemo");
    makeActorAppeared();
}

void DodoryuDemoAngry::start() {
    setNerve(&::DodoryuDemoAngryNrvAngryDemoWait::sInstance);
}

void DodoryuDemoAngry::end() {
    MR::endAnimCamera(mHost, &mActorCameraInfo, "AngryDemo", -1, true);
    MR::endDemo(mHost, "ドドリュウ怒りデモ");
    MR::showPlayer();
}

void DodoryuDemoAngry::exeDemoWait() {
    if (!MR::tryStartDemo(mHost, "ドドリュウ怒りデモ")) {
        return;
    }

    MR::invalidateShadowAll(mHost);
    ::resetCastDisposition(mHost);
    MR::requestMovementOnImageEffect();
    MR::setImageEffectControlAuto();
    MR::startAnimCameraTargetSelf(mHost, &mActorCameraInfo, "AngryDemo", 0, 1.0f);
    setNerve(&::DodoryuDemoAngryNrvAngryPlay::sInstance);
}

void DodoryuDemoAngry::exePlay() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "AngryDemo", nullptr);
        MR::startBrk(mHost, "AngryStart");
        MR::startBva(mHost, "Angry");
        MR::stopStageBGM(60);
        MR::hidePlayer();
    }

    if (MR::isStep(this, 120)) {
        MR::startCenterScreenBlur(60, 15.0f, 80, 5, 30);
    }

    if (MR::isBckStopped(mHost)) {
        mHost->nextState();
        MR::startBossBGM(MR::BossBgmID_DodoryuB);

        Dodoryu* pHost = mHost;
        TVec3f jointPos;
        MR::copyJointPos(mHost, "Nose", &jointPos);
        MR::appearStarPiece(pHost, jointPos, 8, 20.0f, 40.0f, false);

        MR::startSound(mHost, "SE_OJ_STAR_PIECE_BURST");
    }
}

DodoryuDemoDown::DodoryuDemoDown(Dodoryu* pHost, const char* pName) : DodoryuStateBase(pHost, pName), mActorCameraInfo() {
    initWithoutIter();
}

void DodoryuDemoDown::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    initNerve(&::DodoryuDemoDownNrvDownPlay::sInstance);
    MR::initAnimCamera(mHost, &mActorCameraInfo, "DownDemo");
    makeActorAppeared();
}

void DodoryuDemoDown::start() {
    setNerve(&::DodoryuDemoDownNrvDownDemoWait::sInstance);
}

void DodoryuDemoDown::end() {
    MR::endAnimCamera(mHost, &mActorCameraInfo, "DownDemo", -1, true);
    MR::endDemo(mHost, "ドドリュウ死亡デモ");
    MR::showPlayer();
}

void DodoryuDemoDown::exeDemoWait() {
    if (!MR::tryStartDemo(mHost, "ドドリュウ死亡デモ")) {
        return;
    }

    MR::startAnimCameraTargetSelf(mHost, &mActorCameraInfo, "DownDemo", 0, 1.0f);
    MR::invalidateShadowAll(mHost);
    MR::requestMovementOnImageEffect();
    MR::setImageEffectControlAuto();
    ::resetCastDisposition(mHost);
    setNerve(&::DodoryuDemoDownNrvDownPlay::sInstance);
}

void DodoryuDemoDown::exePlay() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "DownDemo", nullptr);
        MR::startBrk(mHost, "Normal");
        MR::startBva(mHost, "Cry");
        MR::stopStageBGM(30);
        MR::hidePlayer();
    }

    if (MR::isStep(this, ::sDieSoundStep)) {
        MR::startSound(mHost, "SE_BM_DODORYU_DIE");
    }

    if (MR::isBckStopped(mHost)) {
        MR::hideModel(mHost);
    }

    if (MR::isAnimCameraEnd(mHost, &mActorCameraInfo, "DownDemo")) {
        mHost->nextState();
    }
}
