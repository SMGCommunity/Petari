#pragma once

#include "Game/NameObj/NameObjHolder.h"

class NameObjGroup;

class NameObjFinder {
public:
    static NameObjGroup* find(const char *);
};