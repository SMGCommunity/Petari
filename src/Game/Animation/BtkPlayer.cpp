#include "Game/Animation/BtkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

BtkPlayer::BtkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData)
    : MaterialAnmPlayerBase(pResourceHolder->mBtkResTable, pModelData) {
}

void BtkPlayer::attach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTextureSRTKey` inherits J3DAnmBase`.
    pModelData->mMaterialTable.entryTexMtxAnimator(reinterpret_cast<J3DAnmTextureSRTKey*>(pAnmRes));
}

void BtkPlayer::detach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTextureSRTKey` inherits J3DAnmBase`.
    pModelData->mMaterialTable.removeTexMtxAnimator(reinterpret_cast<J3DAnmTextureSRTKey*>(pAnmRes));
}
