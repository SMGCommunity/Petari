#pragma once

#include <revolution.h>

namespace AudUtil {
    /// @brief Maps a number from the interval `[0, max]` to the interval `[0.0f, end]`.
    /// @param x The number to evaluate.
    /// @param max The upper bound of the old interval.
    /// @param end The upper bound of the new interval.
    /// @return The mapped number.
    f32 linerMax(s32 x, s32 max, f32 end);

    /// @brief Maps a number from the interval `[min, max]` to the interval `[start, end]`.
    /// @param x The number to evaluate.
    /// @param min The lower bound of the old interval.
    /// @param max The upper bound of the old interval.
    /// @param start The lower bound of the new interval.
    /// @param end The upper bound of the new interval.
    /// @return The mapped number.
    f32 linerMinMax(s32 x, s32 min, s32 max, f32 start, f32 end);

    bool isProperCurrentStageOrAudioDebugMode();
};  // namespace AudUtil
