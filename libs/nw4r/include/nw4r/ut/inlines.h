#pragma once

namespace nw4r {
    namespace ut {
        class NonCopyable {
        protected:
            inline NonCopyable() { }
            inline ~NonCopyable() { }
        private:
            NonCopyable(const NonCopyable &);
            const NonCopyable& operator=(const NonCopyable&);
        };

        template<typename T>
        inline T BitExtract(T bits, int pos, int len=1) {
            const T mask = static_cast<T>((1UL << len) - 1);
            return static_cast<T>((bits >> pos) & mask);
        }
    };
};
