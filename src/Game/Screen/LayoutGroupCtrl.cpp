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

nw4r::lyt::Pane* LayoutGroupCtrl::getPane(u32 idx) const {
    for (nw4r::lyt::PaneLinkList::Iterator iter = mGroup->GetPaneList().GetBeginIter(); iter != mGroup->GetPaneList().GetEndIter(); iter++) {
        if (idx == 0) {
            return iter->mTarget;
        }

        idx--;
    }

    return nullptr;
}

void LayoutGroupCtrl_FORCE_MATCH(nw4r::lyt::Group* pGroup, LayoutAnmPlayer* pAnmPlayer) {
    for (nw4r::lyt::PaneLinkList::Iterator iter = pGroup->GetPaneList().GetBeginIter(); iter != pGroup->GetPaneList().GetEndIter(); iter++) {
        (void)iter->mTarget;
    }

    pAnmPlayer->isStop();
}
