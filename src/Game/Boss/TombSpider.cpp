#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderAction1st.hpp"
#include "Game/Boss/TombSpiderAction2nd.hpp"
#include "Game/Boss/TombSpiderActionCocoon.hpp"
#include "Game/Boss/TombSpiderDemo.hpp"
#include "Game/Boss/TombSpiderEnvironment.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderParts.hpp"
#include "Game/Boss/TombSpiderSensorCtrl.hpp"
#include "Game/Map/PlanetMap.hpp"

namespace {
    static s32 sSecondBgmStartStep = 180;
}

namespace NrvTombSpider {
    NEW_NERVE(TombSpiderNrvWaitPlayer, TombSpider, WaitPlayer);
    NEW_NERVE(TombSpiderNrvDemoGateOpen, TombSpider, DemoGateOpen);
    NEW_NERVE(TombSpiderNrvActionCocoon, TombSpider, ActionCocoon);
    NEW_NERVE(TombSpiderNrvDemoCocoonBreak, TombSpider, DemoCocoonBreak);
    NEW_NERVE(TombSpiderNrvDemoBattle1stStart, TombSpider, DemoBattle1stStart);
    NEW_NERVE(TombSpiderNrvDemoBattle1stStartJumpToPlayer, TombSpider, DemoBattle1stStartJumpToPlayer);
    NEW_NERVE(TombSpiderNrvAction1st, TombSpider, Action1st);
    NEW_NERVE(TombSpiderNrvDemoBattle1stEnd, TombSpider, DemoBattle1stEnd);
    NEW_NERVE(TombSpiderNrvDemoBattle2ndStart, TombSpider, DemoBattle2ndStart);
    NEW_NERVE(TombSpiderNrvAction2nd, TombSpider, Action2nd);
    NEW_NERVE(TombSpiderNrvDemoDeath, TombSpider, DemoDeath);
    NEW_NERVE(TombSpiderNrvWaitDemo, TombSpider, WaitDemo);
}  // namespace NrvTombSpider

TombSpider::TombSpider(const char* pName)
    : LiveActor(pName), mSensorCtrl(nullptr), mParts(nullptr), mEnvironment(nullptr), mActionCocoon(nullptr), mAction1st(nullptr),
      mAction2nd(nullptr), mDemo(nullptr), mNoctilucaActive(false), mCameraInfo(nullptr) {
}

void TombSpider::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("TombSpider", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initEffectKeeper(8, nullptr, false);

    mParts = new TombSpiderParts(this);
    mParts->init();

    initHitSensor(32);
    mSensorCtrl = new TombSpiderSensorCtrl(this);
    mSensorCtrl->initSensor();

    mEnvironment = new TombSpiderEnvironment(this);

    initSound(8, false);

    MR::needStageSwitchWriteA(this, rIter);
    MR::needStageSwitchWriteB(this, rIter);
    MR::needStageSwitchWriteDead(this, rIter);

    initNerve(&NrvTombSpider::TombSpiderNrvWaitPlayer::sInstance);

    mActionCocoon = new TombSpiderActionCocoon(this);
    mActionCocoon->init();
    mAction1st = new TombSpiderAction1st(this);
    mAction1st->init();
    mAction2nd = new TombSpiderAction2nd(this);
    mAction2nd->init();

    MR::invalidateClipping(this);
    MR::initActorCamera(this, rIter, &mCameraInfo);

    mDemo = new TombSpiderDemo(this);

    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 25);
    makeActorAppeared();
}

void TombSpider::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
    }

    if (MR::getPlayerPos()->distance(mPosition) < 18000.0f) {
        if (!mNoctilucaActive) {
            MR::emitEffect(this, "Noctiluca");
            mNoctilucaActive = true;
        }
    } else {
        if (mNoctilucaActive) {
            MR::deleteEffect(this, "Noctiluca");
            mNoctilucaActive = false;
        }
    }

    if (mDemo->isStartDemoGateOpen()) {
        setNerve(&NrvTombSpider::TombSpiderNrvWaitDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "ゲートオープンデモ[トゥームスパイダー]", &NrvTombSpider::TombSpiderNrvDemoGateOpen::sInstance,
                                            nullptr);
    }
}

