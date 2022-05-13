#include "Game/LiveActor/ShadowVolumeCylinder.h"

ShadowVolumeCylinder::ShadowVolumeCylinder() : ShadowVolumeModel("影描画[ボリューム円柱]") {
    mRadius = 100.0f;
    initVolumeModel("ShadowVolumeCylinder");
}

void ShadowVolumeCylinder::setRadius(f32 radius) {
    mRadius = radius;
}