#include "Game/LiveActor/ShadowVolumeOval.hpp"

ShadowVolumeOval::ShadowVolumeOval() : ShadowVolumeModel("影描画[ボリューム楕球]") {
    mSize.x = 100.0f;
    mSize.y = 100.0f;
    mSize.z = 200.0f;
    initVolumeModel("ShadowVolumeSphere");
}

bool ShadowVolumeOval::isDraw() const {
    ShadowController* controller = getController();
    bool ret = false;

    if (controller->isProjected()) {
        if (controller->isDraw()) {
            ret = true;
        }
    }

    return ret;
}

void ShadowVolumeOval::setSize(register const TVec3f &rVec) {
    __asm {
        psq_l f0, 0(rVec), 0, 0
        lfs f1, 8(rVec)
        psq_st f0, 0x20(r3), 0, 0
        stfs f1, 0x28(r3)
    }
}