#include "Game/Boss/BossKameckBattleDemo.hpp"
#include "Game/Boss/BossKameck.hpp"

BossKameckDemoPosition::BossKameckDemoPosition() : LiveActor("キャスト位") {
    makeActorDead();
}

void BossKameckDemoPosition::init(const JMapInfoIter &rIter) {
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

BossKameckBattleDemo::BossKameckBattleDemo(BossKameck *pBoss, const JMapInfoIter &rIter) : BossKameckAction("ボスカメック戦デモ", pBoss) {
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

void BossKameckBattleDemo::startTryDemo(const char *pName, const Nerve *pNerve) {
    appear();
    if (MR::tryStartDemoMarioPuppetable(mBossKameck, pName)) {
        MR::overlayWithPreviousScreen(2);
        setNerve(pNerve);
    }
    else {
        mDemoNerve = pNerve;
        mCurDemoName = pName;
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvTryStartDemo::sInstance);
    }
}

void BossKameckBattleDemo::exeTryStartDemo() {
    if (MR::tryStartDemoMarioPuppetable(mBossKameck, mCurDemoName)) {
        MR::overlayWithPreviousScreen(2);
        setNerve(mDemoNerve);
        mCurDemoName = nullptr;
        mDemoNerve = nullptr;
    }
}

void BossKameckBattleDemo::startDemoAppearVs1() {
    startTryDemo("カメック登場Vs1", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvWaitAppearVs1::sInstance);
    MR::hideModelAndOnCalcAnim(mBossKameck);
    MR::invalidateShadowAll(mBossKameck);
}

void BossKameckBattleDemo::startDemoAppearVs2() {
    startTryDemo("カメック登場Vs2", &NrvBossKamecBattleDemo::BossKameckBattleDemoNrvWaitAppearVs2::sInstance);
    MR::hideModelAndOnCalcAnim(mBossKameck);
    MR::invalidateShadowAll(mBossKameck);
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
        mBossKameck->startDemo();
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvAppearVs1::sInstance);
    }
}

void BossKameckBattleDemo::exeAppearVs1() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(1);
        MR::showModel(mBossKameck);
        BossKameckDemoPosition* pos = mDemoPos;
        const char* demoName = "DemoBossKameckStart";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, demoName, 0, 1.0f);
        MR::startBck(pos, "DemoBossKameckStart", nullptr);
        MR::startAction(mBossKameck, "DemoBossKameckStart");
        MR::startStageBGM("MBGM_BOSS_04", false);
    }

    if (MR::isLessStep(this, 270)) {
        MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_DEMO_PRE_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 270)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEMO_SMOKE", -1, -1);
        MR::startSound(mBossKameck, "SE_BV_KAMECK_APPEAR", -1, -1);
        MR::validateShadowAll(mBossKameck);
    }

    updateCastPose();
    if (MR::isActionEnd(mBossKameck)) {
        BossKameckDemoPosition* pos = mDemoPos;
        MR::endAnimCamera(pos, pos->mCameraInfo, "DemoBossKameckStart", 0, true);
        pos->makeActorDead();
        MR::endDemo(mBossKameck, "カメック登場Vs1");
        mBossKameck->endDemo();
        kill();
    }
}

void BossKameckBattleDemo::exeWaitAppearVs2() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(0x5A);
        mBossKameck->startDemo();
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvBossKamecBattleDemo::BossKameckBattleDemoNrvAppearVs2::sInstance);
    }
}

