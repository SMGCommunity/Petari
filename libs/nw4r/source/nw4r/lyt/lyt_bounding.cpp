#include "nw4r/lyt/lyt_bounding.h"

namespace nw4r {
    namespace lyt {
        NW4R_UT_RUNTIME_TYPEINFO_DEFINITION(Bounding, Bounding::Pane);

        Bounding::Bounding(const res::Bounding *pBlock, const ResBlockSet &) : Pane(pBlock) {

        }

        Bounding::~Bounding() {

        }

        void Bounding::DrawSelf(const DrawInfo &) {
            
        }
    };
};