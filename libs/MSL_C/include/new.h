#pragma once

#include <cstring.h>

#define _MSL_THROW throw()

void* operator new(size_t, void* ptr) _MSL_THROW { 
    return ptr; 
}

void operator delete[](void *ptr) _MSL_THROW;