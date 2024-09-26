#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util.hpp"

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
    u16 flags = mFlags;
    if (((s16)flags & 0x1) != 1) {
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
