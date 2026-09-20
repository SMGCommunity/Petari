#include "Game/Screen/LayoutGroupCtrl.hpp"
#include "Game/Animation/LayoutAnmPlayer.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include <nw4r/lyt/group.h>

LayoutGroupCtrl::LayoutGroupCtrl(LayoutManager* pManager, const char* pName, u32 animLayerCount)
    : mManager(pManager), mGroup(), mAnmPlayerArray(animLayerCount), _10(true) {
    mGroup = pManager->getGroup(pName);

    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i] = new LayoutAnmPlayer(pManager);
    }
}

void LayoutGroupCtrl::movement() {
    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i]->movement();
    }
}

void LayoutGroupCtrl::calcAnim() {
    for (u32 i = 0; i < mAnmPlayerArray.size(); i++) {
        mAnmPlayerArray[i]->reflectFrame();
    }
}

u32 LayoutGroupCtrl::getPaneNum() const {
    return mGroup->mPaneLinkList.GetSize();
}
