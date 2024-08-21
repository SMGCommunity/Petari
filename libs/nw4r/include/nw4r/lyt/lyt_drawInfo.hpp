#pragma once

#include <revolution.h>
#include "nw4r/math/types.hpp"
#include "nw4r/ut/Rect.hpp"

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

            math::MTX34 mViewMtx;               // 0x4
            ut::Rect mViewRect;                 // 0x34
            math::VEC2 mLocationAdjustScale;
            f32 mGlobalAlpha;                   // 0x4C

            struct {
                u8 mulViewDraw : 1;
                u8 influencedAlpha : 1;
                u8 locationAdjust : 1;
                u8 invisiblePaneCalculateMtx : 1;
            } mFlag;                // 0x50
        };
    };
};