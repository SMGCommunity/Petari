#pragma once

#include <revolution.h>
#include "nw4r/ut/Rect.h"

namespace nw4r {
    namespace lyt {
        class DrawInfo {
        public:
            DrawInfo();

            virtual ~DrawInfo();

            Mtx mViewMtx;           // _4
            ut::Rect mViewRect;     // _34
            f32 _44;
            f32 _48;
            f32 mGlobalAlpha;       // _4C

            struct {
                u8 mulViewDraw : 1;
                u8 influencedAlpha : 1;
                u8 locationAdjust : 1;
                u8 invisiblePaneCalculateMtx : 1;
            } mFlag;                // _50
        };
    };
};