#pragma once

#include <revolution/types.h>

namespace JGadget {

    namespace {

        template <typename T>
        class TPRIsEnd_value_ {
        public:
            TPRIsEnd_value_(T _end)
                : end(_end) {}

            template <typename U>
            inline bool operator()(U const other) {
                return other == end;
            }

        private:
            T end;
        };

        template <typename T>
        class TPRIsEqual_pointer_ {
        public:
            TPRIsEqual_pointer_(const T* _ptr)
                : ptr(_ptr) {}

            template <typename U>
            inline bool operator()(const U* other) {
                return other == ptr;
            }

        private:
            const T* ptr;
        };

    } // namespace

} // namespace JGadget