#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Util.hpp"

NameObjHolder::NameObjHolder(int a1) : 
    mObjs(0), _4(0), mObjCount(0), _4C(0) {
        mObjs = new NameObj*[a1];
        _4 = a1;
    }

void NameObjHolder::add(NameObj *pObj) {
    u32 count = mObjCount;
    mObjCount = count + 1;
    mObjs[count] = pObj;
}

void NameObjHolder::suspendAllObj() {
    for (int i = 0; i < mObjCount; i++) {
        MR::requestMovementOff(mObjs[i]);
    }
}

void NameObjHolder::resumeAllObj() {
    for (int i = 0; i < mObjCount; i++) {
        MR::requestMovementOn(mObjs[i]);
    }
}


void NameObjHolder::syncWithFlags() {
    callMethodAllObj(&NameObj::syncWithFlags);
}

#ifdef NON_MATCHING
// some weird register usage and ordering
void NameObjHolder::callMethodAllObj(func functionPtr) {
    func function = functionPtr;

    NameObj** start = mObjs;
    NameObj** end = &mObjs[mObjCount];
    function = functionPtr;

    while (start != end) {
        (*start->*function)();
        start++;
    }
}
#endif

void NameObjHolder::clearArray() {
    mObjCount = 0;
    _4C = 0;
}