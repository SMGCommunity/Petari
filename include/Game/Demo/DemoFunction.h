#pragma once

#include "Game/Util.h"

class DemoFunction {
public:
    static void* loadDemoArchive();

    static bool isDemoPartTalk(const char *);
};