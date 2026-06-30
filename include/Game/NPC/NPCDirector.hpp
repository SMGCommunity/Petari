#pragma once

#include "Game/NameObj/NameObj.hpp"

class NPCCapsParameterReader;
class NPCItemParameterReader;
class ResourceHolder;

class NPCDirector : public NameObj {
public:
    /// @brief Creates a new `NPCDirector`.
    NPCDirector();

    virtual void init(const JMapInfoIter& rIter);

private:
    /* 0x0C */ NPCCapsParameterReader* mCapsParameterReader;
    /* 0x10 */ NPCItemParameterReader* mItemParameterReader;
    /* 0x14 */ ResourceHolder* mDataResourceHolder;
};
