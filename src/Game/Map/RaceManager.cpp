#include "Game/Map/RaceManager.hpp"
#include "Game/Map/RaceRail.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    struct RaceStructData {
        /* 0x00 */ const char* mName;
        /* 0x04 */ s32 mEventBgmId;
        /* 0x08 */ s32 mMaxTime;
        /* 0x0C */ const char* mMessageId;
        /* 0x10 */ const char* mGalaxyName;
        /* 0x14 */ s32 mScenarioNo;
        /* 0x18 */ bool mIsDemoWithStarPointer;
    };

    static const RaceStructData sRaceStruct[] = {
        {"ペンギンレース[オーシャンリング]", 2, 120, "RaceName_Penguin", "OceanRingGalaxy", 2, false},
        {"テレサレース[ファントム]", 3, 120, "RaceName_TeresaPhantom", "PhantomGalaxy", 2, true},
        {"テレサレース[デスプロムナード]", 3, 120, "RaceName_TeresaDeathPromenade", "TeresaMario2DGalaxy", 1, false},
        {"サーフィン[トライアル]", 2, 0, "RaceName_SurfingTrial", "SurfingLv1Galaxy", 1, false},
        {"サーフィン[チャレンジ]", 2, 180, "RaceName_SurfingChallenge", "SurfingLv2Galaxy", 1, false}};

    const RaceStructData& getRaceStruceData(s32 id) {
        return sRaceStruct[id];
    }

    RaceManager* getRaceManager() {
        return MR::getSceneObj< RaceManager >(SceneObj_RaceManager);
    }
};  // namespace

RaceManagerLayout::RaceManagerLayout(const char* pName) : LayoutActor(pName, true) {}

void RaceManagerLayout::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("Race", 1);
    MR::createAndAddPaneCtrl(this, "Go", 1);
    MR::createAndAddPaneCtrl(this, "Goal", 1);
    MR::createAndAddPaneCtrl(this, "Lose", 1);
    MR::createAndAddPaneCtrl(this, "TimeUp", 1);
    MR::createAndAddPaneCtrl(this, "Rank", 1);
    MR::createAndAddPaneCtrl(this, "ScoreTime", 1);
    initEffectKeeper(2, "Race", nullptr);
    hideAllPane();
}

void RaceManagerLayout::playCountAndGo() {
    MR::showPane(this, "Go");
    MR::showPane(this, "BestRecord");
    MR::startPaneAnim(this, "Go", "Count", 0);
}

void RaceManagerLayout::playGo() {
    MR::showPane(this, "Go");
    MR::startPaneAnim(this, "Go", "Count", 0);
    MR::setPaneAnimFrame(this, "Go", 180.0f, 0);
    MR::emitEffect(this, "RaceStart");
}

void RaceManagerLayout::playGoal() {
    MR::showPane(this, "Goal");
    MR::startPaneAnim(this, "Goal", "Goal", 0);
    MR::startPaneAnim(this, "ScoreTime", "Goal", 0);
    MR::emitEffect(this, "RaceGoal");
}

void RaceManagerLayout::playLose() {
    MR::showPane(this, "Lose");
    MR::startPaneAnim(this, "Lose", "Lose", 0);
    MR::startPaneAnim(this, "ScoreTime", "Lose", 0);
}

void RaceManagerLayout::playTimeUp() {
    MR::showPane(this, "TimeUp");
    MR::startPaneAnim(this, "TimeUp", "TimeUp", 0);
    MR::startPaneAnim(this, "ScoreTime", "TimeUp", 0);
}

void RaceManagerLayout::playRank(u32 rank) {
    MR::showPane(this, "Rank");
    MR::startPaneAnim(this, "Rank", "Rank", 0);
    MR::setPaneAnimFrameAndStop(this, "Rank", rank, 0);
}

bool RaceManagerLayout::isAllAnimStopped() {
    return MR::isPaneAnimStopped(this, "Go", 0) && MR::isPaneAnimStopped(this, "Goal", 0) && MR::isPaneAnimStopped(this, "Lose", 0) &&
           MR::isPaneAnimStopped(this, "TimeUp", 0) && MR::isPaneAnimStopped(this, "ScoreTime", 0) && MR::isPaneAnimStopped(this, "Rank", 0);
}

void RaceManagerLayout::hideRecordPane() {
    MR::hidePane(this, "Record");
}

void RaceManagerLayout::hideBestRecordPane() {
    MR::hidePane(this, "BestRecord");
}

