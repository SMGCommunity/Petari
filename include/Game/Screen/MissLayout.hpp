#pragma once

#include "Game/Screen/LayoutActor.hpp"

class MissLayout : public LayoutActor {
public:
    /// @brief Creates a new `MissLayout`.
    MissLayout();

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();

    bool isActive() const;
    void start();
    void exeWait();
    void exeActive();
    void exeEnd();
};
