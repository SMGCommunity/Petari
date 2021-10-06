#include "Game/AreaObj/AreaObj.h"

AreaObj::AreaObj(int a1, const char *pName) : NameObj(pName) {
    mType = a1;
    mValid = true;
    _15 = 1;
    _16 = 1;
    mObjArg0 = -1;
    mObjArg1 = -1;
    mObjArg2 = -1;
    mObjArg3 = -1;
    mObjArg4 = -1;
    mObjArg5 = -1;
    mObjArg6 = -1;
    mObjArg7 = -1;
    mSwitchCtrl = 0;

    /* todo -- assign forms based on a1 */
}

// AreaObj::init

bool AreaObj::isInVolume(const TVec3f &rPos) const {
    bool ret = false;

    if (mValid && _15 && _16) {
        if (mForm->isInVolume(rPos)) {
            ret = true;
        }
    }

    return ret;
}

void AreaObj::onSwitchA() {
    mSwitchCtrl->onSwitchA();
}

void AreaObj::offSwitchA() {
    mSwitchCtrl->offSwitchA();
}

bool AreaObj::isOnSwitchA() const {
    return mSwitchCtrl->isOnSwitchA();
}

bool AreaObj::isOnSwitchB() const {
    return mSwitchCtrl->isOnSwitchB();
}

bool AreaObj::isValidSwitchA() const {
    return mSwitchCtrl->isOnSwitchA();
}

bool AreaObj::isValidSwitchB() const {
    return mSwitchCtrl->isOnSwitchB();
}

void AreaObj::setFollowMtx(const TPos3f *pMtx) {
    mForm->_4 = (TPos3f*)pMtx;
}

TPos3f* AreaObj::getFollowMtx() const {
    return mForm->_4;
}

AreaObjMgr::AreaObjMgr(s32 count, const char* pName) : NameObj(pName), mArray() {
    _14 = 0;
    _18 = count;
}

// AreaObjMgr::entry
// AreaObjMgr::find_in

void AreaObj::validate() {
    mValid = true;
}

void AreaObj::invalidate() {
    mValid = false;
}

const char* AreaObj::getManagerName() const {
    return mName;
}