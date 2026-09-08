#include "Game/Player/MarioParts.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

MarioParts::MarioParts(LiveActor* pActor, const char* pName, const char* pModelName, bool a4, MtxPtr mtx1, MtxPtr mtx2)
    : PartsModel(pActor, pName, pModelName, mtx2, a4 ? MR::DrawBufferType_CrystalBox : MR::DrawBufferType_PlayerDecoration, true), _9C() {
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    if (MR::getLightNumMax(this) > 0) {
        MR::initLightCtrl(this);
    }

    if (mtx1 != nullptr) {
        initFixedPosition(mtx1, TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
        offFixedPosNormalizeScale();
    }
}

MarioParts::MarioParts(LiveActor* pActor, const char* pName, const char* pModelName, MtxPtr mtx)
    : PartsModel(pActor, pName, pModelName, nullptr, MR::DrawBufferType_UNK_0x17, true), _9C() {
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    if (MR::getLightNumMax(this) > 0) {
        MR::initLightCtrl(this);
    }
}

void MarioParts::init(const JMapInfoIter& rIter) {
    initEffectKeeper(8, _9C, false);
    initSound(8, false);
    MR::invalidateClipping(this);
    makeActorAppeared();
    makeActorAppeared();
}
