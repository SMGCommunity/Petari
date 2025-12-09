#include "nw4r/lyt/bounding.h"
#include "nw4r/lyt/common.h"

namespace nw4r {
    namespace lyt {
        NW4R_UT_RTTI_DEF_DERIVED(Bounding, Pane);
        Bounding::Bounding(const res::Bounding* pBlock, const ResBlockSet&) : Pane(pBlock) {}

        Bounding::~Bounding() {}

        void Bounding::DrawSelf(const DrawInfo&) {}

    };  // namespace lyt
};  // namespace nw4r
