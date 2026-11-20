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
};  // namespace

NameObjHolder::NameObjHolder(int num) {
    mObjArray.init(num);
}

void NameObjHolder::add(NameObj* pObj) {
    mObjArray.push_back(pObj);
}

void NameObjHolder::suspendAllObj() {
    for (int i = 0; i < mObjArray.size(); i++) {
        MR::requestMovementOff(mObjArray[i]);
    }
}

void NameObjHolder::resumeAllObj() {
    for (int i = 0; i < mObjArray.size(); i++) {
        MR::requestMovementOn(mObjArray[i]);
    }
}

void NameObjHolder::syncWithFlags() {
    callMethodAllObj(&NameObj::syncWithFlags);
}

void NameObjHolder::callMethodAllObj(NameObjMethod pMethod) {
    std::for_each(mObjArray.begin(), mObjArray.end(), std::mem_fun(pMethod));
}

void NameObjHolder::clearArray() {
    mObjArray.clear();
    mObjFindCache.clear();
}

NameObj* NameObjHolder::find(const char* pName) {
    NameObj** cached = std::find_if(mObjFindCache.begin(), mObjFindCache.end(), equal_fullname(pName));
    if (cached != mObjFindCache.end()) {
        NameObj* obj = *cached;
        mObjFindCache.erase(cached);
        mObjFindCache.insert(mObjFindCache.begin(), obj);
        return obj;
    }

    NameObj** found = std::find_if(mObjArray.begin(), mObjArray.end(), equal_fullname(pName));
    if (found == mObjArray.end()) {
        return nullptr;
    }

    NameObj* obj = *found;
    if (mObjFindCache.size() >= mObjFindCache.capacity()) {
        mObjFindCache.pop_back();
    }

    mObjFindCache.insert(mObjFindCache.begin(), obj);
    return obj;
}