void BossKameckBattleDemo::exeAppearVs2() {
    if (MR::isFirstStep(this)) {
        MR::overlayWithPreviousScreen(1);
        MR::showModel(mBossKameck);
        BossKameckDemoPosition* pos = mDemoPos;
        const char* demoName = "DemoBossKameckStart2";
        pos->makeActorAppeared();
        MR::startAnimCameraTargetSelf(pos, pos->mCameraInfo, demoName, 0, 1.0f);
        MR::startBck(pos, "DemoBossKameckStart2", nullptr);
        MR::startAction(mBossKameck, "DemoBossKameckStart2");
        MR::startStageBGM("MBGM_BOSS_04", false);
    }

    if (MR::isStep(this, 60)) {
        MR::startSound(mBossKameck, "SE_BV_KAMECK_APPEAR", -1, -1);
    }

    if (MR::isLessStep(this, 155)) {
        MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_DEMO_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 155)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEMO_APPEA_END", -1, -1);
        MR::validateShadowAll(mBossKameck);
    }

    updateCastPose();
    if (MR::isActionEnd(mBossKameck)) {
        BossKameckDemoPosition* pos = mDemoPos;
        MR::endAnimCamera(pos, pos->mCameraInfo, "DemoBossKameckStart2", 0, true);
        pos->makeActorDead();
        MR::endDemo(mBossKameck, "カメック登場Vs2");
        mBossKameck->endDemo();
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
        MR::startAction(mBossKameck, powerUpName);
        MR::startBckPlayer("BattleWait", (const char*)0);
        mBossKameck->startDemo();
    }

    if (MR::isStep(this, 200)) {
        mBossKameck->startDemoAppearKameck();
    }

    updateCastPose();

    if (MR::isActionEnd(mBossKameck)) {
        MR::endDemo(mBossKameck, "カメックパワーアップVs1");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        mBossKameck->endDemoAppearKameck();
        mBossKameck->endDemo();
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
        MR::startAction(mBossKameck, powerUpName);
        MR::startBckPlayer("BattleWait", (const char*)0);
        mBossKameck->startDemo();
    }

    MR::startLevelSound(mBossKameck, "SE_BM_LV_KAMECK_DEMO_MOVE", -1, -1, -1);

    if (MR::isStep(this, 200)) {
        mBossKameck->startDemoAppearKameck();
    }

    updateCastPose();

    if (MR::isActionEnd(mBossKameck)) {
        MR::endDemo(mBossKameck, "カメックパワーアップVs2");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckPowerUp2";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        mBossKameck->endDemoAppearKameck();
        mBossKameck->endDemo();
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
        MR::startAction(mBossKameck, powerUpName);
        mBossKameck->deadKameck();
        mBossKameck->startDemo();
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(30);
    }

    updateCastPose();

    if (MR::isStep(this, 18)) {
        MR::startSound(mBossKameck, "SE_BV_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 150)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 285)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, 345)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEMO_STAFF_BREAK", -1, -1);
        MR::startSystemSE("SE_SY_POW_STAR_APPEAR", -1, -1);
    }

    if (MR::isActionEnd(mBossKameck)) {
        MR::endDemo(mBossKameck, "ボスカメックダウンLv1");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        MR::appearPowerStarWithoutDemo(mBossKameck);
        mBossKameck->endDemo();
        mBossKameck->kill();
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
        MR::startAction(mBossKameck, powerUpName);
        mBossKameck->deadKameck();
        mBossKameck->startDemo();
        MR::startBckPlayer("BattleWait", (const char*)0);
        MR::stopStageBGM(30);
    }

    updateCastPose();

    if (MR::isStep(this, 18)) {
        MR::startSound(mBossKameck, "SE_BV_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 150)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEAD", -1, -1);
    }

    if (MR::isStep(this, 285)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, 345)) {
        MR::startSound(mBossKameck, "SE_BM_KAMECK_DEMO_STAFF_BREAK", -1, -1);
        MR::startSystemSE("SE_SY_POW_STAR_APPEAR", -1, -1);
    }

    if (MR::isActionEnd(mBossKameck)) {
        MR::endDemo(mBossKameck, "ボスカメックダウンLv2");
        BossKameckDemoPosition* pos = mDemoPos;
        const char* powerUpName = "DemoBossKameckDown2";
        MR::endAnimCamera(pos, pos->mCameraInfo, powerUpName, 0, true);
        pos->makeActorDead();
        MR::appearPowerStarWithoutDemo(mBossKameck);
        mBossKameck->endDemo();
        mBossKameck->kill();
        kill();
    }
}

void BossKameckBattleDemo::updateCastPose() {
    mDemoPos->movement();
    mDemoPos->calcAnim();
    MR::setPlayerBaseMtx(MR::getJointMtx(mDemoPos, "MarioPosition"));
    BossKameck* boss = mBossKameck;
    boss->setPose(MR::getJointMtx(mDemoPos, "KameckPosition"));
}

BossKameckBattleDemo::~BossKameckBattleDemo() {

}

BossKameckDemoPosition::~BossKameckDemoPosition() {

}

namespace NrvBossKamecBattleDemo {
    INIT_NERVE(BossKameckBattleDemoNrvTryStartDemo);
    INIT_NERVE(BossKameckBattleDemoNrvWaitAppearVs1);
    INIT_NERVE(BossKameckBattleDemoNrvAppearVs1);
    INIT_NERVE(BossKameckBattleDemoNrvPowerUpVs1);
    INIT_NERVE(BossKameckBattleDemoNrvDownVs1);
    INIT_NERVE(BossKameckBattleDemoNrvWaitAppearVs2);
    INIT_NERVE(BossKameckBattleDemoNrvAppearVs2);
    INIT_NERVE(BossKameckBattleDemoNrvPowerUpVs2);
    INIT_NERVE(BossKameckBattleDemoNrvDownVs2);
};