void RaceManagerLayout::hideAllPane() {
    MR::hidePane(this, "Go");
    MR::hidePane(this, "Goal");
    MR::hidePane(this, "Lose");
    MR::hidePane(this, "TimeUp");
    MR::hidePane(this, "Record");
    MR::hidePane(this, "BestRecord");
    MR::hidePane(this, "Rank");
}

bool RaceManagerLayout::isPlayCountAnim() const {
    return MR::getPaneAnimFrame(this, "Go", 0) < 180.0f;
}

bool RaceManagerLayout::isPlayCountAndGoAnim() const {
    return !MR::isPaneAnimStopped(this, "Go", 0);
}

void RaceManagerLayout::setTime(u32 time) {
    MR::updateClearTimeTextBox(this, "TimeCounter", time);
}

void RaceManagerLayout::setBestTime(u32 bestTime) {
    MR::updateClearTimeTextBox(this, "BestTimeCounter", bestTime);
}

namespace NrvRaceManager {
    NEW_NERVE(RaceManagerNrvWait, RaceManager, Wait);
    NEW_NERVE(RaceManagerNrvPrepWipe, RaceManager, Prep);
    NEW_NERVE(RaceManagerNrvPrepImme, RaceManager, Prep);
    NEW_NERVE(RaceManagerNrvPreWipeOut, RaceManager, WipeOut);
    NEW_NERVE(RaceManagerNrvPreWipeWait, RaceManager, WipeWait);
    NEW_NERVE(RaceManagerNrvPreWipeIn, RaceManager, WipeIn);
    NEW_NERVE(RaceManagerNrvIntro, RaceManager, Intro);
    NEW_NERVE(RaceManagerNrvCount, RaceManager, Count);
    NEW_NERVE(RaceManagerNrvRace, RaceManager, Race);
    NEW_NERVE(RaceManagerNrvDemo, RaceManager, Demo);
    NEW_NERVE(RaceManagerNrvGoal, RaceManager, Goal);
    NEW_NERVE(RaceManagerNrvRank, RaceManager, Rank);
    NEW_NERVE(RaceManagerNrvPstWipeOut, RaceManager, WipeOut);
    NEW_NERVE(RaceManagerNrvPstWipeWait, RaceManager, WipeWait);
    NEW_NERVE(RaceManagerNrvPstWipeIn, RaceManager, WipeIn);
};  // namespace NrvRaceManager

RaceManager::RaceManager()
    : LiveActor("レース管理"), mLayout(nullptr), mRacerNum(0), mAudienceNum(0), mRank(0), mBestTime(0), mTime(0), mPlayerRacer(nullptr) {
    _FC.identity();
}

void RaceManager::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);

    mLayout = new RaceManagerLayout("レース管理用レイアウト");
    mLayout->init(rIter);

    MR::invalidateClipping(this);
    initNerve(&NrvRaceManager::RaceManagerNrvWait::sInstance);
    makeActorAppeared();
}

void RaceManager::movement() {
    LiveActor::movement();
}

void RaceManager::exeWait() {}

void RaceManager::exePrep() {
    // std::for_each(&mRacer[0], &mRacer[mRacerNum], std::mem_fun(&AbstractRacer::initRacer));

    if (isNerve(&NrvRaceManager::RaceManagerNrvPrepWipe::sInstance)) {
        setNerve(&NrvRaceManager::RaceManagerNrvPreWipeOut::sInstance);
    } else {
        prepBind();
        setNerve(&NrvRaceManager::RaceManagerNrvIntro::sInstance);
    }
}

void RaceManager::exeWipeOut() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvRaceManager::RaceManagerNrvPreWipeOut::sInstance)) {
            MR::stopStageBGM(90);
        }

        MR::closeWipeFade(-1);
    }

    if (MR::isWipeActive()) {
        return;
    }

    if (isNerve(&NrvRaceManager::RaceManagerNrvPreWipeOut::sInstance)) {
        prepRace();
        setNerve(&NrvRaceManager::RaceManagerNrvPreWipeWait::sInstance);
    } else {
        resetRace();
        setNerve(&NrvRaceManager::RaceManagerNrvPstWipeWait::sInstance);
    }
}

void RaceManager::exeWipeWait() {
    if (!MR::isStep(this, 30)) {
        return;
    }

    if (isNerve(&NrvRaceManager::RaceManagerNrvPreWipeWait::sInstance)) {
        setNerve(&NrvRaceManager::RaceManagerNrvPreWipeIn::sInstance);
    } else {
        setNerve(&NrvRaceManager::RaceManagerNrvPstWipeIn::sInstance);
    }
}

