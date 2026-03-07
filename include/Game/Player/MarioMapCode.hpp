#pragma once

#include <revolution.h>

class Triangle;

class FloorCode {
public:
    /// @brief Creates a new `FloorCode`.
    FloorCode();

    /// @brief Get the floor code for a triangle.
    /// @param triangle The triangle for which to get the floor code.
    /// @return The floor code for the triangle or -1 if a nullpointer or invalid triangle is provided.
    u32 getCode(const Triangle*);
};
