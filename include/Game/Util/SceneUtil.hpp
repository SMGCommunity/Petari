#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class JKRMemArchive;
class JMapIdInfo;
class JMapInfo;
class JMapInfoIter;
class JMapLinkInfo;
class NameObj;

namespace MR {
    s32 getCurrentScenarioNo();
    s32 getCurrentSelectedScenarioNo();
    void setCurrentScenarioNo(s32, s32);
    bool isScenarioDecided();
    const char* getCurrentStageName();
    bool isEqualSceneName(const char*);
    bool isEqualStageName(const char*);
    bool isStageBeginPrologueEvent();
    bool isStageBeginFadeWipe();
    bool isStageBeginTitleWipe();
    bool isStageBeginWithoutWipe();
    bool isStageDisablePauseMenu();
    bool isStageAstroLocation();
    bool isStageSwimAngleLimit();
    bool isStageStarPieceFollowGroupLimit();
    bool isStageFileSelect();
    bool isStageKoopaVs();
    bool isStageKoopaVs1();
    bool isStageKoopaVs2();
    bool isStageKoopaVs3();
    bool isStageEpilogueDemo();
    bool isBeginScenarioStarter();
    bool isStageSuddenDeathDodoryu();
    void setInitializeStatePlacementPlayer();
    void setInitializeStatePlacementHighPriority();
    void setInitializeStatePlacement();
    void setInitializeStateAfterPlacement();
    bool isInitializeStateEnd();
    bool isInitializeStatePlacementSomething();
    void stopSceneForScenarioOpeningCamera();
    void playSceneForScenarioOpeningCamera();
    const JMapIdInfo& getCurrentMarioStartIdInfo();
    s32 getStartPosNum();
    s32 getCurrentStartZoneId();
    const JMapIdInfo& getInitializeStartIdInfo();
    JKRMemArchive* getStageArchive(const char*);
    s32 getGeneralPosNum();
    void getGeneralPosData(const char**, TVec3f*, TVec3f*, JMapLinkInfo**, int);
    s32 getChildObjNum(const JMapInfoIter&);
    const char* getChildObjName(const char**, const JMapInfoIter&, int);
    void initChildObj(NameObj*, const JMapInfoIter&, int);
    const char* getAppearPowerStarObjName(s32);
    s32 getCurrentStageNormalScenarioNum();
    s32 getCurrentStagePowerStarNum();
    s32 getZoneNum();
    const char* getZoneNameFromZoneId(s32);
    s32 getPlacedHiddenStarScenarioNo(const char*, s32);
    void getRailInfo(JMapInfoIter*, const JMapInfo**, const JMapInfoIter&);
    bool getNextLinkRailInfo(JMapInfoIter*, const JMapInfo**, const JMapInfoIter&);
    s32 getCurrentStartCameraId();
    void getStartCameraIdInfoFromStartDataIndex(JMapIdInfo*, int);
    s32 getPlacedRailNum(s32);
    void getCameraRailInfo(JMapInfoIter*, const JMapInfo**, s32, s32);
    bool getCameraRailInfoFromRailDataIndex(JMapInfoIter*, const JMapInfo**, int, s32);
    void getStageCameraData(void**, s32*, s32);
    void getCurrentScenarioStartAnimCameraData(void**, s32*);
    void incCoin(int);
    void incPurpleCoin();
    s32 getCoinNum();
    s32 getPurpleCoinNum();
    s32 getPowerStarNum();
    bool isPlacementLocalStage();
    s32 getPlacedZoneId(const JMapInfoIter&);
    TMtx34f* getZonePlacementMtx(const JMapInfoIter&);
    TMtx34f* getZonePlacementMtx(s32);
    const char* getJapaneseObjectName(const char*);
    void setCurrentPlacementZoneId(s32);
    void clearCurrentPlacementZoneId();
    s32 getCurrentPlacementZoneId();
    const char* getCurrentPlacementZoneName();
};  // namespace MR
