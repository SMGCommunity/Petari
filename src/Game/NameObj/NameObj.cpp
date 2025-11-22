#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/SingletonHolder.hpp"

#define FLAG_MOVEMENT_OFF 1u
#define FLAG_SUSPEND 2u
#define FLAG_RESUME 4u

NameObj::NameObj(const char* pName)
    : mName(pName),
      mFlag(0),
      mExecutorIdx(-1) {
    SingletonHolder< NameObjRegister >::get()->add(this);
}

NameObj::~NameObj() {
}

void NameObj::init(const JMapInfoIter& rIter) {
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
    iter.mInfo = nullptr;
    iter.mIndex = -1;

    init(iter);
}

void NameObj::setName(const char* pName) {
    mName = pName;
}

void NameObj::executeMovement() {
    if ((mFlag & FLAG_MOVEMENT_OFF) == FLAG_MOVEMENT_OFF) {
        return;
    }

    movement();
}

void NameObj::requestSuspend() {
    if ((mFlag & FLAG_RESUME) == FLAG_RESUME) {
        mFlag &= ~FLAG_RESUME;
    }

    mFlag |= FLAG_SUSPEND;
}

void NameObj::requestResume() {
    if ((mFlag & FLAG_SUSPEND) == FLAG_SUSPEND) {
        mFlag &= ~FLAG_SUSPEND;
    }

    mFlag |= FLAG_RESUME;
}

void NameObj::syncWithFlags() {
    u16 flag;

    if ((mFlag & FLAG_SUSPEND) == FLAG_SUSPEND) {
        flag = mFlag;
        flag &= ~FLAG_SUSPEND;
        mFlag = flag | FLAG_MOVEMENT_OFF;
    }

    if ((mFlag & FLAG_RESUME) == FLAG_RESUME) {
        flag = mFlag;
        flag &= ~FLAG_RESUME;
        mFlag = flag & ~FLAG_MOVEMENT_OFF;
    }
}

void NameObjFunction::requestMovementOn(NameObj* pObj) {
    pObj->requestResume();
    MR::notifyRequestNameObjMovementOnOff();
}

void NameObjFunction::requestMovementOff(NameObj* pObj) {
    pObj->requestSuspend();
    MR::notifyRequestNameObjMovementOnOff();
}
