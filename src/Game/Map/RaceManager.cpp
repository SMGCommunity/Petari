#include "Game/Map/RaceManager.hpp"
#include "Game/Util/LayoutUtil.hpp"

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
        {"サーフィン[チャレンジ]", 2, 180, "RaceName_SurfingChallenge", "SurfingLv2Galaxy", 1, false}
    };

    const RaceStructData& getRaceStruceData(s32 id) {
        return sRaceStruct[id];
    }

    RaceManager* getRaceManager() {
        return MR::getSceneObj<RaceManager*>(SceneObj_RaceManager);
    }
};

RaceManagerLayout::RaceManagerLayout(const char* pName) :
    LayoutActor(pName, true)
{
    
}

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
    return MR::isPaneAnimStopped(this, "Go", 0)
        && MR::isPaneAnimStopped(this, "Goal", 0)
        && MR::isPaneAnimStopped(this, "Lose", 0)
        && MR::isPaneAnimStopped(this, "TimeUp", 0)
        && MR::isPaneAnimStopped(this, "ScoreTime", 0)
        && MR::isPaneAnimStopped(this, "Rank", 0);
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
        return MR::isOnGameEventFlagPowerStarSuccess(
            getRaceStruceData(index).mGalaxyName,
            getRaceStruceData(index).mScenarioNo);
    }
};
