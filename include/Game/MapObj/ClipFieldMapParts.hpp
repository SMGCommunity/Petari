#pragma once

#include "Game/MapObj/GeneralMapParts.hpp"

class ClipFieldMapParts : public GeneralMapParts {
public:
    /// @brief Creates a new `ClipFieldMapParts`.
    /// @param pName A pointer to the null-terminated name of the object.
    ClipFieldMapParts(const char* pName);

    virtual ~ClipFieldMapParts();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void connectToScene();
};
