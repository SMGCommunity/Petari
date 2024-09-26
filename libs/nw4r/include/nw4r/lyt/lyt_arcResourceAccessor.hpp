#pragma once

#include "nw4r/lyt/lyt_resourceAccessor.hpp"

namespace nw4r {
    namespace lyt {
        class ArcResourceAccessor : public ResourceAccessor {
        public:
            ArcResourceAccessor();

            bool Attach(void *, const char *);

            ARCHandle mHandle;      // 0x4
            void* mArchiveBuf;      // 0x20
            u8 _24[0x30-0x24];
            char mName[128];        // 0x30
        };
    };
};