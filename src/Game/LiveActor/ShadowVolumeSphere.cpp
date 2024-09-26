#include "Game/LiveActor/ShadowVolumeSphere.hpp"

ShadowVolumeSphere::~ShadowVolumeSphere() {

}

ShadowVolumeSphere::ShadowVolumeSphere() : ShadowVolumeModel("影描画[ボリューム球]") {
    mRadius = 100.0f;
    initVolumeModel("ShadowVolumeSphere");
}

void ShadowVolumeSphere::setRadius(f32 radius) {
    mRadius = radius;
}

bool ShadowVolumeSphere::isDraw() const {
    ShadowController* controller = getController();
    bool ret = false;

    if (controller->isProjected() && controller->isDraw()) {
        ret = true;
    }

    return ret;
}