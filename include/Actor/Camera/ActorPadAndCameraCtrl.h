#pragma once

#include "Actor/Model/ModelManager.h"
#include "JGeometry/TVec3.h"

class ActorPadAndCameraCtrl
{
public:
    ActorPadAndCameraCtrl(const ModelManager *, const JGeometry::TVec3<f32> *);

    static ActorPadAndCameraCtrl* tryCreate(const ModelManager *, const JGeometry::TVec3<f32> *);
    void update();

    ModelManager* mModelManager; // _0
    const JGeometry::TVec3<f32>* _4;
    ResourceHolder* mResourceHolder; // _8
    u8 _C[0x18-0xC];
};