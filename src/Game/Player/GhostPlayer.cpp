#include "Game/Player/GhostPlayer.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Player/GhostPacket.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/JetTurtleShadow.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

void GhostPlayer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    struct AnimSoundInfo {
        /* 0x00 */ const char* pAnimName;
        /* 0x04 */ const char* pSoundBM;
        /* 0x08 */ const char* pSoundBV;
    };

    const AnimSoundInfo sAnimSoundTable[28] = {{"ジャンプ", "SE_BM_GHOST_MARIO_JUMP_S", "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"ジャンプB", "SE_BM_GHOST_MARIO_JUMP_M", "SE_BV_GHOST_MARIO_JUMP_M"},
                                               {"ジャンプC", "SE_BM_GHOST_MARIO_JUMP_L", "SE_BV_GHOST_MARIO_JUMP_L"},
                                               {"壁ジャンプ", "SE_BM_GHOST_MARIO_JUMP_S", "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"埋まり脱出ジャンプ", "SE_BM_GHOST_MARIO_JUMP_S", "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"幅とび", "SE_BM_GHOST_MARIO_JUMP_L", "SE_BV_GHOST_MARIO_JUMP_L"},
                                               {"しゃがみジャンプ", "SE_BM_GHOST_MARIO_JUMP_L", "SE_BV_GHOST_MARIO_JUMP_L"},
                                               {"崖つかまり開始", nullptr, "SE_BV_GHOST_MARIO_HANG"},
                                               {"崖つかまり終了", nullptr, "SE_BV_GHOST_MARIO_CLIMB"},
                                               {"壁押し", nullptr, "SE_BV_GHOST_MARIO_HANG"},
                                               {"ヒップドロップ開始", "SE_BM_GHOST_MARIO_HIP_DROP_TURN", "SE_BV_GHOST_MARIO_HIPDROP_S"},
                                               {"ヒップドロップ着地", "SE_BM_GHOST_MARIO_HIP_DROP_LAND", "SE_BV_GHOST_MARIO_HIPDROP_E"},
                                               {"埋まり脱出ジャンプ", "SE_BM_GHOST_MARIO_JUMP_S", "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"水泳スピン", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"水泳ジェット開始", nullptr, "SE_BV_GHOST_MARIO_SWM_ACCEL"},
                                               {"カメ持ちリング準備", nullptr, "SE_BV_GHOST_MARIO_TAKE"},
                                               {"カメ持ちリング", nullptr, "SE_BV_GHOST_MARIO_SWM_ACCEL"},
                                               {"投げ", nullptr, "SE_BV_GHOST_MARIO_THROW"},
                                               {"空中ひねり", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"地上ひねり", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"空パンチ", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"アイスひねり", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"アイスひねり移動", "SE_BM_GHOST_MARIO_SPIN", "SE_BV_GHOST_MARIO_SPIN"},
                                               {"スケートアクセルジャンプ", "SE_BM_GHOST_MARIO_JUMP_L", "SE_BV_GHOST_MARIO_JUMP_L"},
                                               {"スケートジャンプ2", "SE_BM_GHOST_MARIO_JUMP_L", "SE_BV_GHOST_MARIO_JUMP_L"},
                                               {"氷上力行右", nullptr, "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"氷上力行左", nullptr, "SE_BV_GHOST_MARIO_JUMP_S"},
                                               {"ショートジャンプ", "SE_BM_GHOST_MARIO_JUMP_S", "SE_BV_GHOST_MARIO_JUMP_S"}};

    static inline const AnimSoundInfo* getAnimSoundInfo(const char* pAnimName) {
        for (u32 i = 0; i < ARRAY_SIZE(sAnimSoundTable); i++) {
            if (MR::isEqualString(pAnimName, sAnimSoundTable[i].pAnimName)) {
                return &sAnimSoundTable[i];
            }
        }
        return nullptr;
    }

    void playSound(LiveActor* pActor, const char* pAnimName) {
        const AnimSoundInfo* matchedEntry = getAnimSoundInfo(pAnimName);

        if (matchedEntry != nullptr) {
            if (matchedEntry->pSoundBM != nullptr) {
                MR::startSound(pActor, matchedEntry->pSoundBM);
            }
            if (matchedEntry->pSoundBV != nullptr) {
                MR::startSound(pActor, matchedEntry->pSoundBV);
            }
        }
    }
};  // namespace

namespace NrvGhostPlayer {
    NEW_NERVE(HostTypeNrvWait, GhostPlayer, Wait);
    NEW_NERVE(HostTypeNrvWinDemo, GhostPlayer, WinDemo);
    NEW_NERVE(HostTypeNrvLostDemo, GhostPlayer, LostDemo);
    NEW_NERVE(HostTypeNrvPreStartDemo0, GhostPlayer, PreStartDemo0);
    NEW_NERVE(HostTypeNrvPreStartDemo1, GhostPlayer, PreStartDemo1);
    NEW_NERVE(HostTypeNrvPreStartDemo2, GhostPlayer, PreStartDemo2);
    NEW_NERVE(HostTypeNrvStartDemo, GhostPlayer, Wait);
    NEW_NERVE(HostTypeNrvRun, GhostPlayer, Wait);
};  // namespace NrvGhostPlayer

void GhostPlayer::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter&) {
    char archiveName[0x100];
    strcpy(archiveName, "GhostData");
    strcat(archiveName, MR::getCurrentStageName());
    pArchiveList->addArchive(archiveName);

    if (MR::isPlayerLuigi()) {
        pArchiveList->addArchive("GhostLuigi");
    } else {
        pArchiveList->addArchive("GhostMario");
    }
}

GhostPlayer::GhostPlayer(const char* pName) : LiveActor(pName), _90(0), mCameraInfo(nullptr), mXanimePlayer(nullptr) {
    _8C = 0;
    mTargetRotation.zero();
    mKilledByStar = false;
    mPowerStarTarget = nullptr;
    mHasJetTurtle = false;
}

void GhostPlayer::init(const JMapInfoIter& rIter) {
    char gstPath[256];
    char arcPath[256];

    RaceManagerLayout* layout = new RaceManagerLayout("レース管理用レイアウト");
    mRaceManagerLayout = layout;
    mRaceManagerLayout->init(rIter);

    mCurrentPacket = nullptr;
    if (MR::isPlayerLuigi()) {
        const char* currentStageName = MR::getCurrentStageName();
        sprintf(gstPath, "%sLuigi.gst", currentStageName);
    } else {
        const char* currentStageName = MR::getCurrentStageName();
        sprintf(gstPath, "%s.gst", currentStageName);
    }
    strcpy(arcPath, "GhostData");
    strcat(arcPath, MR::getCurrentStageName());
    strcat(arcPath, ".arc");

    mCurrentPacket = static_cast< GhostPacket* >(MR::loadResourceFromArc(arcPath, gstPath));

    if (MR::isFileExist(gstPath, false)) {
        mCurrentPacket = static_cast< GhostPacket* >(MR::loadToMainRAM(gstPath, nullptr, nullptr, JKRDvdRipper::ALLOC_DIRECTION_FORWARD));
    }

    if (mCurrentPacket == nullptr) {
        makeActorDead();
        return;
    } else {
        if (MR::isPlayerLuigi())
            initModelManagerWithAnm("GhostLuigi", nullptr, false);
        else
            initModelManagerWithAnm("GhostMario", nullptr, false);
    }

    initEffectKeeper(5, "GhostMario", false);
    MR::connectToScene(this, MR::MovementType_Player, MR::CalcAnimType_Player, -1, MR::DrawType_Player);
    MR::initDefaultPos(this, rIter);
    mStartPos = mPosition;
    mVelocity.zero();
    initAnimation();
    initHitSensor(1);
    MR::addHitSensorRide(this, "body", 4, 40.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::initShadowVolumeSphere(this, 50.0f);
    MR::validateShadow(this, nullptr);
    MR::calcGravity(this);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcShadowDropGravity(this, nullptr);
    initSound(6, false);
    MR::invalidateClipping(this);
    _90 = 0;
    mCameraInfo = nullptr;
    if (MR::isValidInfo(rIter)) {
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "レース開始1");
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "レース開始2");
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "レース開始3");
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "レース終了");
    }
    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    initNerve(&NrvGhostPlayer::HostTypeNrvWait::sInstance);
    MR::needStageSwitchReadAppear(this, rIter);
    MR::syncStageSwitchAppear(this);
    makeActorDead();
    mWaitingToStart = true;
    _112 = true;
    MR::getRotatedAxisZ(&mInitialDirection, mRotation);
    PSMTXCopy(getBaseMtx(), mTargetRotationMtx);
    mJetTurtleShadow = new JetTurtleShadow("カメシャドウモデル");
    mJetTurtleShadow->initWithoutIter();
    mHandRPos = new FixedPosition(this, "HandR", TVec3f(15.59f, 42.5f, 42.93f), TVec3f(-17.05f, -0.7f, 113.55f));
    MR::declareStarPiece(this, 50);
    MR::declareCoin(this, 50);
    mAppearStarPieceCooldown = 0;
    mPlayerTrampleCooldown = 0;
}

