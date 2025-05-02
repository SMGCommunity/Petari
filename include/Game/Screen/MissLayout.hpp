#pragma once

#include "Game/LiveActor/Nerve.hpp"
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

namespace {
    NERVE_DECL(MissLayoutNrvWait, MissLayout, MissLayout::exeWait);
    NERVE_DECL(MissLayoutNrvActive, MissLayout, MissLayout::exeActive);
    NERVE_DECL(MissLayoutNrvEnd, MissLayout, MissLayout::exeEnd);
};
