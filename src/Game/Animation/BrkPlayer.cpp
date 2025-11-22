#include "Game/Animation/BrkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

BrkPlayer::BrkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData)
    : MaterialAnmPlayerBase(pResourceHolder->mBrkResTable, pModelData) {}

void BrkPlayer::attach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTevRegKey` inherits J3DAnmBase`.
    pModelData->mMaterialTable.entryTevRegAnimator(reinterpret_cast< J3DAnmTevRegKey* >(pAnmRes));
}

void BrkPlayer::detach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTevRegKey` inherits J3DAnmBase`.
    pModelData->mMaterialTable.removeTevRegAnimator(reinterpret_cast< J3DAnmTevRegKey* >(pAnmRes));
}
