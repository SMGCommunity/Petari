#include "Game/LiveActor/ClippingActorInfo.h"
#include "Game/LiveActor/ClippingJudge.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

ClippingActorInfo::ClippingActorInfo(LiveActor *pActor) {
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
    }
    else {
        if (MR::isClipped(mActor)) {
            mActor->endClipped();
        }
    }
}

/*
bool ClippingActorInfo::isJudgedToClip() const {
    s32 farClipLevel = mFarClipLevel;
    if (*_14 == 1) {
        farClipLevel = 0;
    }

    f32 val = _8;
    ClippingJudge* judge = MR::getClippingJudge();
    return judge->isJudgedToClipFrustum(*_4, val, farClipLevel);
}
*/

bool ClippingActorInfo::isGroupClipping() const {
    return mInfo;
}

void ClippingActorInfo::setTypeToSphere(f32 a1, const TVec3f *a2) {
    _8 = a1;
    if (!a2) {
        _4 = &mActor->mPosition;
    }
    else {
        _4 = a2;
    }
}

#ifdef NON_MATCHING
void ClippingActorInfo::setGroupClippingNo(const JMapInfoIter &rIter) {
    JMapIdInfo* id_info = new JMapIdInfo();
    *id_info = MR::createJMapIdInfoFromClippingGroupId(rIter);
    mInfo = id_info; 
}
#endif
 
ClippingActorInfoList::ClippingActorInfoList(int a1) {
    _0 = a1;
    _4 = 0;
    mClippingActorList = 0;

    mClippingActorList = new ClippingActorInfo*[a1];

    for (s32 i = 0; i < _0; i++) {
        mClippingActorList[i] = 0;
    }
}

void ClippingActorInfoList::add(ClippingActorInfo *pInfo) {
    mClippingActorList[_4] = pInfo;
    _4++;
}

ClippingActorInfo* ClippingActorInfoList::find(const LiveActor *pActor, s32 *pIndex) const {
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

// ClippingActorInfoList::findOrNone

bool ClippingActorInfoList::isInList(const LiveActor *pActor) const {
    for (s32 i = 0; i < _4; i++) {
        ClippingActorInfo* inf = mClippingActorList[i];
        if (inf->mActor == pActor) {
            return true;
        }
    }

    return false;
}