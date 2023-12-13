#include "Game/LiveActor/ShadowVolumeOvalPole.hpp"

ShadowVolumeOvalPole::~ShadowVolumeOvalPole() {

}

ShadowVolumeOvalPole::ShadowVolumeOvalPole() : ShadowVolumeModel("影描画[ボリューム楕円柱]") {
    mSize.x = 100.0f;
    mSize.y = 100.0f;
    mSize.z = 200.0f;
    initVolumeModel("ShadowVolumeCylinder");
}

void ShadowVolumeOvalPole::setSize(register const TVec3f &rVec) {
    __asm {
        psq_l f0, 0(rVec), 0, 0
        lfs f1, 8(rVec)
        psq_st f0, 0x20(r3), 0, 0
        stfs f1, 0x28(r3)
    }
}