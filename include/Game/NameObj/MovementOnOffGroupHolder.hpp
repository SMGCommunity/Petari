#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class NameObjGroup;

class MovementOnOffGroupHolder : public NameObj {
public:
    /// @brief Creates a new `MovementOnOffGroupHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    MovementOnOffGroupHolder(const char* pName);

    NameObjGroup* joinToGroup(const char*, NameObj*, u32);
    void onMovementGroup(const char*);
    NameObjGroup* createGroup(const char*, u32);
    NameObjGroup* findGroupFromName(const char*) const;

private:
    /* 0x0C */ MR::Vector< MR::FixedArray< NameObjGroup*, 32 > > mGroups;
};
