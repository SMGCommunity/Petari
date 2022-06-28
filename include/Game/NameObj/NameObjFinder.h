#pragma once

#include "Game/NameObj/NameObjHolder.h"

class NameObj;

class NameObjFinder {
public:
    static NameObj* find(const char *);
};