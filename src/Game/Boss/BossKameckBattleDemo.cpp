#include "Game/Boss/BossKameckBattleDemo.hpp"
#include "Game/Boss/BossKameck.hpp"

BossKameckDemoPosition::BossKameckDemoPosition() : LiveActor("キャスト位") {
    makeActorDead();
}

void BossKameckDemoPosition::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("BossKameckBattleDemo", nullptr, false);

    mCameraInfo = MR::createActorCameraInfo(rIter);

    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckStart");
    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckPowerUp");
    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckDown");
    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckStart2");
    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckPowerUp2");
    MR::initAnimCamera(this, mCameraInfo, "DemoBossKameckDown2");
    MR::invalidateClipping(this);
    makeActorDead();
}

namespace NrvBossKamecBattleDemo {
    NEW_NERVE(BossKameckBattleDemoNrvTryStartDemo, BossKameckBattleDemo, TryStartDemo);
    NEW_NERVE(BossKameckBattleDemoNrvWaitAppearVs1, BossKameckBattleDemo, WaitAppearVs1);
    NEW_NERVE(BossKameckBattleDemoNrvAppearVs1, BossKameckBattleDemo, AppearVs1);
    NEW_NERVE(BossKameckBattleDemoNrvPowerUpVs1, BossKameckBattleDemo, PowerUpVs1);
    NEW_NERVE(BossKameckBattleDemoNrvDownVs1, BossKameckBattleDemo, DownVs1);
    NEW_NERVE(BossKameckBattleDemoNrvWaitAppearVs2, BossKameckBattleDemo, WaitAppearVs2);
    NEW_NERVE(BossKameckBattleDemoNrvAppearVs2, BossKameckBattleDemo, AppearVs2);
    NEW_NERVE(BossKameckBattleDemoNrvPowerUpVs2, BossKameckBattleDemo, PowerUpVs2);
    NEW_NERVE(BossKameckBattleDemoNrvDownVs2, BossKameckBattleDemo, DownVs2);
};  // namespace NrvBossKamecBattleDemo

BossKameckBattleDemo::BossKameckBattleDemo(BossKameck* pBoss, const JMapInfoIter& rIter) : BossKameckAction("ボスカメック戦デモ", pBoss) {
    mDemoPos = nullptr;
    mCurDemoName = nullptr;
    mDemoNerve = nullptr;
    mDemoPos = new BossKameckDemoPosition();
    mDemoPos->init(rIter);
}

void BossKameckBattleDemo::init() {
    initNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvWaitAppearVs1::sInstance);
}

void BossKameckBattleDemo::appear() {
    mIsDead = false;
    mDemoPos->appear();
}

void BossKameckBattleDemo::kill() {
    mIsDead = true;
    mDemoPos->kill();
}

void BossKameckBattleDemo::startTryDemo(const char* pName, const Nerve* pNerve) {
    appear();

    if (MR::tryStartDemoMarioPuppetable(mHost, pName)) {
        MR::overlayWithPreviousScreen(2);
        setNerve(pNerve);
    } else {
        mDemoNerve = pNerve;
        mCurDemoName = pName;
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvTryStartDemo::sInstance);
    }
}

void BossKameckBattleDemo::exeTryStartDemo() {
    if (MR::tryStartDemoMarioPuppetable(mHost, mCurDemoName)) {
        MR::overlayWithPreviousScreen(2);
        setNerve(mDemoNerve);
        mCurDemoName = nullptr;
        mDemoNerve = nullptr;
    }
}

void BossKameckBattleDemo::startDemoAppearVs1() {
    startTryDemo("カメック登場Vs1", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvWaitAppearVs1::sInstance);
    MR::hideModelAndOnCalcAnim(mHost);
    MR::invalidateShadowAll(mHost);
}

void BossKameckBattleDemo::startDemoAppearVs2() {
    startTryDemo("カメック登場Vs2", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvWaitAppearVs2::sInstance);
    MR::hideModelAndOnCalcAnim(mHost);
    MR::invalidateShadowAll(mHost);
}

void BossKameckBattleDemo::startDemoPowerUpVs1() {
    startTryDemo("カメックパワーアップVs1", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvPowerUpVs1::sInstance);
}

void BossKameckBattleDemo::startDemoPowerUpVs2() {
    startTryDemo("カメックパワーアップVs2", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvPowerUpVs2::sInstance);
}

void BossKameckBattleDemo::startDemoDownVs1() {
    startTryDemo("ボスカメックダウンLv1", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvDownVs1::sInstance);
}

void BossKameckBattleDemo::startDemoDownVs2() {
    startTryDemo("ボスカメックダウンLv2", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvDownVs2::sInstance);
}

void BossKameckBattleDemo::exeWaitAppearVs1() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BattleWait", (const char*)nullptr);
        MR::stopStageBGM(0x5A);
        mHost->startDemo();
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvAppearVs1::sInstance);
    }
}

void BossKameckBattleDemo::exeAppearVs1() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(1);
        MR::showModel(mHost);
        BossKameckDemoPosition* pos = mDemoPos;
        const char* demoName = "DemoBossKameckStart";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, demoName, 0, 1.0f);
        MR::startBck(pos, "DemoBossKameckStart", nullptr);
        MR::startAction(mHost, "DemoBossKameckStart");
        MR::startStageBGM("MBGM_BOSS_04", false);
    }

    if (MR::isLessStep(this, 270)) {
        MR::startLevelSound(mHost, "SE_BM_LV_KAMECK_DEMO_PRE_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 270)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEMO_SMOKE", -1, -1);
        MR::startSound(mHost, "SE_BV_KAMECK_APPEAR", -1, -1);
        MR::validateShadowAll(mHost);
    }

    updateCastPose();

    if (MR::isActionEnd(mHost)) {
        BossKameckDemoPosition* pos = mDemoPos;
        MR::endAnimCamera(pos, pos->mCameraInfo, "DemoBossKameckStart", 0, true);
        pos->makeActorDead();
        MR::endDemo(mHost, "カメック登場Vs1");
        mHost->endDemo();
        kill();
    }
}

