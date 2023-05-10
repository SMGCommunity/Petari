#pragma once

#include <revolution.h>
#include "nw4r/math/types.h"
#include "nw4r/ut/Rect.h"

namespace nw4r {
    namespace lyt {
        class DrawInfo {
        public:
            DrawInfo();

            virtual ~DrawInfo();

            bool IsMultipleViewMtxOnDraw() const {
                return mFlag.mulViewDraw;
            }

            const math::MTX34& GetViewMtx() const {
                return mViewMtx;
            }

            math::MTX34 mViewMtx;               // _4
            ut::Rect mViewRect;                 // _34
            math::VEC2 mLocationAdjustScale;
            f32 mGlobalAlpha;                   // _4C

            struct {
                u8 mulViewDraw : 1;
                u8 influencedAlpha : 1;
                u8 locationAdjust : 1;
                u8 invisiblePaneCalculateMtx : 1;
            } mFlag;                // _50
        };
    };
};