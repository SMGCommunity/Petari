#pragma once

#include <cstring.h>

void* operator new(size_t, void* ptr) { 
    return ptr; 
}