void GhostPlayer::appear() {
    LiveActor::appear();
    setNerve(&NrvGhostPlayer::HostTypeNrvPreStartDemo0::sInstance);
    mIsHidden = true;
    MR::invalidateShadow(this, nullptr);
    if (MR::isPlayerLuigi()) {
        MR::startBtk(this, "GhostLuigi");
        MR::startBrk(this, "GhostLuigi");
    } else {
        MR::startBtk(this, "GhostMario");
        MR::startBrk(this, "GhostMario");
    }
    MR::emitEffect(this, "Shadow");
}

void GhostPlayer::control() {
    if (!mWaitingToStart && !MR::isDead(this)) {
        MR::startLevelSound(this, "SE_BM_LV_GHOST_MARIO_AMBIENT");
        if (!isNerve(&NrvGhostPlayer::HostTypeNrvLostDemo::sInstance) && !isNerve(&NrvGhostPlayer::HostTypeNrvWinDemo::sInstance)) {
            if (!strcmp("powerstarget", MR::getPlayerCurrentBckName()) && isNerve(&NrvGhostPlayer::HostTypeNrvRun::sInstance)) {
                setNerve(&NrvGhostPlayer::HostTypeNrvLostDemo::sInstance);
                mKilledByStar = true;
            } else if (mCurrentPacket) {
                u32 packetDelayTimer = 0;
                while (true) {
                    GhostPacket packet(mCurrentPacket, 0);
                    mCurrentPacket = reinterpret_cast< GhostPacket* >(reinterpret_cast< char* >(mCurrentPacket) + receiveGhostPacket(&packet));
                    if (_112) {
                        if (mXanimePlayer->_10[3] > 0.9f)
                            continue;

                        _112 = false;
                        packetDelayTimer = 10;
                        continue;
                    }

                    if (packetDelayTimer == 0)
                        break;
                    packetDelayTimer--;
                }
                if (mAppearStarPieceCooldown)
                    mAppearStarPieceCooldown--;
                if (mPlayerTrampleCooldown)
                    mPlayerTrampleCooldown--;
            }
        }
    }
}

