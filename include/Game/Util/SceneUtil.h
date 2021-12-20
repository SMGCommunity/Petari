#pragma once

#include "Game/Util/JMapInfo.h"
#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

namespace MR {
    s32 getPlacedZoneId(const JMapInfoIter &);

    s32 getCurrentScenarioNo();

    const char* getCurrentStageName();

    s32 getZoneNum();

    const char* getZoneNameFromZoneId(s32);

    void setInitializeStatePlacementPlayer();
    void setInitializeStatePlacementHighPriority();
    void setInitializeStatePlacement();
    void setInitializeStateAfterPlacement();

    s32 getGeneralPosNum();
    void getGeneralPosData(const char **, TVec3f *, TVec3f *, JMapLinkInfo **, int);
}