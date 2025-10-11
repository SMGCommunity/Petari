#include "Game/Animation/BvaPlayer.hpp"

BvaPlayer::BvaPlayer(const ResTable* pResTable, J3DModel* pModel) :
    AnmPlayerBase(pResTable),
    mModel(pModel)
{
    
}

// BvaPlayer::calc

J3DAnmVisibilityFull* BvaPlayer::getAnmVisibility() {
    if (mAnmRes != nullptr) {
        // TODO: Should be `static_cast` because `J3DAnmVisibilityFull` inherits J3DAnmBase`.
        return reinterpret_cast<J3DAnmVisibilityFull*>(mAnmRes);
    }

    return nullptr;
}
