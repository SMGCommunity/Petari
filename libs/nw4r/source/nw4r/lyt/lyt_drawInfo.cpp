#include "nw4r/lyt/lyt_drawInfo.h"
#include <cstring>

namespace nw4r {
    namespace lyt {
        DrawInfo::DrawInfo() : mViewRect(1.0f, 1.0f, 1.0f, 1.0f), mLocationAdjustScale(0.0f, 0.0f), mGlobalAlpha(0.0f) {
            memset(&mFlag, 0, 1);
            PSMTXIdentity(mViewMtx);
        }

        DrawInfo::~DrawInfo() {
            
        }
    };
};