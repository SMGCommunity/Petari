#pragma once

#include <revolution.h>

/// @brief The controller for linearly interpolating a value over time.
class ValueControl {
public:
    /// @brief The direction of progression.
    enum Direction {
        /// @brief The value should approach `0.0`.
        Direction_Backward = -1,

        /// @brief The value should approach `1.0`.
        Direction_Forward = 1,
    };

    /// @brief Creates a new `ValueControl`.
    /// @param maxFrame The maximum number of frames.
    ValueControl(int maxFrame);

    /// @brief Updates the direction of progression to approach `1.0`.
    void setDirToOne();

    /// @brief Updates the direction of progression to approach `1.0` and minimizes the current frame.
    void setDirToOneResetFrame();

    /// @brief Updates the direction of progression to approach `0.0`.
    void setDirToZero();

    /// @brief Updates the direction of progression to approach `0.0` and maximizes the current frame.
    void setDirToZeroResetFrame();

    /// @brief Updates the maximum number of frames, preserving the value of progression.
    /// @param maxFrame The new maximum number of frames.
    void setMaxFrame(int maxFrame);

    /// @brief Updates the current frame based on the direction of progression.
    void update();

    /// @brief Updates the direction of progression to approach `0.0` and minimizes the current frame.
    void setZero();

    /// @brief Updates the direction of progression to approach `1.0` and maximizes the current frame.
    void setOne();

    /// @brief Returns the value of progression between the unit interval.
    /// @return The value of progression.
    f32 getValue() const;

    /// @brief Returns the sign of the direction of progression.
    /// @retval -1 The value approaches `0.0`.
    /// @retval 1 The value approaches `1.0`.
    s32 getDirection() const;

    /// @brief Resets the current frame based on the direction of progression.
    void resetFrame();

    /// @brief The current frame.
    /* 0x0 */ s32 mFrame;

    /// @brief The maximum number of frames.
    /* 0x4 */ s32 mMaxFrame;

    /// @brief The sign of the direction of progression.
    /* 0x8 */ s32 mDirection;

private:
    /// @brief Determines if the given direction of progression suggests approaching `0.0`.
    /// @param direction The sign of the direction of progression to test.
    /// @return `true` if the direction suggests approaching `0.0`, `false` otherwise.
    static bool isDirToZero(s32 direction) {
        return (u32)(-direction & ~direction) >> 31 == 0;
    }
};