void GhostPlayer::exeWait() {
    if (mRaceManagerLayout->isAllAnimStopped())
        mRaceManagerLayout->kill();
}

void GhostPlayer::warpPosition(const char* pName) {
    MR::findNamePos(pName, getBaseMtx());
    PSMTXCopy(getBaseMtx(), mTargetRotationMtx);
    MR::extractMtxTrans(getBaseMtx(), &mPosition);
}

void GhostPlayer::exePreStartDemo0() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_GHOST_MARIO_APPEAR");
        MR::startSound(this, "SE_BV_GHOST_MARIO_APPEAR");
        MR::offPlayerControl();
        TPos3f* cameraTargetMatrix = &mCameraTargetMtx->mMatrix;
        cameraTargetMatrix->set(getBaseMtx());

        MR::startMultiActorCameraTargetOther(this, mCameraInfo, "レース開始1", CameraTargetArg(mCameraTargetMtx), -1);
        warpPosition("ゴーストデモゴースト位置");
    }

    if (getNerveStep() == 1) {
        Mtx playerPosMtx;
        TVec3f playerPosVec;

        MR::findNamePos("ゴーストデモマリオ位置", playerPosMtx);
        MR::extractMtxTrans(playerPosMtx, &playerPosVec);
        MR::setPlayerPosAndWait(playerPosVec);
        MR::setPlayerBaseMtx(playerPosMtx);
        MR::startBckPlayerJ("レース見る");
        MR::resetPlayerEffect();
        MR::tryStartDemo(this, "レース準備");
        MR::requestMovementOn(this);
        MR::requestMovementOnPlayer();
    }
    if (getNerveStep() == 0x3C) {
        mIsHidden = false;
        setAnimation("ゴースト出現");
        MR::validateShadow(this, nullptr);
    }
    if (getNerveStep() == 0x6B) {
        MR::startSound(this, "SE_BV_GHOST_MARIO_LAND");
    }
    if (getNerveStep() == 0x6F) {
        MR::startSound(this, "SE_BM_GHOST_MARIO_LAND");
    }
    if (getNerveStep() == 0x92) {
        MR::startSound(this, "SE_BV_GHOST_MARIO_PROVOKE");
    }
    if (getNerveStep() == 0xF0) {
        MR::endMultiActorCamera(this, mCameraInfo, "レース開始1", false, -1);
        TPos3f* cameraTargetMatrix = &mCameraTargetMtx->mMatrix;
        cameraTargetMatrix->set(getBaseMtx());
        MR::startMultiActorCameraTargetOther(this, mCameraInfo, "レース開始2", CameraTargetArg(mCameraTargetMtx), -1);
        setNerve(&NrvGhostPlayer::HostTypeNrvPreStartDemo1::sInstance);
        setAnimation("レース見る");
    } else if (isRequestSkipDemo()) {
        MR::endMultiActorCamera(this, mCameraInfo, "レース開始1", false, -1);
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "レース開始3", -1);
        setNerve(&NrvGhostPlayer::HostTypeNrvPreStartDemo2::sInstance);
        if (getNerveStep() < 0x3C) {
            mIsHidden = false;
            setAnimation("ゴースト出現");
            MR::validateShadow(this, nullptr);
        }
    }
}