void RaceManager::exeWipeIn() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvRaceManager::RaceManagerNrvPreWipeIn::sInstance)) {
            MR::startSubBGM("BGM_MINIGAME_START", false);
        } else if (!MR::isPlayerDead()) {
            MR::startCurrentStageBGM();
        }

        MR::openWipeFade(-1);
    }

    if (MR::isWipeActive()) {
        return;
    }

    if (isNerve(&NrvRaceManager::RaceManagerNrvPreWipeIn::sInstance)) {
        setNerve(&NrvRaceManager::RaceManagerNrvIntro::sInstance);
    } else {
        MR::onPlayerControl(true);
        MR::endDemo(this, "レース");
        // std::for_each(&mRacer[0], &mRacer[mRacerNum], std::mem_fun(&AbstractRacer::exitRacer));
        setNerve(&NrvRaceManager::RaceManagerNrvWait::sInstance);
    }
}

void RaceManager::exeIntro() {
    if (MR::isFirstStep(this)) {
        if (getRaceStruceData(mCurrentRace).mIsDemoWithStarPointer) {
            MR::startStarPointerModeDemoWithStarPointer(this);
        } else {
            MR::startStarPointerModeDemo(this);
        }
    }

    if (MR::isStep(this, 110)) {
        setNerve(&NrvRaceManager::RaceManagerNrvCount::sInstance);
    }
}

void RaceManager::exeCount() {
    if (MR::isFirstStep(this)) {
        mLayout->setTime(0);
        mLayout->setBestTime(mBestTime);
        mLayout->appear();
        mLayout->playCountAndGo();
        MR::showPane(mLayout, "Record");
    }

    mLayout->movement();

    if (getNerveStep() % 60 == 0) {
        MR::startSystemSE("SE_SY_RACE_COUNT_DOWN", -1, -1);
    }

    if (MR::isStep(this, 166)) {
        MR::startEventBGM(getRaceStruceData(mCurrentRace).mEventBgmId);
    }

    if (mLayout->isPlayCountAnim()) {
        return;
    }

    mLayout->playGo();
    startRace();
    setNerve(&NrvRaceManager::RaceManagerNrvRace::sInstance);
}

// RaceManager::exeRace

void RaceManager::exeDemo() {
    if (!MR::canStartDemo()) {
        return;
    }

    goalRace();
    MR::offPlayerControl();
    setNerve(&NrvRaceManager::RaceManagerNrvGoal::sInstance);
}

void RaceManager::exeGoal() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(30);

        if (mRank == 0) {
            MR::startSubBGM("BGM_RACE_LOSE", false);
            mLayout->playTimeUp();
        } else if (mRank == 1) {
            MR::startSubBGM("BGM_RACE_WIN", false);
            mLayout->playGoal();
        } else {
            MR::startSubBGM("BGM_RACE_LOSE", false);
            mLayout->playLose();
        }
    }

    if (mLayout->isAllAnimStopped()) {
        setNerve(&NrvRaceManager::RaceManagerNrvRank::sInstance);
    }
}

void RaceManager::exeRank() {
    if (MR::isLessStep(this, 60)) {
        return;
    }

    if (!MR::canStartDemo()) {
        return;
    }

    MR::tryStartDemoWithoutCinemaFrame(this, "レース");
    setNerve(&NrvRaceManager::RaceManagerNrvPstWipeOut::sInstance);
}

bool RaceManager::startWithWipe() {
    mRank = 0;

    mLayout->hideAllPane();
    MR::requestStartDemoMarioPuppetableWithoutCinemaFrame(this, "レース", &NrvRaceManager::RaceManagerNrvPrepWipe::sInstance, nullptr);

    return true;
}

bool RaceManager::startImmediately() {
    mRank = 0;

    mLayout->hideAllPane();
    MR::requestStartDemoWithoutCinemaFrame(this, "レース", &NrvRaceManager::RaceManagerNrvPrepImme::sInstance, nullptr);
    MR::requestMovementOnPlayer();
    MR::stopStageBGM(90);
    MR::startSubBGM("BGM_MINIGAME_START", false);

    return true;
}

void RaceManager::prepRace() {
    mPlayerRacer->prepRacer(this);

    for (s32 i = 0; i < mRacerNum; i++) {
        AbstractRacer* pRacer = mRacer[i];

        if (pRacer != mPlayerRacer) {
            pRacer->prepRacer(this);
        }
    }

    // std::for_each(&mAudience[0], &mAudience[mAudienceNum], std::mem_fun(&AbstractAudience::prepAudience));
}

