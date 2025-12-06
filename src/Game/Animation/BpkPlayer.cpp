#include "Game/Animation/BpkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

BpkPlayer::BpkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData)
    : MaterialAnmPlayerBase(pResourceHolder->mBpkResTable, pModelData) {}

void BpkPlayer::attach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmColor` inherits J3DAnmBase`.
    pModelData->mMaterialTable.entryMatColorAnimator(reinterpret_cast< J3DAnmColor* >(pAnmRes));
}

void BpkPlayer::detach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmColor` inherits J3DAnmBase`.
    pModelData->mMaterialTable.removeMatColorAnimator(reinterpret_cast< J3DAnmColor* >(pAnmRes));
}
