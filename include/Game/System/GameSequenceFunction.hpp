#pragma once

#include <revolution/types.h>

class GalaxyMoveArgument;
class UserFile;

namespace GameSequenceFunction {
    void activateGalaxyCometScheduler();
    void deactivateGalaxyCometScheduler();
    void forceToNextStateGalaxyCometScheduler();
    bool isGalaxyCometLand(const char*);
    s32 getEncounterGalaxyCometPowerStarId(const char*);
    const char* getEncounterGalaxyCometName(const char*);
    s32 getGalaxyCometState(const char*);
    bool isNeedMoviePlayerForStorySequenceEvent();
    bool hasNextDemoForStorySequenceEvent();
    s32 getExecutingStorySequenceEventNum();
    bool isLuigiDisappearFromAstroGalaxy();
    bool isLuigiHidingAnyGalaxy();
    bool isActiveLuigiHideAndSeekEvent();
    bool isEndLuigiHideAndSeekEvent();
    void getLuigiHidingGalaxyNameAndStarId(const char**, s32*);
    void onGameEventFlagGetLuigiLetter();
    bool isReadyToStartScene();
    void startScene();
    void notifyToGameSequenceProgressToEndScene();
    void requestChangeScene(const char*);
    void requestGalaxyMove(const GalaxyMoveArgument&);
    void requestCancelScenarioSelect();
    void sendStageResultSequenceParam(const char*, s32, int, int);
    bool hasStageResultSequence() NO_INLINE;
    const char* getClearedStageName() NO_INLINE;
    s32 getClearedPowerStarId();
    int getClearedStarPieceNum();
    int getClearedCoinNum();
    bool hasPowerStarYetAtResultSequence();
    bool isPowerStarAtResultSequence(const char*, s32);
    bool isPowerStarGreenAtResultSequence();
    bool isPowerStarRedAtResultSequence();
    bool isGrandStarAtResultSequence();
    bool hasRetryGalaxySequence();
    bool isNeedToReflectStageResultSequenceStarPiece();
    void reflectStageResultSequenceCoin();
    void resetStageResultSequenceParam();
    void startPreLoadSaveDataSequence();
    void startTotalMailSizeSaveSequence();
    void startGameDataLoadSequence(int, bool);
    void startCreateUserFileSequence(int);
    void startDeleteUserFileSequence(int);
    void startCopyUserFileSequence(int, int);
    void startSetMiiOrIconIdUserFileSequence(int, const void*, const u32*);
    void storeMiiOrIconIdUserFileSequence(int, const void*, const u32*);
    void storeCopyUserFileSequence(int, int);
    void startSaveAllUserFileSequence();
    bool isActiveSaveDataHandleSequence();
    bool isSuccessSaveDataHandleSequence();
    void restoreUserFile(UserFile*, int);
    void restoreUserFile(UserFile*, int, bool);
    void tryNANDErrorSequence(s32);
    void reserveUserName(const wchar_t*);
    void requestPrepareResetNWC24();
    bool isEnableToResetNWC24();
    void resetNWC24();
    void storeSceneStartGameDataHolder();
    void updateGameDataAndSequenceAfterStageResultSequence();
    void startGameDataSaveSequence(bool, bool);
};  // namespace GameSequenceFunction
