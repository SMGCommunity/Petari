#pragma once

#include "Game/NPC/NPCActor.hpp"

class ButlerExplain : public NPCActor {
public:
    /// @brief Creates a new `ButlerExplain`.
    /// @param pName A pointer to the null-terminated name of the object.
    ButlerExplain(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void startDemo();

    void exeDemo();
};
