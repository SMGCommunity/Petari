#pragma once

#include <revolution/mem.h>

namespace nw4r {
    namespace lyt {
        class Layout {
        public:
            static MEMAllocator* mspAllocator;
        };

        MEMAllocator* Layout::mspAllocator;
    };
};