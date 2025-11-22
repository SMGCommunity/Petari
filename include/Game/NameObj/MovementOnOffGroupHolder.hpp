#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjGroup.hpp"
#include "Game/Util/Array.hpp"

class MovementOnOffGroupHolder : public NameObj {
public:
    MovementOnOffGroupHolder(const char*);
    ~MovementOnOffGroupHolder();

    NameObjGroup* joinToGroup(const char*, NameObj*, u32);
    void          onMovementGroup(const char*);
    NameObjGroup* createGroup(const char*, u32);
    NameObjGroup* findGroupFromName(const char*) const;

private:
    MR::Vector< MR::FixedArray< NameObjGroup*, 32 > > mGroups;
};
