#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/Rect.h"

namespace nw4r {
    namespace lyt {
        class DrawInfo {
        public:
            DrawInfo();
            virtual ~DrawInfo();

            const math::MTX34& GetViewMtx() const { return mViewMtx; }
            bool IsMultipleViewMtxOnDraw() const { return mFlag.mulViewDraw; }
            void SetMultipleViewMtxOnDraw(bool bEnable) { mFlag.mulViewDraw = bEnable; }
            bool IsInfluencedAlpha() const { return mFlag.influencedAlpha; }
            void SetInfluencedAlpha(bool bEnable) { mFlag.influencedAlpha = bEnable; }
            bool IsLocationAdjust() const { return mFlag.locationAdjust; }
            void SetLocationAdjust(bool bEnable) { mFlag.locationAdjust = bEnable; }
            const math::VEC2& GetLocationAdjustScale() const { return mLocationAdjustScale; }
            void SetLocationAdjustScale(const math::VEC2& scale) { mLocationAdjustScale = scale; }
            f32 GetGlobalAlpha() const { return mGlobalAlpha; }
            void SetGlobalAlpha(f32 alpha) { mGlobalAlpha = alpha; }
            bool IsInvisiblePaneCalculateMtx() const { return mFlag.invisiblePaneCalculateMtx; }
            bool IsYAxisUp() const { return mViewRect.bottom - mViewRect.top < 0.0f; }

            math::MTX34 mViewMtx;
            ut::Rect mViewRect;
            math::VEC2 mLocationAdjustScale;
            f32 mGlobalAlpha;

            struct {
                u8 mulViewDraw : 1;
                u8 influencedAlpha : 1;
                u8 locationAdjust : 1;
                u8 invisiblePaneCalculateMtx : 1;
            } mFlag;
        };
    };  // namespace lyt
};  // namespace nw4r
