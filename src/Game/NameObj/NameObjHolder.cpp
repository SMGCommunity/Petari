#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <algorithm>
#include <cstring>

namespace {
    struct equal_fullname {
        equal_fullname(const char* pName) : mHash(MR::getHashCode(pName)), mName(pName) {
        }

        bool operator()(const NameObj* pObj) const {
            return strcmp(pObj->getName(), mName) == 0;
        }

        /* 0x00 */ u32 mHash;
        /* 0x04 */ const char* mName;
    };
}  // namespace

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

void NameObjHolder::callMethodAllObj(NameObjMethod pMethod) {
    std::for_each(mObjArray1.begin(), mObjArray1.end(), std::mem_fun(pMethod));
}

void NameObjHolder::clearArray() {
    mObjArray1.clear();
    mObjArray2.clear();
}

NameObj* NameObjHolder::find(const char* pName) {
    NameObj** pCached = std::find_if(mObjArray2.begin(), mObjArray2.end(), equal_fullname(pName));
    if (pCached != mObjArray2.end()) {
        NameObj* pObj = *pCached;
        mObjArray2.erase(pCached);
        mObjArray2.insert(mObjArray2.begin(), pObj);
        return pObj;
    }

    NameObj** pFound = std::find_if(mObjArray1.begin(), mObjArray1.end(), equal_fullname(pName));
    if (pFound == mObjArray1.end()) {
        return nullptr;
    }

    NameObj* pObj = *pFound;
    if (mObjArray2.size() >= mObjArray2.capacity()) {
        mObjArray2.pop_back();
    }

    mObjArray2.insert(mObjArray2.begin(), pObj);
    return pObj;
}
