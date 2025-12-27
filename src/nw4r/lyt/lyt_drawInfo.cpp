#include "nw4r/lyt/drawInfo.h"
#include "nw4r/math/types.h"
#include <mem.h>

namespace nw4r {
    namespace lyt {
        DrawInfo::DrawInfo() : mLocationAdjustScale(1.0f, 1.0f), mGlobalAlpha(1.0f) {
            memset(&mFlag, 0, sizeof(mFlag));
            MTX34Identity(&mViewMtx);
        }

        DrawInfo::~DrawInfo() {}
    };  // namespace lyt
};  // namespace nw4r
