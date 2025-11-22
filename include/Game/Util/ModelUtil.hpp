#pragma once

#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/JGeometry/TBox.hpp"
#include <revolution.h>

class J3DModel;
class J3DMaterial;
class LiveActor;

namespace MR {
    bool isExistModel(const char*);

    J3DModel* getJ3DModel(const LiveActor*);
    J3DModelData* getJ3DModelData(const LiveActor*);
    J3DModelData* getJ3DModelData(const char*);

    void calcJ3DModel(LiveActor*);

    u32 getMaterialNo(J3DModelData*, const char*);
    u32 getMaterialNo(J3DModel*, const char*);
    J3DMaterial* getMaterial(J3DModelData*, const char*);
    J3DMaterial* getMaterial(J3DModelData*, int);
    J3DMaterial* getMaterial(J3DModel*, int);
    J3DMaterial* getMaterial(const LiveActor*, int);
    u16 getMaterialNum(J3DModel*);
    const char* getMaterialName(const J3DModelData*, int);
    void updateModelDiffDL(LiveActor*);
    bool isEnvelope(J3DMaterial*);
    void hideMaterial(J3DModel*, const char*);

    void showMaterial(const LiveActor*, const char*);
    void hideMaterial(const LiveActor*, const char*);

    void updateMaterial(LiveActor*);

    void syncJointAnimation(LiveActor*, const LiveActor*);

    void syncMaterialAnimation(LiveActor*, const LiveActor*);

    void copyJointAnimation(LiveActor*, const LiveActor*);

    void calcModelBoundingBox(TBox3f*, const LiveActor*);

    void calcModelBoundingRadius(f32*, const LiveActor*);

    bool isExistCollisionResource(const LiveActor*, const char*);
    bool isExistModel(const char*);
    bool isExistSubModel(const char*, const char*);

    bool isExistEffectTexMtx(LiveActor*);

    s16 getBckFrameMax(const LiveActor*, const char*);

    void updateModelManager(LiveActor*);
    void calcAnimModelManager(LiveActor*);
};  // namespace MR
