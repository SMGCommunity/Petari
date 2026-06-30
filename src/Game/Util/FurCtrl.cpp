#include "Game/Util/FurCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

void FurDrawManager::add(FurCtrl* pCtrl, u8 idx) {
    u8 i = mNumFurCtrls + idx;
    if (_C != i) {
        _10[idx] = pCtrl;
        mNumFurCtrls++;
    }
}

void FurDrawManager::draw() const {
    for (u32 i = 0; i < _E; i++) {
        if (!MR::isClipped(_14[i]->_0) && !MR::isDead(_14[i]->_0) && !MR::isHiddenModel(_14[i]->_0)) {
            if (_14[i]->_1C == 1) {
                _14[i]->drawFur();
            }
        }
    }

    GXSetClipMode(GX_CLIP_ENABLE);
}
