#pragma once

#include "Game/Util.hpp"

class DemoFunction {
public:
    static void* loadDemoArchive();

    static bool isDemoPartTalk(const char *);
};