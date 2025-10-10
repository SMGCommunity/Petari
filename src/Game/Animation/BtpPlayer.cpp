#include "Game/Animation/BtpPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

BtpPlayer::BtpPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) :
    MaterialAnmPlayerBase(pResourceHolder->mBtpResTable, pModelData)
{
    
}

void BtpPlayer::attach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTexPattern` inherits J3DAnmBase`.
    pModelData->mMaterialTable.entryTexNoAnimator(reinterpret_cast<J3DAnmTexPattern*>(pAnmRes));
}

void BtpPlayer::detach(J3DAnmBase* pAnmRes, J3DModelData* pModelData) {
    // TODO: Should be `static_cast` because `J3DAnmTexPattern` inherits J3DAnmBase`.
    pModelData->mMaterialTable.removeTexNoAnimator(reinterpret_cast<J3DAnmTexPattern*>(pAnmRes));
}
