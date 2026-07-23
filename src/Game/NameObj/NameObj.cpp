#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjRegister.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Util/SingletonHolder.hpp"

#define FLAG_MOVEMENT_OFF 1u
#define FLAG_SUSPEND 2u
#define FLAG_RESUME 4u

NameObj::NameObj(const char* pName) : mName(pName), mFlag(), mExecutorIdx(-1) {
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
    init(JMapInfoIter());
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
    if ((getFlag() & FLAG_RESUME) == FLAG_RESUME) {
        mFlag &= ~FLAG_RESUME;
    }

    mFlag |= FLAG_SUSPEND;
}

void NameObj::requestResume() {
    if ((getFlag() & FLAG_SUSPEND) == FLAG_SUSPEND) {
        mFlag &= ~FLAG_SUSPEND;
    }

    mFlag |= FLAG_RESUME;
}

void NameObj::syncWithFlags() {
    if ((getFlag() & FLAG_SUSPEND) == FLAG_SUSPEND) {
        mFlag &= ~FLAG_SUSPEND;
        mFlag |= FLAG_MOVEMENT_OFF;
    }

    if ((getFlag() & FLAG_RESUME) == FLAG_RESUME) {
        mFlag &= ~FLAG_RESUME;
        mFlag &= ~FLAG_MOVEMENT_OFF;
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
