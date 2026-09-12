#include "Game/Player/GroupChecker.hpp"
#include "Game/Util/HashUtil.hpp"

GroupChecker::GroupChecker(const char* pName, u32 a2) : NameObj(pName) {
    mHashTable = new HashSortTable(a2);
}

void GroupChecker::initAfterPlacement() {
    mHashTable->sort();
}

void GroupChecker::add(const NameObj* pObj) {
    const char* name = pObj->mName;
    u32 hashCode = MR::getHashCode(name);
    mHashTable->add(name, 0, true);
}

void GroupCheckManager::add(const NameObj* object, s32 index) {
    mGroups[index]->add(object);
}

bool GroupCheckManager::isExist(const NameObj* object, s32 index) {
    return mGroups[index]->mHashTable->search(object->mName, nullptr);
}

GroupChecker::~GroupChecker() {
}

GroupCheckManager::~GroupCheckManager() {
}

GroupCheckManager::GroupCheckManager(const char* pName) : NameObj(pName) {
    mGroups[0] = new GroupChecker("カメサーチ対象物グループ", 0x20);
    mGroups[1] = new GroupChecker("スピニングボックス反射グループ", 0x8);
    _14 = 2;
}