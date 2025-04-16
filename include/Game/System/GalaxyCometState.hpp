#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"

/// @brief The state of a Prankster Comet.
class GalaxyCometState : NerveExecutor {
public:
    /// @brief Creates a new `GalaxyCometState`.
    GalaxyCometState();

    /// @brief Destroys the `GalaxyCometState`.
    virtual ~GalaxyCometState();

    /// @brief Updates the working nerve action.
    void update();

    /// @brief Determines if the Prankster Comet is in orbit.
    /// @return `true` if the Prankster Comet is in orbit, `false` otherwise.
    bool isLand() const;

    /// @brief Determines if the Prankster Comet is hidden.
    /// @return `true` if the Prankster Comet is hidden, `false` otherwise.
    bool isHide() const;

    /// @brief Transitions from the working nerve action into the next nerve action.
    void forceToNext();

    /// @brief Returns the index of the working nerve action.
    /// @return The index of the working nerve action.
    s32 getStateIndex() const;

    /// @brief Returns the number of "past seconds."
    /// @return The number of "past seconds."
    s32 getPastSecond() const;

    /// @brief Transitions from the working nerve action into the given nerve action.
    /// @param state The index of the target nerve action.
    /// @param pastSecond The number of "past seconds."
    void setStateAndPastSecond(int state, u16 pastSecond);

private:
    /// @brief Determines if the working nerve action should be updated.
    /* 0x08 */ bool mIsUpdateNerve;
};

namespace {
    NERVE_DECL_NULL(GalaxyCometStateHide);
    NERVE_DECL_NULL(GalaxyCometStateLand);
};
