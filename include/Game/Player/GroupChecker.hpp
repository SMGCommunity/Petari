#pragma once

#include "Game/NameObj/NameObj.hpp"

class HashSortTable;

class GroupChecker : public NameObj {
public:
    GroupChecker(const char*, u32);

    virtual ~GroupChecker();
    virtual void initAfterPlacement();

    void add(const NameObj*);

    /* 0x0C */ HashSortTable* mHashTable;
};

class GroupCheckManager : public NameObj {
public:
    GroupCheckManager(const char*);

    virtual ~GroupCheckManager();

    void add(const NameObj*, s32);
    bool isExist(const NameObj*, s32);

    /* 0x0C */ GroupChecker* mGroups[2];
    /* 0x14 */ u32 _14;
};