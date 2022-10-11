#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjRegister.h"
#include "Game/SingletonHolder.h"
#include "Game/Util.h"

NameObj::NameObj(const char *pName) {
    mName = pName;
    mFlags = 0;
    mExecutorIdx = -1;
    SingletonHolder<NameObjRegister>::sInstance->add(this);
}

NameObj::~NameObj() {

}

void NameObj::init(const JMapInfoIter &) {

}

void NameObj::initAfterPlacement() {

}

void NameObj::movement() {

}

void NameObj::draw() const {

}

void NameObj::calcAnim() {

}

void NameObj::calcViewAndEntry() {

}

void NameObj::initWithoutIter() {
    JMapInfoIter iter;
    iter.mInfo = 0;
    iter._4 = -1;
    init(iter);
}

void NameObj::setName(const char *pName) {
    mName = pName;
}

void NameObj::executeMovement() {
    if ((mFlags & 0x1) != 1) {
        movement();
    }
}

void NameObj::requestSuspend() {
    u16 flag = (mFlags & 0x4);
    if (flag == 0x4) {
        mFlags &= ~0x4;
    }
    mFlags |= 2; 
}

void NameObj::requestResume() {
    u16 flag = (mFlags & 0x2);
    if (flag == 0x2) {
        mFlags &= ~0x2;
    }
    mFlags |= 4;
}

void NameObj::syncWithFlags() {
    u16 flag = (mFlags & 0x2);
    if (flag == 0x2) {
        flag = mFlags;
        flag &= ~0x2;
        flag |= 0x1;
        mFlags = flag;
    }

    flag = (mFlags & 0x4);
    if (flag == 0x4) {
        flag = mFlags & ~0x4;
        mFlags = flag & ~0x1;
    }
}

void NameObjFunction::requestMovementOn(NameObj *pObj) {
    pObj->requestResume();
    MR::notifyRequestNameObjMovementOnOff();
}

void NameObjFunction::requestMovementOff(NameObj *pObj) {
    pObj->requestSuspend();
    MR::notifyRequestNameObjMovementOnOff();
}