bool GhostPlayer::isRequestSkipDemo() const {
    if (MR::hasRetryGalaxySequence() && MR::testCorePadTriggerA(0))
        return true;
    return false;
}

void GhostPlayer::exePreStartDemo1() {
    if (getNerveStep() == 0xF0) {
        MR::endMultiActorCamera(this, mCameraInfo, "レース開始2", false, -1);
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "レース開始3", -1);
        setNerve(&NrvGhostPlayer::HostTypeNrvPreStartDemo2::sInstance);
    } else if (isRequestSkipDemo()) {
        MR::endMultiActorCamera(this, mCameraInfo, "レース開始2", false, -1);
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "レース開始3", -1);
        setNerve(&NrvGhostPlayer::HostTypeNrvPreStartDemo2::sInstance);
    }
}

void GhostPlayer::exePreStartDemo2() {
    if (MR::isFirstStep(this)) {
        mRaceManagerLayout->setTime(0);
        mRaceManagerLayout->appear();
        mRaceManagerLayout->playCountAndGo();
        mRaceManagerLayout->hideRecordPane();
        mRaceManagerLayout->hideBestRecordPane();
        setAnimation("ゴーストレース開始");
        MR::startBckPlayerJ("レース開始");

        Mtx raceStartMarioPos;
        MR::findNamePos("レース開始時マリオ位置", raceStartMarioPos);
        MR::setPlayerBaseMtx(raceStartMarioPos);

        mPosition = mStartPos;
        MR::endDemo(this, "レース準備");
    }

    if (MR::getPlayerTriggerZ()) {
        MR::startBckPlayerJ("レースクラウチング開始");
        MR::startSoundPlayer("SE_PV_SQUAT", -1);
    } else if (MR::testSubPadReleaseZ(0)) {
        MR::startBckPlayerJ("レース開始");
    }

    if (getNerveStep() % 60 == 0) {
        MR::startSystemSE("SE_SY_RACE_COUNT_DOWN");
    }

    TVec3f normalizedFrontVec = mInitialDirection;
    TVec3f upVec;
    TVec3f frontVec;

    bool isNormalized = MR::normalizeOrZero(&normalizedFrontVec);
    TPos3f* baseMtx = reinterpret_cast< TPos3f* >(getBaseMtx());
    baseMtx->getYDir(upVec);
    if (!isNormalized) {
        MR::calcFrontVec(&frontVec, this);
        if (MR::vecBlendSphere(frontVec, normalizedFrontVec, &frontVec, 0.1f) == false) {
            frontVec = normalizedFrontVec;
        }
        MR::makeMtxUpFront(reinterpret_cast< TPos3f* >(getBaseMtx()), upVec, frontVec);
        PSMTXCopy(getBaseMtx(), mTargetRotationMtx);
        MR::extractMtxTrans(getBaseMtx(), &mPosition);
    }
    if (!mRaceManagerLayout->isPlayCountAnim()) {
        mWaitingToStart = false;
        MR::startSound(this, "SE_BV_GHOST_MARIO_RUN_START");
        setNerve(&NrvGhostPlayer::HostTypeNrvRun::sInstance);
        MR::onPlayerControl(true);
        MR::noticePlayerDashChance();
        MR::startBckPlayerJ("基本");
        MR::startSystemSE("SE_SY_RACE_START");
        MR::endMultiActorCamera(this, mCameraInfo, "レース開始3", false, -1);
    }
}

