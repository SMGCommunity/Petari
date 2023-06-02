#pragma once

#include <va_list.h>

namespace nw4r {
    namespace db {

        namespace detail {
            typedef struct ConsoleHead ConsoleHead;
        }

        typedef detail::ConsoleHead* ConsoleHandle;

        void Panic(const char *, int, const char *, ...);
    };
};