#include "Game/Player/MarioParts.hpp"
#include "Game/Util.hpp"

MarioParts::MarioParts(LiveActor* pHost, const char* pName, const char* pModelName, bool a4, MtxPtr mtx1, MtxPtr mtx2) 
    : PartsModel(pHost, pName, pModelName, mtx1, a4 ? 0x16 : 0x15, true) {
    _9C = 0;
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    if (MR::getLightNumMax(this) > 0) {
        MR::initLightCtrl(this);
    }
    if (mtx2) {
        TVec3f offs(0.0f, 0.0f, 0.0f);
        TVec3f rot(0.0f, 0.0f, 0.0f);
        initFixedPosition(mtx2, offs, rot);
        offFixedPosNormalizeScale();
    }
}

MarioParts::MarioParts(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr mtx) 
    : PartsModel(pHost, pName, pModelName, mtx, 0x17, true) {
    _9C = 0;
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    if (MR::getLightNumMax(this) > 0) {
        MR::initLightCtrl(this);
    }
}

void MarioParts::init(const JMapInfoIter& rIter) {
    initEffectKeeper(8, _9C, false);
    initSound(8, false);
    MR::newDifferedDLBuffer(this);
    makeActorAppeared();
    makeActorAppeared();
}

MarioParts::~MarioParts() {
    
}
