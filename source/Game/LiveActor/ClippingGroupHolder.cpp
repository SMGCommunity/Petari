#include "Game/LiveActor/ClippingGroupHolder.hpp"
#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"

ClippingInfoGroup::ClippingInfoGroup(const char *pGroupName, int count) : NameObj(pGroupName) {
    _C = count;
    _10 = 0;
    _14 = 0;
    _18 = 0;
    _1C = 0;
    _14 = new ClippingActorInfo*[count];

    for (s32 i = 0; i < _C; i++) {
        _14[i] = 0;
    }
}

void ClippingInfoGroup::setGroupNo(const JMapInfoIter &rIter) {
    _18 = new JMapIdInfo(MR::createJMapIdInfoFromClippingGroupId(rIter));
}

void ClippingInfoGroup::registerInfo(ClippingActorInfo *pInfo) {
    _14[_10] = pInfo;
    _10++;
}

bool ClippingInfoGroup::isClippedNowAll() const {
    for (s32 i = 0; i < _10; i++) {
        if (!MR::isDead(_14[i]->mActor)) {
            if (MR::isInvalidClipping(_14[i]->mActor)) {
                return false;
            }

            if (!_14[i]->isJudgedToClip()) {
                return false;
            }
        }
    }

    return true;
}

void ClippingInfoGroup::startClippedAll() {
    _1C = 1;

    for (s32 i = 0; i < _10; i++) {
        if (!MR::isDead(_14[i]->mActor) && !MR::isClipped(_14[i]->mActor)) {
            _14[i]->mActor->startClipped();
        }
    }
}

void ClippingInfoGroup::endClippedAll() {
    _1C = 0;

    for (s32 i = 0; i < _10; i++) {
        if (!MR::isDead(_14[i]->mActor) && MR::isClipped(_14[i]->mActor)) {
            _14[i]->mActor->endClipped();
        }
    }
}

void ClippingGroupHolder::movement() {
    ClippingInfoGroup* group;
    for (s32 i = 0; i < mNumGroups; i++) {
        group = mInfoGroups[i];

        if (group->isClippedNowAll()) {
            u8 var = group->_1C;
            if (!var) {
                group->startClippedAll();
            }
        }
        else {
            u8 var = group->_1C;
            if (!var) {
                group->endClippedAll();
            }
        }
    }
}

void ClippingGroupHolder::createAndAdd(ClippingActorInfo *pInfo, const JMapInfoIter &rIter, int count) {
    ClippingInfoGroup* group = findGroup(rIter);

    if (!group) {
        group = createGroup(pInfo, rIter, count);
    }

    group->registerInfo(pInfo);
}

ClippingInfoGroup* ClippingGroupHolder::createGroup(ClippingActorInfo *pInfo, const JMapInfoIter &rIter, int count) {
    ClippingInfoGroup* group = new ClippingInfoGroup(pInfo->mActor->mName, count);
    group->setGroupNo(rIter);
    mInfoGroups[mNumGroups] = group;
    mNumGroups++;
    return group;
}

#ifdef NON_MATCHING
// reg usage issue, and not reloading the array to return
ClippingInfoGroup* ClippingGroupHolder::findGroup(const JMapInfoIter &rIter) {
    JMapIdInfo info = MR::createJMapIdInfoFromClippingGroupId(rIter);

    for (s32 i = 0; i < mNumGroups; i++) {
        bool isFound = false;
        JMapIdInfo* inf = mInfoGroups[i]->_18;

        if (*inf == info) {
            return mInfoGroups[i];
        }
    }

    return 0;
}
#endif

ClippingInfoGroup::~ClippingInfoGroup() {

}

ClippingGroupHolder::~ClippingGroupHolder() {

}

ClippingGroupHolder::ClippingGroupHolder() : NameObj("クリッピングアクター保持") {
    mNumGroups = 0;
    mInfoGroups = 0;
    mInfoGroups = new ClippingInfoGroup*[0x40];

    for (s32 i = 0; i < 0x40; i++) {
        mInfoGroups[i] = 0;
    }
}