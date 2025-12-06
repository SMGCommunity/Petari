#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class QuakeEffectArea : public AreaObj {
public:
    QuakeEffectArea(int, const char*);
    virtual ~QuakeEffectArea();
};