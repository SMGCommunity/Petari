#include "Game/Animation/XanimeCore.hpp"

void XanimeTrack::init() {
    _0 = 0;
    mWeight = 0.0f;
    _C = 0;
    _8 = 0.0f;
}

XjointInfo::XjointInfo() {
    _0._0.set(1.0f, 1.0f, 1.0f);
    _0._C.x = 0.0f;
    _0._C.y = 0.0f;
    _0._C.z = 0.0f;
    _0._20 = 0.0f;
    _0._1C = 0.0f;
    _0._18 = 0.0f;
    _0._24 = 1.0f;
    _28 = _0;
    _5C = 1.0f;
    _60 = 1.0f;
    _50.zero();
}

void XanimeCore::shareJointTransform(const XanimeCore *pOther) {
    mTransformList = pOther->mTransformList;
}

#ifdef NON_MATCHING
// missing new array call
void XanimeCore::initMember(u32 trackCount) {
    _1C = 1.0f;
    _20 = 1.0f;
    _24 = 0.0f;
    _28 = 0;
    _29 = 0;
    _6 = 0;
    _C = 0;
    mTrackList = new XanimeTrack[trackCount];

    s32 curTrack = 0;

    while (curTrack < trackCount) {
        mTrackList[curTrack].init();
        curTrack++;
    }

    setWeight(0, 1.0f);
}
#endif

XanimeCore::XanimeCore(u32 trackCount, u32 jointCount, u8 a3) {
    _4 = a3;
    mTrackCount = trackCount;
    mJointCount = jointCount;
    mJointList = new XjointInfo[jointCount];
    mTransformList = 0;
    initMember(trackCount);
}

XanimeCore::XanimeCore(u32 trackCount, XanimeCore *pOtherCore) {
    mTrackCount = trackCount;
    mJointCount = pOtherCore->mJointCount;
    _4 = pOtherCore->_4;
    mJointList = pOtherCore->mJointList;
    mTransformList = pOtherCore->mTransformList;
    initMember(trackCount);
}

void XanimeCore::doFreeze() {
    _28 = 1;
    _1C = 0.0f;
}

// XanimeCore::setBck

void XanimeCore::setWeight(u8 idx, f32 weight) {
    mTrackList[idx].mWeight = weight;
}

XanimeCore::~XanimeCore() {

}

XtransformInfo::XtransformInfo() {
    _0.zero();
    _C.zero();
    _20 = 0.0f;
    _1C = 0.0f;
    _18 = 0.0f;
    _24 = 1.0f;
}