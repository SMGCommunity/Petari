#pragma once

#include "JSystem/JGeometry/TMatrix34.h"
#include "JSystem/JMap/JMapInfo.h"
#include "JSystem/JMap/JMapInfoIter.h"
#include "Scene/Scene.h"

class JMapIdInfo;

namespace MR
{
    JMapIdInfo* getInitializeStartIdInfo();
    void getRailInfo(JMapInfoIter *, const JMapInfo **, const JMapInfoIter &);
    void getCameraRailInfo(JMapInfoIter *, const JMapInfo **, s32, s32);

    const char* getCurrentStageName();

    bool isEqualStageName(const char *);

    bool isPlacementLocalStage();
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* getZonePlacementMtx(s32);
    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> >* getZonePlacementMtx(const JMapInfoIter &);

    s32 getPlacedZoneId(const JMapInfoIter &);

    SceneObjHolder* getSceneObjHolder();
};