#pragma once

#include "nw4r/lyt/pane.h"

namespace nw4r {
    namespace lyt {
        class ResBlockSet;

        class Bounding : public Pane {
        public:
            NW4R_UT_RTTI_DECL(Bounding);
            Bounding(const res::Bounding*, const ResBlockSet&);

            virtual ~Bounding();
            virtual void DrawSelf(const DrawInfo&);
        };
    };  // namespace lyt
};  // namespace nw4r
