#include "Game/LiveActor/ShadowVolumeOval.hpp"

ShadowVolumeOval::ShadowVolumeOval() : ShadowVolumeModel("影描画[ボリューム楕球]"), mSize(100.0f, 100.0f, 200.0f) {
    initVolumeModel("ShadowVolumeSphere");
}

bool ShadowVolumeOval::isDraw() const {
    ShadowController* controller = getController();

    return controller->isProjected() && controller->isDraw();
}

void ShadowVolumeOval::setSize(const TVec3f& rSize) {
    mSize = rSize;
}
