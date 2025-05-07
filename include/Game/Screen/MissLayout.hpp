#pragma once

#include "Game/Screen/LayoutActor.hpp"

class MissLayout : LayoutActor {
public:
    /// @brief Creates a new `MissLayout`.
    MissLayout();

    /// @brief Destroys the `MissLayout`.
    virtual ~MissLayout();

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();

    bool isActive() const;
    void start();
    void exeWait();
    void exeActive();
    void exeEnd();
};
