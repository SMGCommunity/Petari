#pragma once

#include <revolution.h>

namespace MR {
    bool isPowerStarGetDemoWithLuigiCurrentGalaxyAndScenario(s32);

    bool isOnGameEventFlagViewCompleteEndingMarioAndLuigi();

    bool isOnGameEventFlagChildsRoom();
    bool isOnGameEventFlagLibraryRoom();
    bool isOnGameEventFlagRosettaTalkAboutTicoInTower();
    bool isRosettaTalkTorchProgress();
    bool isRosettaTalkCountDownStart();
    bool isRosettaTalkAstroDomeRecover();
    bool isRosettaTalkKoopa();
    bool isRosettaTalkTorchLecture();
    bool isKinopioExplorerCompleteTrickComet();
    bool isKinopioExplorerTalkGoFinalBattle();
    bool isKinopioExplorerTalkGetGrandStar6();
    bool isKinopioExplorerTalkGetGrandStar5();
    bool isKinopioExplorerTalkGetGrandStar4();
    bool isKinopioExplorerTalkGetGrandStar3();
    bool isKinopioExplorerTrickComet();
    bool isKinopioExplorerTalkGetGrandStar2();
    bool isUFOKinokoBeforeConstruction();

    bool isOnGameEventFlagUseAstroDome();
    bool isOnGameEventFlagGreenDriver();

    bool hasGrandStar(int);

    bool isGalaxyAppearGreenDriver(const char *);
    void onGameEventFlagGalaxyOpen(const char *);
    bool hasPowerStarInCurrentStage(s32);

    bool hasPowerStarAtResultSequence();

    bool hasPowerStarInCurrentStageWithDeclarer(const char *, s32);
    bool isPowerStarGreenInCurrentStageWithDeclarer(const char *, s32);
    bool isPowerStarRedInCurrentStageWithDeclarer(const char* , s32);

    bool isPowerStarGreenInCurrentStage(s32);
    bool isPowerStarRedInCurrentStage(s32);
    bool isGrandStarInCurrentStage(s32);

    bool isPowerStarGreenAtResultSequence();
    bool isPowerStarRedAtResultSequence();

    bool isGrandStarInCurrentStageWithDeclarer(const char *, s32);

    bool isPowerStarGetDemoWithLuigiCurrentGalaxy();

    bool isGalaxyRedCometAppearInCurrentStage();
    bool isGalaxyDarkCometAppearInCurrentStage();
    bool isGalaxyRedCometAppearInCurrentStage();
    bool isGalaxyGhostCometAppearInCurrentStage();
    bool isGalaxyQuickCometAppearInCurrentStage();
    bool isGalaxyBlackCometAppearInCurrentStage();

    bool isPlayerLuigi();

    void explainOneUpIfAtFirst();

    s32 getPlayerLeft();
    void incPlayerLeft();

    bool isUFOKinokoBeforeConstruction();
    bool isUFOKinokoUnderConstruction();

    bool isStarPieceCounterStop();

    void updateAlreadyDoneFlag(int, u32);
    bool isOnMessageAlreadyRead(s8);
    void onMessageAlreadyRead(s8);
    u32 setupAlreadyDoneFlag(const char *, const JMapInfoIter &, u32 *);

    bool isGalaxyCometLandInStage(const char *);

    int getEncounterGalaxyCometNameId(const char *);

    int getStarPieceNum();

    const char* getCometNameFromId(int);

    s32 getEncounterGalaxyCometPowerStarId(const char *);

    s32 getPowerStarLeftToDisplayCountDownPlate();

    bool isOnGameEventFlagViewNormalEnding();
    bool isButlerMapAppear();
};