void BossKameckBattleDemo::exeWaitAppearVs2() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(0x5A);
        mHost->startDemo();
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvAppearVs2::sInstance);
    }
}

void BossKameckBattleDemo::exeAppearVs2() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(1);
        MR::showModel(mHost);
        BossKameckDemoPosition* pos = mDemoPos;
        const char* demoName = "DemoBossKameckStart2";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, demoName, 0, 1.0f);
        MR::startBck(pos, "DemoBossKameckStart2", nullptr);
        MR::startAction(mHost, "DemoBossKameckStart2");
        MR::startStageBGM("MBGM_BOSS_04", false);
    }

    if (MR::isStep(this, 60)) {
        MR::startSound(mHost, "SE_BV_KAMECK_APPEAR", -1, -1);
    }

    if (MR::isLessStep(this, 155)) {
        MR::startLevelSound(mHost, "SE_BM_LV_KAMECK_DEMO_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 155)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEMO_APPEA_END", -1, -1);
        MR::validateShadowAll(mHost);
    }

    updateCastPose();

    if (MR::isActionEnd(mHost)) {
        BossKameckDemoPosition* pos = mDemoPos;
        MR::endAnimCamera(pos, pos->mCameraInfo, "DemoBossKameckStart2", 0, true);
        pos->makeActorDead();
        MR::endDemo(mHost, "カメック登場Vs2");
        mHost->endDemo();
        kill();
    }
}

void BossKameckBattleDemo::exePowerUpVs1() {
    if (MR::isFirstStep(this)) {
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, powerUpName, 0, 1.0f);
        MR::startBck(pos, powerUpName, nullptr);
        MR::startAction(mHost, powerUpName);
        MR::startBckPlayer("BattleWait", (const char*)0);
        mHost->startDemo();
    }

    if (MR::isStep(this, 200)) {
        mHost->startDemoAppearKameck();
    }

    updateCastPose();

    if (MR::isActionEnd(mHost)) {
        MR::endDemo(mHost, "カメックパワーアップVs1");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        mHost->endDemoAppearKameck();
        mHost->endDemo();
        kill();
    }
}

void BossKameckBattleDemo::exePowerUpVs2() {
    if (MR::isFirstStep(this)) {
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp2";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, powerUpName, 0, 1.0f);
        MR::startBck(pos, powerUpName, nullptr);
        MR::startAction(mHost, powerUpName);
        MR::startBckPlayer("BattleWait", (const char*)0);
        mHost->startDemo();
    }

    MR::startLevelSound(mHost, "SE_BM_LV_KAMECK_DEMO_MOVE", -1, -1, -1);

    if (MR::isStep(this, 200)) {
        mHost->startDemoAppearKameck();
    }

    updateCastPose();

    if (MR::isActionEnd(mHost)) {
        MR::endDemo(mHost, "カメックパワーアップVs2");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp2";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        mHost->endDemoAppearKameck();
        mHost->endDemo();
        kill();
    }
}

void BossKameckBattleDemo::exeDownVs1() {
    if (MR::isFirstStep(this)) {
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, powerUpName, 0, 1.0f);
        MR::startBck(pos, powerUpName, nullptr);
        MR::startAction(mHost, powerUpName);
        mHost->deadKameck();
        mHost->startDemo();
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(30);
    }

    updateCastPose();

    if (MR::isStep(this, 18)) {
        MR::startSound(mHost, "SE_BV_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 150)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 285)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, 345)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEMO_STAFF_BREAK", -1, -1);
        MR::startSystemSE("SE_SY_POW_STAR_APPEAR", -1, -1);
    }

    if (MR::isActionEnd(mHost)) {
        MR::endDemo(mHost, "ボスカメックダウンLv1");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        MR::appearPowerStarWithoutDemo(mHost);
        mHost->endDemo();
        mHost->kill();
        kill();
    }
}

void BossKameckBattleDemo::exeDownVs2() {
    if (MR::isFirstStep(this)) {
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown2";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, powerUpName, 0, 1.0f);
        MR::startBck(pos, powerUpName, nullptr);
        MR::startAction(mHost, powerUpName);
        mHost->deadKameck();
        mHost->startDemo();
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(30);
    }

    updateCastPose();

    if (MR::isStep(this, 18)) {
        MR::startSound(mHost, "SE_BV_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 150)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 285)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, 345)) {
        MR::startSound(mHost, "SE_BM_KAMECK_DEMO_STAFF_BREAK", -1, -1);
        MR::startSystemSE("SE_SY_POW_STAR_APPEAR", -1, -1);
    }

    if (MR::isActionEnd(mHost)) {
        MR::endDemo(mHost, "ボスカメックダウンLv2");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown2";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        MR::appearPowerStarWithoutDemo(mHost);
        mHost->endDemo();
        mHost->kill();
        kill();
    }
}

void BossKameckBattleDemo::updateCastPose() {
    mDemoPos->movement();
    mDemoPos->calcAnim();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPos, "MarioPosition"));
    BossKameck* boss = mHost;
    boss->setPose(MR::getJointMtx(mDemoPos, "KameckPosition"));
}

BossKameckBattleDemo::~BossKameckBattleDemo() {}

BossKameckDemoPosition::~BossKameckDemoPosition() {}
