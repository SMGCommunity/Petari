#pragma once

#include "Game/Util/JMapInfo.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

namespace MR {
    s32 getPlacedZoneId(const JMapInfoIter &);
    s32 getCurrentPlacementZoneId();

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

    const char* getAppearPowerStarObjName(s32);

    JMapIdInfo& getInitializeStartIdInfo();
}