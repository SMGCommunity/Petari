#pragma once

#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/ut/RuntimeTypeInfo.h"

namespace nw4r {
    namespace lyt {
        struct ResBlockSet;

        class Bounding : public Pane {
        public:
            NW4R_UT_RUNTIME_TYPEINFO;

            Bounding(const res::Bounding *, const ResBlockSet &);

            virtual ~Bounding();
            virtual void DrawSelf(const DrawInfo &);
        };
    };
};