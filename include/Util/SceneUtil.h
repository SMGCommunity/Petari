#pragma once

#include "JGeometry/TMatrix34.h"
#include "JMap/JMapInfo.h"
#include "JMap/JMapInfoIter.h"

class JMapIdInfo;

namespace MR
{
    JMapIdInfo* getInitializeStartIdInfo();
    void getRailInfo(JMapInfoIter *, const JMapInfo **, const JMapInfoIter &);
    void getCameraRailInfo(JMapInfoIter *, const JMapInfo **, s32, s32);

    bool isEqualStageName(const char *);

    bool isPlacementLocalStage();
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* getZonePlacementMtx(s32);
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* getZonePlacementMtx(const JMapInfoIter &);

    s32 getPlacedZoneId(const JMapInfoIter &);
};