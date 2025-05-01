#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Util/ObjUtil.hpp"

NameObjHolder::NameObjHolder(int num) {
    mObjArray1.init(num);
}

void NameObjHolder::add(NameObj* pObj) {
    mObjArray1.push_back(pObj);
}

void NameObjHolder::suspendAllObj() {
    for (int i = 0; i < mObjArray1.size(); i++) {
        MR::requestMovementOff(mObjArray1[i]);
    }
}

void NameObjHolder::resumeAllObj() {
    for (int i = 0; i < mObjArray1.size(); i++) {
        MR::requestMovementOn(mObjArray1[i]);
    }
}

void NameObjHolder::syncWithFlags() {
    callMethodAllObj(&NameObj::syncWithFlags);
}

#ifdef NON_MATCHING
// Missing stack variables?
void NameObjHolder::callMethodAllObj(NameObjMethod pMethod) {
    NameObjMethod method = pMethod;
    NameObj** begin = mObjArray1.begin();
    NameObj** end = mObjArray1.end();

    for (NameObj** p = begin; p != end; p++) {
        (*p->*method)();
    }
}
#endif

void NameObjHolder::clearArray() {
    mObjArray1.clear();
    mObjArray2.clear();
}

// NameObjHolder::find
