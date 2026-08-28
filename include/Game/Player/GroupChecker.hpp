#pragma once

#include "Game/NameObj/NameObj.hpp"

class HashSortTable;

class GroupChecker : public NameObj {
public:
    GroupChecker(const char*, u32);

    /* 0x08 */ virtual ~GroupChecker();
    /* 0x10 */ virtual void initAfterPlacement();

    void add(const NameObj*);

    HashSortTable* mHashTable;  // 0xC
};

class GroupCheckManager : public NameObj {
public:
    GroupCheckManager(const char*);

    /* 0x08 */ virtual ~GroupCheckManager();

    void add(const NameObj*, s32);
    bool isExist(const NameObj*, s32);

    GroupChecker* mShellSearchGroup;        // 0xC
    GroupChecker* mSpinningBoxSearchGroup;  // 0x10
    u32 _14;
};