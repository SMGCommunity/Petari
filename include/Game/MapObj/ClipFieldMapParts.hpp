#pragma once

#include "Game/MapObj/GeneralMapParts.hpp"

class ClipFieldMapParts : public GeneralMapParts {
public:
    ClipFieldMapParts(const char*);

    virtual ~ClipFieldMapParts();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void connectToScene();
};