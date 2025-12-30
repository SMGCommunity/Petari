#pragma once

#include <revolution/types.h>
#include <revolution/gx/GXStruct.h>

namespace MR {
    /// @brief Returns the most suitable render mode object based on the console's settings.
    /// @return The pointer to the render mode object.
    const GXRenderModeObj *getSuitableRenderMode();

    /// @brief Checks if the progressive mode is enabled.
    /// @return `true` if progressive mode is enabled, `false` otherwise.
    bool isProgressiveModeOn();

    /// @brief Checks if the EuRgb60 mode is enabled.
    /// @return `true` if EuRgb60 mode is enabled, `false` otherwise.
    bool isEuRgb60ModeOn();

    /// @brief Checks if the aspect ratio flag is set to 16:9.
    /// @return `true` if the aspect ratio flag is 16:9, `false` otherwise.
    bool isAspectRatioFlag16Per9();

    /// @brief Returns the required size for an external frame buffer.
    /// @return The size in bytes.
    u32 getRequiredExternalFrameBufferSize();
};

class RenderModeObj {
public:
    /// @brief Returns the maximum width for the current video interface.
    /// @return The maximum width in pixels.
    static u32 getViWidthMax();
};
