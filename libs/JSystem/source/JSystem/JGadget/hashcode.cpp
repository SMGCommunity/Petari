#include "JSystem/JGadget/hashcode.h"

namespace JGadget {
    namespace {
        template<typename T>
        class TPRIsEnd_value_ {
        public:

            TPRIsEnd_value_(T value) : end(value) {}
            
            template<typename U>
            inline bool operator()(U const other) {
                return other == end;
            }
        private:
            T end;
        };
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
