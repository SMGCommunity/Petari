#include "Game/Player/GroupChecker.hpp"

GroupChecker::GroupChecker(const char *pName, u32 a2) : NameObj(pName)
{
    mHashTable = new HashSortTable(a2);
}

void GroupChecker::initAfterPlacement()
{
    mHashTable->sort();
}

void GroupChecker::add(const NameObj *pObj)
{
    const char *name = pObj->mName;
    u32 hashCode = MR::getHashCode(name);
    mHashTable->add(name, 0, true);
}

/*
void GroupCheckManager::add(const NameObj *pObj, s32 idx) {
    mShellSearchGroup[idx].add(pObj);
}
*/

GroupChecker::~GroupChecker() {}

GroupCheckManager::~GroupCheckManager() {}

GroupCheckManager::GroupCheckManager(const char *pName) : NameObj(pName)
{
    mShellSearchGroup = new GroupChecker("カメサーチ対象物グループ", 0x20);
    mSpinningBoxSearchGroup = new GroupChecker("スピニングボックス反射グループ", 0x8);
    _14 = 2;
}