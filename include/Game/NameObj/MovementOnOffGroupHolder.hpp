#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjGroup.hpp"

class MovementOnOffGroupHolder : public NameObj {
public:
    MovementOnOffGroupHolder(const char *);
    ~MovementOnOffGroupHolder();

    NameObjGroup* joinToGroup(const char *, NameObj *, u32);
    void onMovementGroup(const char *);
    NameObjGroup* createGroup(const char *, u32);
    NameObjGroup* findGroupFromName(const char *) const;

    NameObjGroup* mGroups[0x20];    // 0xC
    s32 mCount;                     // 0x8C
};