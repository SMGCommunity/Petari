#pragma once

#include <revolution.h>

/// @brief Represents a controller for smoothly interpolating values over time.
class ValueControl {
public:
    /// @brief The direction of progression.
    enum Direction {
        Direction_Backward = -1,
        Direction_Forward = 1,
    };

    /// @brief Constructor for ValueControl.
    /// @param maxFrame The maximum number of frames.
    ValueControl(int maxFrame);

    /// @brief Sets the direction to forward (1).
    void setDirToOne();

    /// @brief Sets the direction to forward (1) and resets the current frame.
    void setDirToOneResetFrame();

    /// @brief Sets the direction to backward (-1).
    void setDirToZero();

    /// @brief Sets the direction to backward (-1) and resets the current frame.
    void setDirToZeroResetFrame();

    /// @brief Sets a new maximum frame count, preserving current progress ratio.
    /// @param maxFrame The new maximum number of frames.
    void setMaxFrame(int maxFrame);

    /// @brief Updates the current frame based on the current direction.
    void update();

    /// @brief Sets the state to the beginning (0%) and direction to backward.
    void setZero();

    /// @brief Sets the state to the end (100%) and direction to forward.
    void setOne();

    /// @brief Gets the current progress value as a float between 0.0 and 1.0.
    /// @return The current progress ratio.
    f32 getValue() const;

    /// @brief Gets the current direction of progression.
    /// @return DIRECTION_FORWARD or DIRECTION_BACKWARD.
    s32 getDirection() const;

    /// @brief Resets the current frame depending on the direction.
    void resetFrame();

private:
    /// @brief Determines if the given direction indicates backward movement.
    /// @param direction The direction value to test.
    /// @return True if going backward, false otherwise.
    static bool isDirToZero(s32 direction) {
        return (u32)(-direction & ~direction) >> 31 == 0;
    }

    /* 0x0 */ s32 mFrame;         ///< The current frame.
    /* 0x4 */ s32 mMaxFrame;      ///< The maximum number of frames.
    /* 0x8 */ s32 mDirection;     ///< The direction of progression (1 or -1).
};
