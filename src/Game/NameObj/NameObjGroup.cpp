#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util/ObjUtil.hpp"

NameObjGroup::NameObjGroup(const char* pName, int numMax) : NameObj(pName), mObjNumMax(), mObjNum(), mObjArray() {
    initObjArray(numMax);
}

NameObjGroup::~NameObjGroup() {
    delete[] mObjArray;
}

void NameObjGroup::registerObj(NameObj* pObj) {
    mObjArray[mObjNum] = pObj;
    mObjNum++;
}

void NameObjGroup::pauseOffAll() const {
    for (s32 i = 0; i < mObjNum; i++) {
        MR::requestMovementOn(mObjArray[i]);
    }
}

void NameObjGroup::initObjArray(int numMax) {
    mObjNumMax = numMax;
    mObjArray = new NameObj*[numMax];

    for (s32 i = 0; i < mObjNumMax; i++) {
        mObjArray[i] = nullptr;
    }
}
