#pragma once

#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class JMapLinkInfo;
class JMapIdInfo;
class NameObj;

namespace MR {
    s32 getPlacedZoneId(const JMapInfoIter &);
    void setCurrentPlacementZoneId(s32);
    s32 getCurrentPlacementZoneId();
    void clearCurrentPlacementZoneId();

    s32 getChildObjNum(const JMapInfoIter &);
    void getChildObjName(const char **, const JMapInfoIter &, int);
    void initChildObj(NameObj *, const JMapInfoIter &, int);

    const char* getCurrentPlacementZoneName();

    s32 getCurrentScenarioNo();

    const char* getCurrentStageName();

    s32 getCurrentStagePowerStarNum();
    s32 getZoneNum();

    s32 getPlacedRailNum();

    const char* getZoneNameFromZoneId(s32);

    TMtx34f *getZonePlacementMtx(long);
    TMtx34f *getZonePlacementMtx(const JMapInfoIter &);

    void getStageCameraData(void **, long *, long);

    void getRailInfo(JMapInfoIter *, const JMapInfo **, const JMapInfoIter &);

    void getCameraRailInfo(JMapInfoIter *, const JMapInfo **, s32, s32);

    void setInitializeStatePlacementPlayer();
    void setInitializeStatePlacementHighPriority();
    void setInitializeStatePlacement();
    void setInitializeStateAfterPlacement();

    s32 getGeneralPosNum();
    void getGeneralPosData(const char **, TVec3f *, TVec3f *, JMapLinkInfo **, int);

    void getCurrentScenarioStartAnimCameraData(void **, long *);

    void playSceneForScenarioOpeningCamera();

    s32 getCurrentStartCameraID();
    s32 getCurrentStartZoneID();

    bool isEqualStageName(const char *);

    bool isStageBeginPrologueEvent();

    bool isStageKoopaVs();
    bool isStageKoopaVs3();

    bool isStageAstroLocation();

    bool isBeginScenarioStarter();

    void incCoin(int);
    void incPurpleCoin();

    s32 getCoinNum();

    bool isPlacementLocalStage();

    bool isInitializeStateEnd();

    bool isStageDisablePauseMenu();

    bool isStageSwimAngleLimit();

    const char* getAppearPowerStarObjName(s32);

    JMapIdInfo& getInitializeStartIdInfo();

    bool isScenarioDecided();

    s32 getCurrentStageNormalScenarioNum();

    const char* getJapaneseObjectName(const char *);
};