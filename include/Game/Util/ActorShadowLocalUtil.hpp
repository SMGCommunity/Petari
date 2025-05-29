#pragma once

#include "Game/Util.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowSurfaceBox.hpp"
#include "Game/LiveActor/ShadowSurfaceCircle.hpp"
#include "Game/LiveActor/ShadowSurfaceOval.hpp"
#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/ShadowVolumeCylinder.hpp"
#include "Game/LiveActor/ShadowVolumeFlatModel.hpp"
#include "Game/LiveActor/ShadowVolumeLine.hpp"
#include "Game/LiveActor/ShadowVolumeOval.hpp"
#include "Game/LiveActor/ShadowVolumeOvalPole.hpp"
#include "Game/LiveActor/ShadowVolumeSphere.hpp"

namespace ActorShadow {
    u32 getShadowControllerCount(const LiveActor *);
    ShadowController* getShadowController(const LiveActor *, u32);
    ShadowController* getShadowController(const LiveActor *, const char *);
    bool isExistShadowController(const LiveActor *, const char *);
    ShadowController* createShadowControllerVolumeParam(LiveActor *, const char *);
    ShadowController* createShadowControllerSuefaceParam(LiveActor *, const char *);  // Not a typo
    ShadowDrawer* getShadowSurfaceOval(const LiveActor *, const char *);
    ShadowDrawer* getShadowVolumeSphere(const LiveActor *, const char *);
    ShadowDrawer* getShadowVolumeCylinder(const LiveActor *, const char *);
    ShadowDrawer* getShadowVolumeBox(const LiveActor *, const char *);
    ShadowDrawer* getShadowVolumeDrawer(const LiveActor *, const char *);
    bool getJointNameFromCSV(const char **, const JMapInfoIter &);
    s32 getShadowTypeFromCSV(const JMapInfoIter &);
    void setUpShadowControlBaseMtxFromCSV(ShadowController *, LiveActor *, const JMapInfoIter &);
    void setUpShadowControlIsFollowScaleFromCSV(ShadowController *, const JMapInfoIter &);
    void setUpShadowControlIsSyncShowFromCSV(ShadowController *, const JMapInfoIter &);
    void setUpShadowControlCalcCollisionFromCSV(ShadowController *, const JMapInfoIter &);
    void setUpShadowControlCalcGravityFromCSV(ShadowController *, const JMapInfoIter &);
    void setUpShadowControlFromCSV(ShadowController *, LiveActor *, const JMapInfoIter &);
    ShadowController* createShadowControlFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowSurfaceCircle* createShadowSurfaceCircleFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowSurfaceOval* createShadowSurfaceOvalFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowSurfaceBox* createShadowSurfaceBoxFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeSphere* createShadowVolumeSphereFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeOval* createShadowVolumeOvalFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeOvalPole* createShadowVolumeOvalPoleFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeCylinder* createShadowVolumeCylinderFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeBox* createShadowVolumeBoxFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeFlatModel* createShadowVolumeFlatModelFromCSV(LiveActor *, const JMapInfoIter &);
    ShadowVolumeLine* createShadowVolumeLineFromCSV(LiveActor *, const JMapInfoIter &);
    void addShadowFromCSV(LiveActor *, const JMapInfoIter &);
};