void RaceManager::startRace() {
    if (MR::isPlayerInBind()) {
        MR::sendSimpleMsgToActor(ACTMES_RACE_START, MR::getCurrentRushActor());
    }

    // std::for_each(&mRacer[0], &mRacer[mRacerNum], std::mem_fun(&AbstractRacer::startRacer));
    MR::endStarPointerMode(this);
    MR::endDemo(this, "レース");
    MR::onPlayerControl(true);
}

void RaceManager::goalRace() {
    if (MR::isPlayerInBind()) {
        MR::sendSimpleMsgToActor(ACTMES_RACE_GOAL, MR::getCurrentRushActor());
    }

    // std::for_each(&mRacer[mRank], &mRacer[mRacerNum], std::mem_fun(&AbstractRacer::loseRacer));
}

void RaceManager::resetRace() {
    if (MR::isPlayerInBind()) {
        MR::sendSimpleMsgToActor(ACTMES_RACE_RESET, MR::getCurrentRushActor());
    }

    mPlayerRacer->resetRacer(this);

    for (s32 i = 0; i < mRacerNum; i++) {
        AbstractRacer* pRacer = mRacer[i];

        if (pRacer != mPlayerRacer) {
            pRacer->resetRacer(this);
        }
    }

    // std::for_each(&mAudience[0], &mAudience[mAudienceNum], std::mem_fun(&AbstractAudience::resetAudience));
    mLayout->kill();
}

void RaceManager::prepBind() {
    if (!MR::isPlayerInBind()) {
        return;
    }

    MR::requestMovementOn(MR::getCurrentRushActor());
    MR::sendSimpleMsgToActor(ACTMES_RACE_READY, MR::getCurrentRushActor());
}

void RaceManager::entry(AbstractRacer* pRacer) {
    mRacer[mRacerNum++] = pRacer;
}

// RaceManager::entry

void RaceManager::entry(AbstractAudience* pAudience) {
    mAudience[mAudienceNum++] = pAudience;
}

// RaceManager::isGoal

u32 RaceManager::getRank(const AbstractRacer* pRacer) const {
    u32 rank = mRacerNum;

    for (int i = 0; i < mRacerNum; i++) {
        if (mRacer[i] == pRacer) {
            rank = i;
        }
    }

    return rank + 1;
}

void RaceManager::renewTime() {
    mTime = getNerveStep();

    if (mTime < mBestTime) {
        MR::setRaceBestTime(mCurrentRace, mTime);
    }
}

namespace RaceManagerFunction {
    void entryRacerOthers(AbstractRacer* pRacer) {
        MR::createSceneObj(SceneObj_RaceManager);
        getRaceManager()->entry(pRacer);
    }

    void entryAudience(AbstractAudience* pAudience) {
        MR::createSceneObj(SceneObj_RaceManager);
        getRaceManager()->entry(pAudience);
    }

    void entryRacerPlayer(PlayerRacer* pRacer) {
        MR::createSceneObj(SceneObj_RaceManager);
        getRaceManager()->entry(pRacer);
    }

    void startRaceWithWipe() {
        getRaceManager()->startWithWipe();
    }

    void startRaceImmediately() {
        getRaceManager()->startImmediately();
    }

    u32 getRaceRank() {
        return getRaceManager()->mRank;
    }

    u32 getRaceTime() {
        return getRaceManager()->mTime;
    }

    const char* getRaceName(int index) {
        return getRaceStruceData(index).mName;
    }

    const char* getRaceMessageId(int index) {
        return getRaceStruceData(index).mMessageId;
    }

    s32 getRaceId(const char* pGalaxyName, s32 scenarioNo) {
        for (s32 i = 0; i < sizeof(sRaceStruct) / sizeof(*sRaceStruct); i++) {
            const RaceStructData& rRaceStructData = getRaceStruceData(i);

            if (!MR::isEqualString(rRaceStructData.mGalaxyName, pGalaxyName)) {
                continue;
            }

            if (rRaceStructData.mScenarioNo != scenarioNo) {
                continue;
            }

            return i;
        }

        return -1;
    }

    bool hasPowerStarRaceScenario(int index) {
        return MR::isOnGameEventFlagPowerStarSuccess(getRaceStruceData(index).mGalaxyName, getRaceStruceData(index).mScenarioNo);
    }
};  // namespace RaceManagerFunction
