#include "Game/Util/SystemUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "nw4r/lyt/lyt_layout.hpp"

namespace MR {
    void setLayoutDefaultAllocator() {
        nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
    }
};  