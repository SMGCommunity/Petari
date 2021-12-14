#include "Game/LiveActor/ClippingActorHolder.h"
#include "Game/Util.h"

ClippingActorHolder::ClippingActorHolder() : NameObj("クリッピングアクター保持") {
    _C = 0;
    _10 = 0;
    _14 = 0;
    _18 = 0;
    _1C = 0;
    mViewGroupCtrl = 0;

    _10 = new ClippingActorInfoList(sActorNumMax);
    _14 = new ClippingActorInfoList(sActorNumMax);
    _18 = new ClippingActorInfoList(sActorNumMax);
    _1C = new ClippingActorInfoList(sActorNumMax);
    mViewGroupCtrl = new ViewGroupCtrl();
}

void ClippingActorHolder::movement() {
    mViewGroupCtrl->update();

    for (s32 i = 0; i < _10->_4; i++) {
        _10->mClippingActorList[i]->judgeClipping();
    }
}

void ClippingActorHolder::registerActor(LiveActor *pActor) {
    ClippingActorInfo* inf = new ClippingActorInfo(pActor);
    _18->add(inf);
    _C++;
}

void ClippingActorHolder::initSystemInfo(LiveActor *pActor, const JMapInfoIter &rIter) {
    s32 viewGroupID = -1;
    bool res = MR::getJMapInfoViewGroupID(rIter, &viewGroupID);

    if (res) {
        mViewGroupCtrl->initActorInfo(find(pActor), viewGroupID);
    }
}