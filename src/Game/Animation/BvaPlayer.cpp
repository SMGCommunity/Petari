#include "Game/Animation/BvaPlayer.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"

BvaPlayer::BvaPlayer(const ResTable* pResTable, J3DModel* pModel) : AnmPlayerBase(pResTable), mModel(pModel) {
}

void BvaPlayer::calc() {
    J3DAnmVisibilityFull* pAnmVisibility = getAnmVisibility();
    if (pAnmVisibility == nullptr) {
        return;
    }

    reflectFrame();

    u16 shapeNum = mModel->mModelData->mShapeTable.getShapeNum();
    for (u16 idx = 0; idx < shapeNum; idx++) {
        u8 visibility = 0;
        pAnmVisibility->getVisibility(idx, &visibility);

        if (visibility != 0) {
            mModel->mShapePacket[idx].offFlag(0x10);
        } else {
            mModel->mShapePacket[idx].onFlag(0x10);
        }
    }
}

J3DAnmVisibilityFull* BvaPlayer::getAnmVisibility() {
    if (mAnmRes != nullptr) {
        // TODO: Should be `static_cast` because `J3DAnmVisibilityFull` inherits J3DAnmBase`.
        return reinterpret_cast< J3DAnmVisibilityFull* >(mAnmRes);
    }

    return nullptr;
}