void GhostPlayer::exeWinDemo() {
    if (MR::isFirstStep(this)) {
        MR::offPlayerControl();
        MR::tryPlayerKillTakingActor();
        MR::readyPlayerDemo();

        TVec3f playerPosVec;
        Mtx playerPosMtx;
        MR::findNamePos("負け時マリオ位置", playerPosMtx);
        MR::extractMtxTrans(playerPosMtx, &playerPosVec);
        MR::setPlayerPosAndWait(playerPosVec);
        MR::setPlayerBaseMtx(playerPosMtx);

        MR::startBckPlayerJ("レース見る");
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "レース終了", -1);
        mIsDemoCameraActive = true;

        HitSensor* sensorBody = getSensor("body");
        for (int i = 0; i < sensorBody->mSensorCount; i++) {
            HitSensor* sensor = sensorBody->mSensors[i];
            if (sensor->mType == ATYPE_POWER_STAR_BIND) {
                mPowerStarTarget = sensor->mHost;
                mPosition = sensor->mHost->mPosition;
            }
        }

        MR::startSubBGM("BGM_RACE_LOSE", false);
        mRaceManagerLayout->appear();
        mRaceManagerLayout->playLose();
        setAnimation("ゴースト勝利");
        MR::startSound(this, "SE_BV_GHOST_MARIO_WIN");
        MR::tryStartDemo(this, "レース終了");
        MR::requestMovementOn(this);
        MR::requestMovementOnPlayer();
        if (mPowerStarTarget)
            MR::requestMovementOn(mPowerStarTarget);
    }

    if (mPowerStarTarget) {
        PSMTXCopy(mPowerStarTarget->getBaseMtx(), getBaseMtx());
        PSMTXCopy(getBaseMtx(), mTargetRotationMtx);
        MR::extractMtxTrans(getBaseMtx(), &mPosition);
    }

    if (MR::isStep(this, 0xB4)) {
        if (mIsDemoCameraActive) {
            MR::endMultiActorCamera(this, mCameraInfo, "レース終了", true, -1);
            mIsDemoCameraActive = false;
        }
        MR::endDemo(this, "レース終了");
        MR::forceKillPlayerByGhostRace();
    }
}

void GhostPlayer::exeLostDemo() {
    if (MR::isFirstStep(this)) {
        warpPosition("負け時マリオ位置");
        mXanimePlayer->changeAnimation("DieSwimEvent");
        mVelocity.zero();
        setAnimation("レース負け");
    }
}

void GhostPlayer::calcAndSetBaseMtx() {
    if (!MR::isDead(this)) {
        J3DModel* model = MR::getJ3DModel(this);
        MtxPtr baseMtx = model->getBaseTRMtx();

        MR::blendMtxRotate(getBaseMtx(), mTargetRotationMtx, 0.2f, baseMtx);
        MR::setMtxTrans(baseMtx, mPosition.x, mPosition.y, mPosition.z);
        MR::getJ3DModel(this)->setBaseScale(mScale);

        if (mHasJetTurtle) {
            mHandRPos->calc();
            mJetTurtleShadow->calcType0(mHandRPos->mMtx);
        }
    }
}

void GhostPlayer::initAnimation() {
    XanimeResourceTable* pResource = MR::getPlayerXanimeResource();

    mAnimTrackWeights[0] = 0.0f;
    mAnimTrackWeights[1] = 0.0f;
    mAnimTrackWeights[2] = 0.0f;
    mAnimTrackWeights[3] = 1.0f;

    mXanimePlayer = new XanimePlayer(MR::getJ3DModel(this), pResource);
    mXanimePlayer->duplicateSimpleGroup();
    mModelManager->mXanimePlayer = mXanimePlayer;
    mXanimePlayer->setDefaultAnimation("基本");
    mXanimePlayer->getCore()->enableJointTransform(MR::getJ3DModelData(this));
    mXanimePlayerUpper = new XanimePlayer(MR::getJ3DModel(this), pResource, mXanimePlayer);
    mXanimePlayerUpper->changeAnimation("基本");
}

