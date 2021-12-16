#include "Game/LiveActor/ClippingGroupHolder.h"
#include "Game/LiveActor/ClippingActorInfo.h"
#include "Game/LiveActor/LiveActor.h"

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

#ifdef NON_MATCHING
// weird regalloc issue
void ClippingGroupHolder::movement() {
    for (s32 i = 0; i < mNumGroups; i++) {
        ClippingInfoGroup* group = mInfoGroups[i];

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
#endif