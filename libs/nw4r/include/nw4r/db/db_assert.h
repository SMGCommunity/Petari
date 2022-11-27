#pragma once

#include <va_list.h>

namespace nw4r {
    namespace db {
        void Panic(const char *, int, const char *, ...);
    };
};