void GhostPlayer::setAnimation(const char* pName) {
    mXanimePlayer->changeAnimation(pName);
    if (mXanimePlayer->_20->mAttribute == 0) {
        mXanimePlayer->_20->mAttribute = 1;
    }
}

void GhostPlayer::setAnimationWeight(const f32* pWeight) {
    mXanimePlayer->changeTrackWeight(0, pWeight[0]);
    mXanimePlayer->changeTrackWeight(1, pWeight[1]);
    mXanimePlayer->changeTrackWeight(2, pWeight[2]);
    mXanimePlayer->changeTrackWeight(3, pWeight[3]);
}

void GhostPlayer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver->isType(ATYPE_POWER_STAR_BIND)) {
        HitSensor* currentRushSensor = MR::getCurrentRushSensor();
        if ((currentRushSensor == nullptr || !currentRushSensor->isType(ATYPE_POWER_STAR_BIND)) && (!MR::isPlayerConfrontDeath() && !mKilledByStar)) {
            if (strcmp("powerstarget", MR::getPlayerCurrentBckName())) {
                setNerve(&NrvGhostPlayer::HostTypeNrvWinDemo::sInstance);
                mKilledByStar = true;
                MR::preventPlayerRush();
            }
        }
    } else {
        if (isNerve(&NrvGhostPlayer::HostTypeNrvRun::sInstance) && MR::sendMsgPush(pReceiver, pSender) && mAppearStarPieceCooldown == 0) {
            if (MR::appearStarPiece(this, mPosition, 1, 10.0f, 40.0f, false)) {
                if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W");
                } else {
                    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
                }
            }
            mAppearStarPieceCooldown = 5;
        }
        if (LiveActor::isNerve(&NrvGhostPlayer::HostTypeNrvWait::sInstance)) {
            return;
        }
    }
}

bool GhostPlayer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgJetTurtleAttack(msg))
        return true;
    if (MR::isMsgPlayerTrample(msg) && mPlayerTrampleCooldown == 0) {
        TVec3f frontVec;
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);
        MR::calcFrontVec(&frontVec, this);

        if (MR::appearStarPiece(this, mPosition, 5, 10.0f, 40.0f, false)) {
            if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W");
            } else {
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
            }
        }

        mPlayerTrampleCooldown = 10;
        return true;
    }

    return false;
}

bool GhostPlayer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return true;
}

void GhostPlayer::draw() const {
    if (!MR::isDead(this) && !mIsHidden) {
        J3DModelX* model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(this));
        model->viewCalc2();
        GXInvalidateVtxCache();
        model->mFlags.clear();
        model->directDraw(nullptr);
        if (mHasJetTurtle)
            mJetTurtleShadow->drawType1();

        GXSetAlphaUpdate(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        GXSetDstAlpha(GX_FALSE, 0);
    }
}

