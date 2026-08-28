#pragma once

#include "Game/MapObj/GeneralMapParts.hpp"

class ClipFieldMapParts : public GeneralMapParts {
public:
    /// @brief Creates a new `ClipFieldMapParts`.
    /// @param pName A pointer to the null-terminated name of the object.
    ClipFieldMapParts(const char* pName);

    /* 0x08 */ virtual ~ClipFieldMapParts();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x48 */ virtual void control();
    /* 0x78 */ virtual void connectToScene();
};
