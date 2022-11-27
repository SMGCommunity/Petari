#pragma once

#include "nw4r/lyt/lyt_resourceAccessor.h"

namespace nw4r {
    namespace lyt {
        class ArcResourceAccessor : public ResourceAccessor {
        public:
            ArcResourceAccessor();

            bool Attach(void *, const char *);

            ARCHandle mHandle;      // _4
            void* mArchiveBuf;      // _20
            u8 _24[0x30-0x24];
            char mName[128];        // _30
        };
    };
};