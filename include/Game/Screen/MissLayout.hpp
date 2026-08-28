#pragma once

#include "Game/Screen/LayoutActor.hpp"

class MissLayout : public LayoutActor {
public:
    /// @brief Creates a new `MissLayout`.
    MissLayout();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x28 */ virtual void kill();

    bool isActive() const;
    void start();
    void exeWait();
    void exeActive();
    void exeEnd();
};