u32 GhostPlayer::receiveGhostPacket(GhostPacket* pPacket) {
    u8 frameIndex;
    u8 bytesRead;
    u16 updateFlags;

    pPacket->read((s8*)&frameIndex);
    pPacket->read((s8*)&bytesRead);
    pPacket->read((s16*)&updateFlags);

    GhostPacket nextPacket = GhostPacket(pPacket->mDataPtr + bytesRead, 1);
    u8 nextFrameIndex;
    nextPacket.read((s8*)&nextFrameIndex);

    if (++frameIndex != nextFrameIndex) {
        if (mIsDemoCameraActive && MR::isBckOneTimeAndStopped(this)) {
            MR::endActorCamera(this, mCameraInfo, false, -1);
            mIsDemoCameraActive = false;
        }
        return false;
    }

    pPacket->mCurOffs = 4;

    if ((updateFlags & 1) != 0) {
        TVec3s rawPos;
        pPacket->read(&rawPos);
        MR::convToFloat(rawPos, -2, &mPosition);
    }

    if ((updateFlags & 0x800) != 0) {
        TVec3Sc rawVel;
        pPacket->read(&rawVel);
        MR::convToFloat(rawVel, 0, &mVelocity);
    }

    if ((updateFlags & 0x400) != 0) {
        TVec3Sc rawScale;
        pPacket->read(&rawScale);
        MR::convToFloat(rawScale, 3, &mScale);
    }

    if ((updateFlags & 2) != 0) {
        s8 rawRotX;
        f32 rotX;
        pPacket->read(&rawRotX);
        MR::convToFloat(rawRotX, 7, &rotX);
        rotX *= 180.0f;
        rotX = MR::clamp(rotX, -180.0f, 180.0f);
        mTargetRotation.x = rotX;
    }

    if ((updateFlags & 4) != 0) {
        s8 rawRotY;
        f32 rotY;
        pPacket->read(&rawRotY);
        MR::convToFloat(rawRotY, 7, &rotY);
        rotY *= 180.0f;
        rotY = MR::clamp(rotY, -180.0f, 180.0f);
        mTargetRotation.y = rotY;
    }

    if ((updateFlags & 8) != 0) {
        s8 rawRotZ;
        f32 rotZ;
        pPacket->read(&rawRotZ);
        MR::convToFloat(rawRotZ, 7, &rotZ);
        rotZ *= 180.0f;
        rotZ = MR::clamp(rotZ, -180.0f, 180.0f);
        mTargetRotation.z = rotZ;
    }

    if ((updateFlags & 0x0E) != 0) {
        MR::makeMtxRotate(mTargetRotationMtx, mTargetRotation);
    }

    if ((updateFlags & 0x10) != 0) {
        char* animName;
        pPacket->read(&animName);
        setAnimation(animName);
        playSound(this, animName);

        bool isSpecialAnim = false;
        if (!strcmp(animName, "基本")) {
            isSpecialAnim = true;
            setAnimationWeight(&mAnimTrackWeights[0]);
        } else if (!strcmp(animName, "壁押し")) {
            isSpecialAnim = true;
        } else if (!strcmp(animName, "しゃがみ歩き")) {
            isSpecialAnim = true;
        } else if (!strcmp(animName, "がんばり走り")) {
            isSpecialAnim = true;
        }

        if (!isSpecialAnim) {
            mXanimePlayer->_20->mAttribute = 1;
        }
    }

    if ((updateFlags & 0x2000) != 0) {
        u32 animHash;
        pPacket->read(&animHash);
        mXanimePlayer->changeAnimationByHash(animHash);

        bool isSpecialAnim = false;
        const char* currentAnimName = mXanimePlayer->getCurrentAnimationName();
        playSound(this, currentAnimName);

        if (!strcmp(currentAnimName, "基本")) {
            isSpecialAnim = true;
            setAnimationWeight(&mAnimTrackWeights[0]);
        }
        if (!strcmp(currentAnimName, "壁押し"))
            isSpecialAnim = true;
        if (!strcmp(currentAnimName, "しゃがみ歩き"))
            isSpecialAnim = true;
        if (!strcmp(currentAnimName, "がんばり走り"))
            isSpecialAnim = true;
        if (!strcmp(currentAnimName, "スケートアクセルジャンプ"))
            isSpecialAnim = true;
        if (!strcmp(currentAnimName, "スケートジャンプ2"))
            isSpecialAnim = true;
        if (!strcmp(currentAnimName, "スケートジャンプ3"))
            isSpecialAnim = true;

        if (strstr(currentAnimName, "水泳ジェット"))
            mHasJetTurtle = true;
        else if (strstr(currentAnimName, "カメ持ち"))
            mHasJetTurtle = true;
        else if (strstr(currentAnimName, "投げ")) {
            mHasJetTurtle = false;
            mXanimePlayerUpper->stopAnimation();
            MR::getJ3DModelData(this)->getJointTree().getJointNodePointer(MR::getJointIndex(this, "Spine1"))->setMtxCalc(nullptr);
        }

        if (mHasJetTurtle && (strcmp(currentAnimName, "基本") || strcmp(currentAnimName, "ジャンプ"))) {
            mXanimePlayerUpper->changeAnimation("ひろいウエイト");
            mXanimePlayerUpper->overWriteMtxCalc(MR::getJointIndex(this, "PartsControl"));
        }

        if (!isSpecialAnim) {
            mXanimePlayer->_20->mAttribute = 1;
        }
    }

    if ((updateFlags & 0x20) != 0) {
        s16 rawAnimFrame;
        f32 animFrame;
        pPacket->read(&rawAnimFrame);
        MR::convToFloat(rawAnimFrame, 5, &animFrame);
        MR::getBckCtrl(this)->setFrame(animFrame);
    }

    for (u32 i = 0; i < ARRAY_SIZE(mAnimTrackWeights); i++) {
        if ((updateFlags & (0x40 << i)) != 0) {
            s8 rawTrackWeight;
            f32 trackWeight;
            pPacket->read(&rawTrackWeight);
            if ((u8)rawTrackWeight == 0x80)
                rawTrackWeight = 0x7F;
            MR::convToFloat(rawTrackWeight, 7, &trackWeight);
            mXanimePlayer->changeTrackWeight(i, trackWeight);
            mAnimTrackWeights[i] = trackWeight;
        }
    }

    if ((updateFlags & 0x1000) != 0) {
        s8 rawAnimRate;
        f32 animRate;
        pPacket->read(&rawAnimRate);
        MR::convToFloat(rawAnimRate, 3, &animRate);
        MR::setBckRate(this, animRate);
    }

    return bytesRead;
}

