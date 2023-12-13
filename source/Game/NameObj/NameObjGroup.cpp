#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util.hpp"

NameObjGroup::NameObjGroup(const char *pGroupName, int maxCount) : NameObj(pGroupName) {
    _C = 0;
    mObjectCount = 0;
    mObjects = 0;
    initObjArray(maxCount);
}

NameObjGroup::~NameObjGroup() {
    delete[] mObjects;
}

void NameObjGroup::registerObj(NameObj *pObj) {
    mObjects[mObjectCount] = pObj;
    mObjectCount++;
}

void NameObjGroup::pauseOffAll() const {
    for (s32 i = 0; i < mObjectCount; i++) {
        MR::requestMovementOn(mObjects[i]);
    }
}

void NameObjGroup::initObjArray(int maxCount) {
    _C = maxCount;
    mObjects = new NameObj*[maxCount];

    for (s32 i = 0; i < _C; i++) {
        mObjects[i] = 0;
    }
}