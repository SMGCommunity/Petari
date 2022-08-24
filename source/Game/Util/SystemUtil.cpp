#include "Game/Util/SystemUtil.h"
#include "Game/Util/MemoryUtil.h"
#include "nw4r/lyt/lyt_layout.h"

namespace MR {
    void setLayoutDefaultAllocator() {
        nw4r::lyt::Layout::mspAllocator = &MR::NewDeleteAllocator::sAllocator;
    }
};  