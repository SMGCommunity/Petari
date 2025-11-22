#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SimpleLayout : public LayoutActor {
public:
    SimpleLayout(const char*, const char*, u32, int);
};

class SimpleEffectLayout : public SimpleLayout {
public:
    SimpleEffectLayout(const char*, const char*, u32, int);
};
