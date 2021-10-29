#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjRegister.h"
#include "Game/SingletonHolder.h"

NameObj::NameObj(const char *pName) {
    mName = pName;
    mFlags = 0;
    _A = -1;
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

#ifdef NON_MATCHING
// mFlags does not get reloaded in the block
void NameObj::requestSuspend() {
    if ((mFlags & 0x4) == 0x4) {
        mFlags &= 0xFFFFFFFB;
    }

    mFlags |= 2;
}
#endif

// same issues will persist with these, decomp when requestSuspend matches

// NameObj::requestResume
// NameObj::syncWithFlags
