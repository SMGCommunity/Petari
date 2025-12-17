#include "Game/System/StorySequenceExecutor.hpp"
#include "Game/Demo/PrologueDirector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Screen/StaffRoll.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/StageResultSequenceChecker.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    const StorySequenceExecutorType::DemoSequenceInfo cDemoPowerStarResultAstroDome[] = {
        {
            0,
            1,
            "天文ドームスター帰還",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoPowerStarResultAstroDomeWithSave[] = {
        {
            0,
            1,
            "天文ドームスター帰還",
        },
        {
            5,
            0,
            "Save",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoPowerStarResultAstroGalaxy[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGrandStarResult[] = {
        {
            0,
            1,
            "グランドスター帰還[２回目以降]",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoPrologue[] = {
        {
            8,
            1,
            "Prologue",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGrandStar1ResultFirst[] = {
        {
            0,
            1,
            "グランドスター１帰還",
        },
        {
            0,
            1,
            "ロゼッタ状況説明デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoKinopioRescued[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            0,
            1,
            "ロゼッタキノピオ探検隊デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoTorchLecture[] = {
        {
            0,
            1,
            "グランドスター２帰還",
        },
        {
            0,
            1,
            "ロゼッタトーチの炎説明デモ",
        },
        {
            0,
            1,
            "バトラーマップレクチャー",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoTrickCometLecture[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            0,
            1,
            "ロゼッタコメット説明デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkAboutKoopa[] = {
        {
            0,
            1,
            "グランドスター３帰還",
        },
        {
            0,
            1,
            "ロゼッタ銀河の中心説明デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoLuigiRescued[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            0,
            1,
            "ロゼッタルイージデモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkAstroDomeRecover[] = {
        {
            0,
            1,
            "グランドスター４帰還",
        },
        {
            0,
            1,
            "ロゼッタ天文台機能回復デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGrandStarReturn5[] = {
        {
            0,
            1,
            "グランドスター５帰還",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkCountDownStart[] = {
        {
            0,
            1,
            "ロゼッタカウントダウン開始デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGrandStarReturn6[] = {
        {
            0,
            1,
            "グランドスター６帰還",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkTorchProgress[] = {
        {
            0,
            1,
            "ロゼッタトーチの炎進捗デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkKoopaBattleVs3GalaxyAppear[] = {
        {
            5,
            0,
            "Save",
        },
        {
            0,
            1,
            "ロゼッタ最終決戦デモ",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoStarPieceLectureRetry[] = {
        {
            0,
            3,
            "スターピース解説前半",
        },
        {
            1,
            1,
            "Fadein",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoNewGalaxyDiscover[] = {
        {
            0,
            1,
            "天文ドームスター帰還",
        },
        {
            0,
            4,
            "バトラー報告",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGreenStarGetFirst[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            0,
            7,
            "バトラーグリーンドライバ説明",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoGreenStarRelease[] = {
        {
            0,
            1,
            "パワースター帰還",
        },
        {
            0,
            8,
            "バトラーグリーンドライバ説明",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoAstroDomeLecture[] = {
        {
            0,
            2,
            "ドームレクチャー１",
        },
        {
            7,
            0,
            "バトラー情報Ａ",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoLuigiMissing[] = {
        {
            0,
            1,
            "ルイージ失踪デモ",
        },
        {
            13,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoEpilogueNormal[] = {
        {
            10,
            14,
        },
        {
            2,
            9,
            "MovieEpilogueA",
        },
        {
            0,
            1,
            "エピローグデモ",
        },
        {
            11,
            15,
        },
        {
            2,
            10,
            "MovieEndingA",
        },
        {
            9,
            12,
        },
        {
            11,
            16,
        },
        {
            9,
            13,
        },
        {
            4,
            1,
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoEpilogueComplete[] = {
        {
            10,
            14,
        },
        {
            2,
            9,
            "MovieEpilogueA",
        },
        {
            0,
            1,
            "エピローグデモ",
        },
        {
            11,
            15,
        },
        {
            2,
            10,
            "MovieEndingA",
        },
        {
            9,
            12,
        },
        {
            2,
            11,
            "MovieEndingB",
        },
        {
            11,
            16,
        },
        {
            9,
            13,
        },
        {
            4,
            1,
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaPicureBookTalk[] = {
        {0, 1, "ロゼッタ朗読"},
        {
            1,
            1,
            "Fadein",
        },
        {
            12,
        },
    };
    const StorySequenceExecutorType::DemoSequenceInfo cDemoRosettaTalkAfterNormalEnding[] = {
        {0, 1, "ロゼッタノーマルエンディング後デモ"},
        {
            6,
            1,
            "RosettaTalkAfterNormalEnding",
        },
        {
            1,
            1,
            "Fadein",
        },
        {
            12,
        },
    };
    const DemoFortressDiscoverCheckList cDemoFortressDiscoverCheckListTable[] = {
        {
            "TriLegLv1Galaxy",
            "クッパＪｒロボプラント発見",
            false,
        },
        {
            "KoopaBattleVs1Galaxy",
            "クッパスタープラント発見",
            true,
        },
        {
            "KoopaJrShipLv1Galaxy",
            "クッパＪｒシッププラント発見",
            false,
        },
        {
            "KoopaBattleVs2Galaxy",
            "クッパダークマタープラント発見",
            true,
        },
        {
            "FloaterOtaKingGalaxy",
            "クッパＪｒクリーチャープラント発見",
            false,
        },
    };
};  // namespace

namespace {
    NEW_NERVE(StorySequenceExecutorIdle, StorySequenceExecutor, Idle);
    NEW_NERVE(StorySequenceExecutorWaitToSceneStart, StorySequenceExecutor, WaitToSceneStart);
    NEW_NERVE(StorySequenceExecutorPlayDemoSequence, StorySequenceExecutor, PlayDemoSequence);
    NEW_NERVE(StorySequenceExecutorWaitTimeKeepDemoEnd, StorySequenceExecutor, WaitTimeKeepDemoEnd);
    NEW_NERVE(StorySequenceExecutorWaitSaveEnd, StorySequenceExecutor, WaitSaveEnd);
    NEW_NERVE(StorySequenceExecutorStartSaveAfterSequence, StorySequenceExecutor, StartSaveAfterSequence);
    NEW_NERVE(StorySequenceExecutorWaitSaveEndAfterSequence, StorySequenceExecutor, WaitSaveEndAfterSequence);
    NEW_NERVE(StorySequenceExecutorFadeinAfterSequence, StorySequenceExecutor, FadeinAfterSequence);
    NEW_NERVE(StorySequenceExecutorStaffRollSequence, StorySequenceExecutor, StaffRollSequence);
};  // namespace

StorySequenceExecutor::StorySequenceExecutor()
    : NerveExecutor("StorySequenceExecutor"), mNextNerve(nullptr), mDemoObj(nullptr), mSaveObj(nullptr), _48(), _6C(), _B0(false) {
    mDemoObj = new NameObj("StorySequenceExecutor");
    mSaveObj = new NameObj("Save");
    mStageName[0] = '\0';
    mScenarioNo = -1;

    initNerve(&::StorySequenceExecutorIdle::sInstance);
}

void StorySequenceExecutor::update() {
    updateNerve();
}

bool StorySequenceExecutor::isNeedMoviePlayerExecutingEventEnum() const {
    for (s32 i = 0; i < _48.size(); i++) {
        bool b = _48[i]->_0 == 12 || _48[i]->_0 == 13;

        if (b) {
            continue;
        }

        if (_48[i]->_0 == 2) {
            return true;
        }
    }

    return false;
}

bool StorySequenceExecutor::hasNextDemo() const {
    for (s32 i = 0; i < _48.size(); i++) {
        bool b = _48[i]->_0 == 12 || _48[i]->_0 == 13;

        if (b) {
            continue;
        }

        if (_48[i]->_0 == 0) {
            return true;
        }
    }

    return false;
}

s32 StorySequenceExecutor::getExecutingEventEnum() const {
    if (getCurrentDemoInfo() == nullptr) {
        return 0;
    }

    return getCurrentDemoInfo()->_2;
}

bool StorySequenceExecutor::isEqualStageScenarioBefore(const char* pStageName, int scenarioNo) const {
    return MR::isEqualString(pStageName, mStageName) && scenarioNo == mScenarioNo;
}

void StorySequenceExecutor::moveGalaxy(GalaxyMoveArgument* pMoveArgument, bool param2) {
    setBeforeStageScenario(*pMoveArgument, param2);

    if (pMoveArgument->isEqualStage("AstroGalaxy") && pMoveArgument->mScenarioNo != 4) {
        pMoveArgument->mScenarioNo = calcAproposScenarioNoOnAstroGalaxy();
    }

    if (pMoveArgument->isEqualStageScenario("AstroDome", -1)) {
        pMoveArgument->mScenarioNo = calcAproposScenarioNoOnAstroDome();
    }

    _6C.clear();
    _48.clear();

    _B0 = false;

    switch (pMoveArgument->mMoveType) {
    case 0:
        decideNextEventForMoveGalaxy(pMoveArgument);
        break;
    case 1:
        if (pMoveArgument->isEqualStageScenario("PeachCastleGardenGalaxy", 1)) {
            prepareDemoSequence(::cDemoPrologue);
        }
        break;
    case 3:
        decideNextStageForGalaxyOut(pMoveArgument);
        break;
    case 4:
        decideNextEventForClearGalaxy(pMoveArgument);
        decideNextEventForMoveGalaxy(pMoveArgument);
        break;
    case 6:
        overwriteGalaxyNameAfterLoading(pMoveArgument);
        decideNextEventForAfterLoading(pMoveArgument);
        break;
    case 7:
        pMoveArgument->mStageName = "FileSelect";
        pMoveArgument->mScenarioNo = 1;
        break;
    default:
        break;
    }

    if (isEqualStageStopCometScheduler(pMoveArgument->mStageName)) {
        GameSequenceFunction::deactivateGalaxyCometScheduler();
    }
}

void StorySequenceExecutor::setNerveSceneStart() {
    if (!isEqualStageStopCometScheduler(MR::getCurrentStageName())) {
        GameSequenceFunction::activateGalaxyCometScheduler();
    }

    if (mNextNerve != nullptr) {
        setNerve(mNextNerve);

        mNextNerve = nullptr;
    }
}

void StorySequenceExecutor::forceStop() {
    _48.clear();

    _B0 = false;
    mNextNerve = nullptr;

    setNerve(&::StorySequenceExecutorIdle::sInstance);
}

void StorySequenceExecutor::exeIdle() {}

void StorySequenceExecutor::exeWaitToSceneStart() {}

void StorySequenceExecutor::exePlayDemoSequence() {
    const StorySequenceExecutorType::DemoSequenceInfo* pDemoInfo = getCurrentDemoInfo();

    if (MR::isFirstStep(this)) {
    }

    switch (pDemoInfo->_0) {
    case 0:
        if (tryStartDemo(pDemoInfo->_4)) {
            setNerve(&::StorySequenceExecutorWaitTimeKeepDemoEnd::sInstance);
        }
        break;
    case 1:
        if (tryStartFadein()) {
            _48[0]++;

            setNerve(&::StorySequenceExecutorPlayDemoSequence::sInstance);
        }
        break;
    case 2:
        if (tryStartMovieAndWaitEnd(pDemoInfo->_2)) {
            _48[0]++;

            setNerve(&::StorySequenceExecutorPlayDemoSequence::sInstance);
        }
        break;
    case 3:
        MR::requestChangeScene(pDemoInfo->_4);
        break;
    case 4:
        MR::requestChangeSceneTitle();
        forceStop();
        break;
    case 5:
        if (tryStartSave()) {
            setNerve(&::StorySequenceExecutorWaitSaveEnd::sInstance);
        }
        break;
    case 6:
        GameDataFunction::onGameEventFlag(pDemoInfo->_4);
        _48[0]++;
        exePlayDemoSequence();
        break;
    case 7:
        GameDataFunction::followStoryEventByName(pDemoInfo->_4);
        _48[0]++;
        exePlayDemoSequence();
        break;
    case 8:
        MR::forceOpenWipeFade();
        MR::startPrologue();
        _48[0]++;
        exePlayDemoSequence();
        break;
    case 9:
        GameSceneFunction::requestStaffRoll();
        setNerve(&::StorySequenceExecutorStaffRollSequence::sInstance);
        break;
    case 10:
        if (pDemoInfo->_2 == 14) {
            MR::startStarPointerModeEnding(this);
        }

        _48[0]++;
        exePlayDemoSequence();
        break;
    case 11:
        if (pDemoInfo->_2 == 15) {
            GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(false);
        } else if (pDemoInfo->_2 == 16) {
            GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(true);
        }

        _48[0]++;
        exePlayDemoSequence();
        break;
    case 12:
        if (tryNextDemoInfo() != nullptr) {
            exePlayDemoSequence();
        } else {
            _48.clear();

            _B0 = false;

            setNerve(&::StorySequenceExecutorIdle::sInstance);
        }
        break;
    case 13:
        if (tryNextDemoInfo() != nullptr) {
            exePlayDemoSequence();
        } else {
            const Nerve* pNerve;

            if (_B0) {
                pNerve = &::StorySequenceExecutorFadeinAfterSequence::sInstance;
            } else {
                pNerve = &::StorySequenceExecutorStartSaveAfterSequence::sInstance;
            }

            setNerve(pNerve);
        }
        break;
    default:
        break;
    }
}

void StorySequenceExecutor::exeWaitTimeKeepDemoEnd() {
    if (MR::isDemoActive()) {
        return;
    }

    if (MR::isEqualString(getCurrentDemoInfo()->_4, "エピローグデモ")) {
        MR::offPlayerControl();
    }

    _48[0]++;

    setNerve(&::StorySequenceExecutorPlayDemoSequence::sInstance);
}

void StorySequenceExecutor::exeWaitSaveEnd() {
    if (GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        return;
    }

    MR::endDemo(mSaveObj, "セーブ");

    _48[0]++;

    setNerve(&::StorySequenceExecutorPlayDemoSequence::sInstance);
}

void StorySequenceExecutor::exeStartSaveAfterSequence() {
    if (tryStartSave()) {
        setNerve(&::StorySequenceExecutorWaitSaveEndAfterSequence::sInstance);
    }
}

void StorySequenceExecutor::exeWaitSaveEndAfterSequence() {
    if (tryWaitSaveEnd()) {
        setNerve(&::StorySequenceExecutorFadeinAfterSequence::sInstance);
    }
}

void StorySequenceExecutor::exeFadeinAfterSequence() {
    if (!MR::isFirstStep(this) || !MR::isWipeOpen()) {
        if (!tryStartFadein()) {
            return;
        }
    }

    _48.clear();
    _B0 = false;

    MR::endStartPosCamera();
    setNerve(&::StorySequenceExecutorIdle::sInstance);
}

void StorySequenceExecutor::exeStaffRollSequence() {
    StaffRoll* pStaffRoll = MR::getStaffRoll();

    if (MR::isFirstStep(this)) {
        MR::deactivateDefaultGameLayout();
        MR::offPlayerControl();
        MR::openWipeFade(-1);

        const StorySequenceExecutorType::DemoSequenceInfo* pDemoInfo = getCurrentDemoInfo();

        if (pDemoInfo->_2 == 12) {
            pStaffRoll->appear();
        } else if (pDemoInfo->_2 == 13) {
            pStaffRoll->startInfo();
        }
    }

    if (!pStaffRoll->isPauseOrEnd()) {
        return;
    }

    MR::closeWipeFade(-1);

    _48[0]++;

    setNerve(&::StorySequenceExecutorPlayDemoSequence::sInstance);
}

void StorySequenceExecutor::decideNextEventForClearGalaxy(GalaxyMoveArgument* pMoveArgument) {
    pMoveArgument->mStageName = nullptr;
    pMoveArgument->mScenarioNo = -1;

    if (GameSequenceFunction::hasStageResultSequence()) {
        if (isEqualStageScenarioBefore("KoopaBattleVs3Galaxy", 1)) {
            if (MR::isStarCompleteAllGalaxy()) {
                prepareDemoSequence(::cDemoEpilogueComplete);
            } else {
                prepareDemoSequence(::cDemoEpilogueNormal);
            }

            pMoveArgument->mStageName = "EpilogueDemoStage";
            pMoveArgument->mScenarioNo = 1;
        } else {
            StageResultSequenceChecker stageResultSequenceChecker = StageResultSequenceChecker();
            stageResultSequenceChecker.check();

            calcAproposScenarioNoOnAstroDome();

            if (!GameSequenceFunction::hasPowerStarYetAtResultSequence()) {
                if (isEqualStageScenarioBefore("HeavensDoorGalaxy", 1)) {
                    setNextStageToAstroGalaxy(pMoveArgument);
                    prepareDemoSequence(::cDemoGrandStar1ResultFirst);
                }

                if (isEqualStageScenarioBefore("EggStarGalaxy", 1)) {
                    prepareDemoSequence(::cDemoPowerStarResultAstroDomeWithSave);
                    setNextStageToAstroDome(pMoveArgument);
                }

                if (isEqualStageScenarioBefore("EggStarGalaxy", 2)) {
                    prepareDemoSequence(::cDemoNewGalaxyDiscover);
                    setNextStageToAstroDome(pMoveArgument);
                }

                if (GameDataFunction::isOnJustGameEventFlag("EventKinopioExplorerRescued")) {
                    prepareDemoSequence(::cDemoKinopioRescued);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (isEqualStageScenarioBefore("TriLegLv1Galaxy", 1)) {
                    prepareDemoSequence(::cDemoTorchLecture);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (GameDataFunction::isOnJustGameEventFlag("EventCometStarter")) {
                    prepareDemoSequence(::cDemoTrickCometLecture);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (isEqualStageScenarioBefore("KoopaBattleVs1Galaxy", 1)) {
                    prepareDemoSequence(::cDemoRosettaTalkAboutKoopa);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (GameDataFunction::isOnJustGameEventFlag("SpecialStarLuigiRescued")) {
                    prepareDemoSequence(::cDemoLuigiRescued);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (isEqualStageScenarioBefore("KoopaJrShipLv1Galaxy", 1)) {
                    prepareDemoSequence(::cDemoRosettaTalkAstroDomeRecover);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (isEqualStageScenarioBefore("KoopaBattleVs2Galaxy", 1)) {
                    prepareDemoSequence(::cDemoGrandStarReturn5);
                    setNextStageToAstroGalaxy(pMoveArgument);
                    prepareDemoSequence(::cDemoRosettaTalkCountDownStart);
                }

                if (isEqualStageScenarioBefore("FloaterOtaKingGalaxy", 1)) {
                    prepareDemoSequence(::cDemoGrandStarReturn6);
                    setNextStageToAstroGalaxy(pMoveArgument);

                    if (!GameDataFunction::canOnGameEventFlag("KoopaBattleVs3Galaxy")) {
                        prepareDemoSequence(::cDemoRosettaTalkTorchProgress);
                    }
                }

                if (stageResultSequenceChecker.isJustGetGreenStarFirst()) {
                    prepareDemoSequence(::cDemoGreenStarGetFirst);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (GameDataFunction::isOnJustGameEventFlag("SpecialStarGreenAll")) {
                    prepareDemoSequence(::cDemoGreenStarRelease);
                    setNextStageToAstroGalaxy(pMoveArgument);
                }

                if (_48.size() > 0) {
                    MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > > arr =
                        MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >();

                    getOptionalDemoForClearGalaxy(&arr, pMoveArgument);
                    prepareDemoSequence(arr);
                    return;
                }
            }

            MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > > arr =
                MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >();

            getOptionalDemoForClearGalaxy(&arr, pMoveArgument);

            if (GameDataConst::isGrandStar(mStageName, mScenarioNo)) {
                prepareDemoSequence(::cDemoGrandStarResult);
                setNextStageToAstroGalaxy(pMoveArgument);
            } else {
                if (arr.size() > 0) {
                    setNextStageToAstroGalaxy(pMoveArgument);
                } else {
                    setNextStageToAstroGalaxyOrDome(pMoveArgument);
                }

                if (pMoveArgument->isEqualStage("AstroDome")) {
                    prepareDemoSequence(::cDemoPowerStarResultAstroDome);
                } else {
                    prepareDemoSequence(::cDemoPowerStarResultAstroGalaxy);
                }
            }

            prepareDemoSequence(arr);
        }
    } else {
        mNextNerve = &::StorySequenceExecutorIdle::sInstance;

        setNerve(&::StorySequenceExecutorWaitToSceneStart::sInstance);
    }
}

void StorySequenceExecutor::decideNextEventForMoveGalaxy(GalaxyMoveArgument* pMoveArgument) {
    if (pMoveArgument->isEqualStage("AstroDome")) {
        if (!GameDataFunction::isPassedStoryEvent("バトラー情報Ａ")) {
            prepareDemoSequence(::cDemoAstroDomeLecture);
            return;
        }

        if (GameDataFunction::hasPowerStar("EggStarGalaxy", 1) && !GameDataFunction::isPassedStoryEvent("スターピースレクチャー")) {
            prepareDemoSequence(::cDemoStarPieceLectureRetry);
            return;
        }

        for (s32 i = 0; i < sizeof(::cDemoFortressDiscoverCheckListTable) / sizeof(*::cDemoFortressDiscoverCheckListTable); i++) {
            s32 grandGalaxyId = GameDataConst::getIncludedGrandGalaxyId(::cDemoFortressDiscoverCheckListTable[i].mStageName);

            if (!GameDataFunction::canOnAndIsOffGameEventFlag(::cDemoFortressDiscoverCheckListTable[i].mStageName)) {
                continue;
            }

            if (GameDataFunction::isPassedStoryEvent(::cDemoFortressDiscoverCheckListTable[i].mStoryEventName)) {
                continue;
            }

            if (pMoveArgument->mScenarioNo != grandGalaxyId) {
                continue;
            }

            prepareDemoSequenceButlerFortressDiscover(pMoveArgument, ::cDemoFortressDiscoverCheckListTable[i]);
        }
    }

    if (pMoveArgument->isEqualStage("LibraryRoom") && GameDataFunction::getPictureBookChapterCanRead() != 0) {
        if (GameDataFunction::getPictureBookChapterCanRead() != GameDataFunction::getPictureBookChapterAlreadyRead()) {
            prepareDemoSequence(::cDemoRosettaPicureBookTalk);
        }
    }
}

void StorySequenceExecutor::decideNextEventForAfterLoading(GalaxyMoveArgument* pMoveArgument) {
    if (GameDataFunction::isOnGameEventFlag("RosettaTalkAfterNormalEnding")) {
        return;
    }

    if (GameDataFunction::canOnGameEventFlag("RosettaTalkAfterNormalEnding")) {
        prepareDemoSequence(::cDemoRosettaTalkAfterNormalEnding);
    }
}

void StorySequenceExecutor::decideNextStageForGalaxyOut(GalaxyMoveArgument* pMoveArgument) {
    if (MR::isEqualString("HeavensDoorGalaxy", mStageName)) {
        setNextStageToAstroGalaxy(pMoveArgument);

        pMoveArgument->mIDInfo = JMapIdInfo(100, 0);
    } else {
        setNextStageToAstroGalaxyOrDome(pMoveArgument);
    }
}

void StorySequenceExecutor::overwriteGalaxyNameAfterLoading(GalaxyMoveArgument* pMoveArgument) {
    if (GameDataFunction::hasGrandStar(1)) {
        setNextStageToAstroGalaxy(pMoveArgument);
    } else if (GameDataFunction::isPassedStoryEvent("ピーチ城浮上後") || !GameDataFunction::isDataMario()) {
        pMoveArgument->mStageName = "HeavensDoorGalaxy";
        pMoveArgument->mScenarioNo = 1;
    } else if (GameDataFunction::isPassedStoryEvent("クッパ襲来後")) {
        pMoveArgument->mStageName = "PeachCastleGardenGalaxy";
        pMoveArgument->mScenarioNo = 1;
        pMoveArgument->mIDInfo = JMapIdInfo(1, 0);
    } else {
        prepareDemoSequence(::cDemoPrologue);

        pMoveArgument->mStageName = "PeachCastleGardenGalaxy";
        pMoveArgument->mScenarioNo = 1;
    }
}

void StorySequenceExecutor::setNextStageToAstroGalaxyOrDome(GalaxyMoveArgument* pMoveArgument) {
    s32 scenarioNo = calcAproposScenarioNoOnAstroDome();

    if (scenarioNo >= 1 && scenarioNo <= 6) {
        setNextStageToAstroDome(pMoveArgument);
    } else if (scenarioNo == 0) {
        setNextStageToAstroGalaxy(pMoveArgument);
    }
}

void StorySequenceExecutor::setNextStageToAstroGalaxy(GalaxyMoveArgument* pMoveArgument) {
    pMoveArgument->mStageName = "AstroGalaxy";
    pMoveArgument->mScenarioNo = calcAproposScenarioNoOnAstroGalaxy();
}

void StorySequenceExecutor::setNextStageToAstroDome(GalaxyMoveArgument* pMoveArgument) {
    pMoveArgument->mStageName = "AstroDome";
    pMoveArgument->mScenarioNo = calcAproposScenarioNoOnAstroDome();
}

const StorySequenceExecutorType::DemoSequenceInfo* StorySequenceExecutor::isExecuteDemoLuigiMissing(const GalaxyMoveArgument*) const {
    if (GameSequenceFunction::isLuigiDisappearFromAstroGalaxy() && !GameDataFunction::isOnGameEventFlag("SpecialStarFindingLuigi1")) {
        return ::cDemoLuigiMissing;
    }

    return nullptr;
}

void StorySequenceExecutor::getOptionalDemoForClearGalaxy(
    MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >* pParam1, const GalaxyMoveArgument* pMoveArgument) const {
    const StorySequenceExecutorType::DemoSequenceInfo* pDemoInfo = isExecuteDemoLuigiMissing(pMoveArgument);

    if (pDemoInfo != nullptr) {
        pParam1->push_back(pDemoInfo);
    }

    if (GameDataFunction::canOnJustGameEventFlag("KoopaBattleVs3Galaxy") || GameDataFunction::isOnJustGameEventFlag("PowerStarComplete")) {
        pParam1->push_back(::cDemoRosettaTalkKoopaBattleVs3GalaxyAppear);
    }
}

bool StorySequenceExecutor::isEqualStageStopCometScheduler(const char* pStageName) {
    if (MR::isEqualString(pStageName, "AstroDome")) {
        return true;
    }

    if (MR::isEqualString(pStageName, "AstroGalaxy")) {
        return true;
    }

    if (MR::isEqualString(pStageName, "LibraryRoom")) {
        return true;
    }

    return false;
}

void StorySequenceExecutor::prepareDemoSequence(const StorySequenceExecutorType::DemoSequenceInfo* pDemoInfo) {
    _48.push_back(pDemoInfo);

    mNextNerve = &::StorySequenceExecutorPlayDemoSequence::sInstance;

    setNerve(&::StorySequenceExecutorWaitToSceneStart::sInstance);
}

void StorySequenceExecutor::prepareDemoSequence(
    const MR::Vector< MR::FixedArray< const StorySequenceExecutorType::DemoSequenceInfo*, 8 > >& rParam1) {
    for (const StorySequenceExecutorType::DemoSequenceInfo* const* ppDemoInfo = rParam1.begin(); ppDemoInfo < rParam1.end(); ppDemoInfo++) {
        prepareDemoSequence(*ppDemoInfo);
    }
}

void StorySequenceExecutor::prepareDemoSequenceButlerFortressDiscover(const GalaxyMoveArgument* pMoveArgument,
                                                                      const DemoFortressDiscoverCheckList& rCheckList) {
    const StorySequenceExecutorType::DemoSequenceInfo* pDemoInfo = addDynamicDemoSequenceInfo(0, rCheckList._8 ? 6 : 5, "バトラー報告");

    addDynamicDemoSequenceInfo(7, 0, rCheckList.mStoryEventName);

    if (pMoveArgument->mMoveType == 4) {
        addDynamicDemoSequenceInfo(13, 0, "");
    } else {
        addDynamicDemoSequenceInfo(1, 1, "Fadein");
        addDynamicDemoSequenceInfo(12, 0, "");
    }

    prepareDemoSequence(pDemoInfo);
}

bool StorySequenceExecutor::tryStartDemo(const char* pParam1) {
    return MR::tryStartTimeKeepDemoMarioPuppetable(mDemoObj, pParam1, nullptr);
}

bool StorySequenceExecutor::tryStartSave() {
    if (MR::tryStartDemoWithoutCinemaFrameValidHandPointerFinger(mSaveObj, "セーブ")) {
        GameSequenceFunction::startGameDataSaveSequence(false, false);

        _B0 = true;

        return true;
    }

    return false;
}

bool StorySequenceExecutor::tryWaitSaveEnd() {
    if (!GameSequenceFunction::isActiveSaveDataHandleSequence()) {
        MR::endDemo(mSaveObj, "セーブ");

        return true;
    }

    return false;
}

bool StorySequenceExecutor::tryStartFadein() {
    if (MR::isFirstStep(this)) {
        MR::openWipeFade(-1);
    } else if (!MR::isWipeActive()) {
        return true;
    }

    return false;
}

bool StorySequenceExecutor::tryStartMovieAndWaitEnd(u32 param1) {
    if (MR::isFirstStep(this)) {
        switch (param1) {
        case 9:
            MR::startMovieEpilogueA();
            break;
        case 10:
            MR::startMovieEndingA();
            break;
        case 11:
            MR::startMovieEndingB();
            break;
        default:
            break;
        }
    } else {
        bool isEndMovie = false;

        switch (param1) {
        case 9:
            isEndMovie = MR::isEndMovieEpilogueA();
            break;
        case 10:
            isEndMovie = MR::isEndMovieEndingA();
            break;
        case 11:
            isEndMovie = MR::isEndMovieEndingB();
            break;
        default:
            break;
        }

        return isEndMovie;
    }

    return false;
}

const StorySequenceExecutorType::DemoSequenceInfo* StorySequenceExecutor::getCurrentDemoInfo() const {
    if (_48.size() == 0) {
        return nullptr;
    }

    return *_48.begin();
}

bool StorySequenceExecutor::tryNextDemoInfo() {
    if (_48.size() == 1) {
        return false;
    }

    // FIXME: Supposed inline of MR::Vector::erase.
    _48.erase(_48.begin());

    return true;
}

// StorySequenceExecutor::addDynamicDemoSequenceInfo

void StorySequenceExecutor::setBeforeStageScenario(const GalaxyMoveArgument& rMoveArgument, bool param2) {
    const char* pStageName = nullptr;
    s32 scenarioNo = -1;

    if (MR::isEqualSceneName("Game")) {
        if (rMoveArgument.mMoveType == 4) {
            pStageName = GameSequenceFunction::getClearedStageName();
            scenarioNo = GameSequenceFunction::getClearedPowerStarId();
        } else {
            pStageName = MR::getCurrentStageName();

            if (!param2) {
                scenarioNo = MR::getCurrentScenarioNo();
            }
        }
    }

    snprintf(mStageName, sizeof(mStageName), "%s", pStageName);

    mScenarioNo = scenarioNo;
}

s32 StorySequenceExecutor::calcAproposScenarioNoOnAstroGalaxy() const {
    if (GameDataFunction::canOnGameEventFlag("KoopaBattleVs3Galaxy")) {
        return 5;
    }

    if (GameDataFunction::hasGrandStar(5)) {
        return 3;
    }

    if (GameDataFunction::hasGrandStar(3)) {
        return 2;
    }

    if (GameDataFunction::isOnJustGameEventFlag("SpecialStarGrand1")) {
        return 4;
    }

    return 1;
}

s32 StorySequenceExecutor::calcAproposScenarioNoOnAstroDome() const {
    return GameDataConst::getIncludedGrandGalaxyId(mStageName);
}
