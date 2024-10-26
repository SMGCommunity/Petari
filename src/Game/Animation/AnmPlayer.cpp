#include "Game/Animation/AnmPlayer.hpp"
#include "Game/Util.hpp"
#include "Game/System/ResourceInfo.hpp"

AnmPlayerBase::AnmPlayerBase(const ResTable *pTable) : 
    mResourceTable(pTable), mData(0), mFrameCtrl(0) {
}

void AnmPlayerBase::update() {
    if (!mData) {
        return;
    }

    mFrameCtrl.update();
}

/*
void AnmPlayerBase::reflectFrame() {
    if (!_4) {
        void* 
        _4 = 
    }
}*/

void AnmPlayerBase::stop() {
    stopAnimation();
    mFrameCtrl.mSpeed = 0.0f;
}

bool AnmPlayerBase::isPlaying(const char *pAnimName) const {
    if (mData) {
        if (MR::isEqualStringCase(pAnimName, mResourceTable->getResName(mData))) {
            return true;
        }
    }

    return false;
}

MaterialAnmPlayerBase::MaterialAnmPlayerBase(const ResTable *pTable, J3DModelData *pModelData) : AnmPlayerBase(pTable) {
    mModelData = pModelData;
}

void MaterialAnmPlayerBase::beginDiff() {
    if (!mData) {
        return;
    }

    reflectFrame();
    attach(reinterpret_cast<J3DAnmBase*>(mData), mModelData);
}

void MaterialAnmPlayerBase::endDiff() {
    if (!mData) {
        return;
    }

    detach(reinterpret_cast<J3DAnmBase*>(mData), mModelData);
}

void AnmPlayerBase::changeAnimation(J3DAnmBase *) {

}

void AnmPlayerBase::stopAnimation() {

}