#pragma once

#include <revolution/os.h>

namespace JASThreadingModel {
    template < typename T >
    class InterruptsDisable {
    public:
        struct Lock {
            Lock(const T&) NO_INLINE { success = OSDisableInterrupts(); }
            ~Lock() NO_INLINE;
            u32 success;
        };
    };
}  // namespace JASThreadingModel