void TombSpider::exeDemoGateOpen() {
    if (mDemo->updateGateOpen()) {
        MR::deleteEffect(this, "Noctiluca");
        MR::emitEffect(TombSpiderFunction::getPlanet(this), "Noctiluca");
        setNerve(&NrvTombSpider::TombSpiderNrvActionCocoon::sInstance);
    }
}

void TombSpider::exeActionCocoon() {
    if (MR::updateActorState(this, mActionCocoon)) {
        setNerve(&NrvTombSpider::TombSpiderNrvWaitDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "出現", &NrvTombSpider::TombSpiderNrvDemoCocoonBreak::sInstance, nullptr);
    }
}

void TombSpider::exeDemoCocoonBreak() {
    if (mDemo->updateCocoonBreak()) {
        setNerve(&NrvTombSpider::TombSpiderNrvDemoBattle1stStart::sInstance);
    }
}

void TombSpider::exeDemoBattle1stStart() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(0);
        MR::startBossBGM(4);  // TODO: this is a magic number
    }

    if (mDemo->updateBattle1stStart()) {
        setNerve(&NrvTombSpider::TombSpiderNrvDemoBattle1stStartJumpToPlayer::sInstance);
    }
}

void TombSpider::exeDemoBattle1stStartJumpToPlayer() {
    if (mDemo->updateBattle1stStartJumpToPlayer()) {
        setNerve(&NrvTombSpider::TombSpiderNrvAction1st::sInstance);
    }
}

void TombSpider::exeAction1st() {
    if (MR::updateActorState(this, mAction1st)) {
        setNerve(&NrvTombSpider::TombSpiderNrvWaitDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "１回戦終了", &NrvTombSpider::TombSpiderNrvDemoBattle1stEnd::sInstance, nullptr);
    }
}

void TombSpider::exeDemoBattle1stEnd() {
    if (mDemo->updateBattle1stEnd()) {
        setNerve(&NrvTombSpider::TombSpiderNrvDemoBattle2ndStart::sInstance);
    }
}

void TombSpider::exeDemoBattle2ndStart() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);
    }

    if (MR::isStep(this, sSecondBgmStartStep)) {
        MR::startBossBGM(5);  // TODO: this is a magic number
    }

    if (mDemo->updateBattle2ndStart()) {
        setNerve(&NrvTombSpider::TombSpiderNrvAction2nd::sInstance);
    }
}

void TombSpider::exeAction2nd() {
    if (MR::updateActorState(this, mAction2nd)) {
        setNerve(&NrvTombSpider::TombSpiderNrvWaitDemo::sInstance);
        MR::requestStartDemoMarioPuppetable(this, "死亡", &NrvTombSpider::TombSpiderNrvDemoDeath::sInstance, nullptr);
    }
}

void TombSpider::exeDemoDeath() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(60);
        MR::deleteEffect(this, "Noctiluca");
        MR::emitEffect(TombSpiderFunction::getPlanet(this), "NoctilucaFast");
    }

    mDemo->updateDeath();
}

void TombSpider::exeWaitDemo() {
}

void TombSpider::control() {
    if (!isNerve(&NrvTombSpider::TombSpiderNrvWaitPlayer::sInstance)) {
        TombSpiderFunction::updateAcid(this);
    }
}

void TombSpider::updateHitSensor(HitSensor* pSensor) {
    mSensorCtrl->update(pSensor);
}

void TombSpider::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTombSpider::TombSpiderNrvActionCocoon::sInstance)) {
        mActionCocoon->attackSensor(pSender, pReceiver);
        return;
    }

    if (isNerve(&NrvTombSpider::TombSpiderNrvAction1st::sInstance)) {
        mAction1st->attackSensor(pSender, pReceiver);
        return;
    }

    if (isNerve(&NrvTombSpider::TombSpiderNrvAction2nd::sInstance)) {
        mAction2nd->attackSensor(pSender, pReceiver);
        return;
    }
}

bool TombSpider::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    return false;
}

bool TombSpider::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTombSpider::TombSpiderNrvActionCocoon::sInstance)) {
        return mActionCocoon->receiveOtherMsg(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvTombSpider::TombSpiderNrvAction1st::sInstance)) {
        return mAction1st->receiveOtherMsg(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvTombSpider::TombSpiderNrvAction2nd::sInstance)) {
        return mAction2nd->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}
