#pragma once

#include <revolution.h>

/// @brief Represents a controller for smoothly interpolating values over time.
class ValueControl {
    public:
        /// @brief The direction of progression.
        enum Direction {
            DIRECTION_BACKWARD = -1,
            DIRECTION_FORWARD = 1
        };
        /// @brief Constructor for ValueControl.
        /// @param a1 The maximum number of frames.
        ValueControl(int a1);
    
        /// @brief Gets the current direction of progression.
        /// @return DIRECTION_FORWARD or DIRECTION_BACKWARD.
        s32 getDirection() const;
    
        /// @brief Sets the direction to forward (1).
        void setDirToOne();
    
        /// @brief Sets the direction to forward (1) and resets the current frame.
        void setDirToOneResetFrame();
    
        /// @brief Sets the direction to backward (-1).
        void setDirToZero();
    
        /// @brief Sets the direction to backward (-1) and resets the current frame.
        void setDirToZeroResetFrame();
    
        /// @brief Sets a new maximum frame count, preserving current progress ratio.
        /// @param newMaxFrame The new maximum number of frames.
        void setMaxFrame(int newMaxFrame);
    
        /// @brief Gets the current progress value as a float between 0.0 and 1.0.
        /// @return The current progress ratio.
        f32 getValue() const;
    
        /// @brief Updates the current frame based on the current direction.
        void update();
    
        /// @brief Sets the state to the beginning (0%) and direction to backward.
        void setZero();
    
        /// @brief Sets the state to the end (100%) and direction to forward.
        void setOne();
    
        /// @brief Resets the current frame depending on the direction.
        void resetFrame();
    
    private:
        /// @brief Determines if the given direction indicates backward movement.
        /// @param currentDirection The direction value to test.
        /// @return True if going backward, false otherwise.
        inline static bool goingBackward(s32 currentDirection);
    
        s32 mCurrentFrame;  ///< The current frame.
        s32 mMaxFrame;      ///< The maximum number of frames.
        s32 mDirection;     ///< The direction of progression (1 or -1).
    };
    