#pragma once

#include "Game/Util/JMapInfo.h"
#include "JSystem/JGeometry/TMatrix.h"
#include <revolution.h>

namespace MR {
    s32 getPlacedZoneId(const JMapInfoIter &);

    s32 getCurrentScenarioNo();

    const char* getCurrentStageName();

    s32 getZoneNum();

    const char* getZoneNameFromZoneId(s32);

    TMtx34f *getZonePlacementMtx(long);

    void getStageCameraData(void **, long *, long);

    void setInitializeStatePlacementPlayer();
    void setInitializeStatePlacementHighPriority();
    void setInitializeStatePlacement();
    void setInitializeStateAfterPlacement();
}