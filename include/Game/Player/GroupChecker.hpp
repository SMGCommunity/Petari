#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util.hpp"

class GroupChecker : public NameObj {
public:
    GroupChecker(const char *, u32);

    virtual ~GroupChecker();
    virtual void initAfterPlacement();

    void add(const NameObj *);

    HashSortTable* mHashTable;  // _C
};

class GroupCheckManager : public NameObj {
public:
    GroupCheckManager(const char *);

    virtual ~GroupCheckManager();

    void add(const NameObj *, s32);
    bool isExist(const NameObj *, s32);

    GroupChecker* mShellSearchGroup;        // _C
    GroupChecker* mSpinningBoxSearchGroup;  // _10
    u32 _14;
};