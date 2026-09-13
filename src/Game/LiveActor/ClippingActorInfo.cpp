#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/ClippingJudge.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ViewGroupCtrl.hpp"
#include "Game/Util.hpp"

ClippingActorInfo::ClippingActorInfo(LiveActor* pActor) {
    mActor = pActor;
    _4 = 0;
    _8 = 0.0f;
    mInfo = 0;
    mFarClipLevel = 6;
    _12 = -1;
    _14 = 0;
    setTypeToSphere(300.0f, 0);
}

void ClippingActorInfo::judgeClipping() {
    if (isJudgedToClip()) {
        if (!MR::isClipped(mActor)) {
            mActor->startClipped();
        }
    } else {
        if (MR::isClipped(mActor)) {
            mActor->endClipped();
        }
    }
}

bool ClippingActorInfo::isJudgedToClip() const {
    s32 farClipLevel = mFarClipLevel;
    if (_14->_0 == 1) {
        farClipLevel = 0;
    }

    f32 val = _8;
    ClippingJudge* judge = MR::getClippingJudge();
    return judge->isJudgedToClipFrustum(*_4, val, farClipLevel);
}

bool ClippingActorInfo::isGroupClipping() const {
    return mInfo;
}

void ClippingActorInfo::setTypeToSphere(f32 radius, const TVec3f* pCenter) {
    _8 = radius;
    if (!pCenter) {
        _4 = &mActor->mPosition;
    } else {
        _4 = pCenter;
    }
}

void ClippingActorInfo::setGroupClippingNo(const JMapInfoIter& rIter) {
    mInfo = new JMapIdInfo(MR::createJMapIdInfoFromClippingGroupId(rIter));
}

ClippingActorInfoList::ClippingActorInfoList(int a1) {
    _0 = a1;
    _4 = 0;
    mClippingActorList = 0;

    mClippingActorList = new ClippingActorInfo*[a1];

    for (s32 i = 0; i < _0; i++) {
        mClippingActorList[i] = 0;
    }
}

void ClippingActorInfoList::add(ClippingActorInfo* pInfo) {
    mClippingActorList[_4] = pInfo;
    _4++;
}

ClippingActorInfo* ClippingActorInfoList::remove(LiveActor* pActor) {
    s32 index = 0;
    ClippingActorInfo* info = find(pActor, &index);
    s32 lastIndex = _4 - 1;
    mClippingActorList[index] = mClippingActorList[lastIndex];
    _4--;
    return info;
}

ClippingActorInfo* ClippingActorInfoList::find(const LiveActor* pActor, s32* pIndex) const {
    for (s32 i = 0; i < _4; i++) {
        ClippingActorInfo* inf = mClippingActorList[i];

        if (inf->mActor == pActor) {
            if (pIndex) {
                *pIndex = i;
            }

            return mClippingActorList[i];
        }
    }

    return mClippingActorList[0];
}

ClippingActorInfo* ClippingActorInfoList::findOrNone(const LiveActor* pActor) const {
    for (s32 i = _4 - 1; i >= 0; i--) {
        ClippingActorInfo* info = mClippingActorList[i];
        if (info->mActor == pActor) {
            return info;
        }
    }

    return nullptr;
}

bool ClippingActorInfoList::isInList(const LiveActor* pActor) const {
    for (s32 i = 0; i < _4; i++) {
        ClippingActorInfo* inf = mClippingActorList[i];
        if (inf->mActor == pActor) {
            return true;
        }
    }

    return false;
}
