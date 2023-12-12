#include "JSystem/JGadget/hashcode.hpp"
#include "JSystem/JGadget/predicate.hpp"

namespace JGadget {
    namespace {
        template<typename T>
        inline u32 getHashCode_(const T *data, TPRIsEnd_value_<T> p) {
            u32 hash = 0;
            while(!p(*data)) {
                hash = *data + hash * 31;
                data++;
            }
            return hash;
        }
    }
    u32 getHashCode(const char *data) {
        TPRIsEnd_value_<char> p('\0');
        return getHashCode_(data, p);
    }
    
}