f32 getShiftRatio(s8 shiftValue) {
    if (shiftValue > 0)
        return static_cast< f32 >(256 << shiftValue) / 256.0f;
    else
        return static_cast< f32 >(256 >> -shiftValue) / 256.0f;
}

namespace MR {
    void convToFloat(TVec3s& rInVec, s8 shiftValue, TVec3f* pOutVec) {
        f32 factor = 1.0f / getShiftRatio(shiftValue);
        pOutVec->x = static_cast< f32 >(rInVec.x) * factor;
        pOutVec->y = static_cast< f32 >(rInVec.y) * factor;
        pOutVec->z = static_cast< f32 >(rInVec.z) * factor;
    }

    void convToFloat(TVec3Sc& rInVec, s8 shiftValue, TVec3f* pOutVec) {
        f32 factor = 1.0f / getShiftRatio(shiftValue);
        pOutVec->x = static_cast< f32 >(rInVec.x) * factor;
        pOutVec->y = static_cast< f32 >(rInVec.y) * factor;
        pOutVec->z = static_cast< f32 >(rInVec.z) * factor;
    }

    void convToFloat(s16 inShort, s8 shiftValue, f32* pOutFloat) {
        f32 factor = 1.0f / getShiftRatio(shiftValue);
        *pOutFloat = static_cast< f32 >(inShort) * factor;
    }

    void convToFloat(s8 inS8, s8 shiftValue, f32* pOutFloat) {
        f32 factor = 1.0f / getShiftRatio(shiftValue);
        *pOutFloat = static_cast< f32 >(inS8) * factor;
    }
}  // namespace MR

GhostPlayer::~GhostPlayer() {
}

// Unused; Unknown data structure
struct BallData {
    f32 value1;
    u32 value2;
};
BallData ballData[30] = {{0.0f, 0x00000000},  {0.0f, 0x00000000}, {0.0f, 0x00000000}, {0.0f, 0xFFFFFFFF},  {8.0f, 0x0000FFC0},  {0.0f, 0x0000FFC0},
                         {10.0f, 0xFF00FFFF}, {8.0f, 0x0000FFC0}, {0.0f, 0x0000FFC0}, {10.0f, 0xFF00FFFF}, {10.0f, 0x000000FF}, {6.0f, 0x000000FF},
                         {15.0f, 0xFFFF40FF}, {0.0f, 0x00000000}, {0.0f, 0x00000000}, {0.0f, 0x00000000},  {0.0f, 0x00000000},  {0.0f, 0x00000000},
                         {0.0f, 0x00000000},  {0.0f, 0xFF0000C0}, {6.0f, 0xFF0000C0}, {6.0f, 0xFF0000C0},  {0.0f, 0xFF0000C0},  {10.0f, 0xFF0000FF},
                         {0.0f, 0xFF0000C0},  {6.0f, 0xFF0000C0}, {6.0f, 0xFF0000C0}, {0.0f, 0xFF0000C0},  {10.0f, 0xFF0000FF}, {0.0f, 0x00000000}};
