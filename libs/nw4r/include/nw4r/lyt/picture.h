#pragma once

#include "nw4r/lyt/common.h"
#include "nw4r/lyt/pane.h"

namespace nw4r {
    namespace lyt {
        struct ResBlockSet;
        class TexMap;

        class Picture : public Pane {
        public:
            NW4R_UT_RTTI_DECL(Picture);

            Picture(const res::Picture*, const ResBlockSet&);

            virtual ~Picture();
            virtual void DrawSelf(const DrawInfo& rInfo);
            virtual const ut::Color GetVtxColor(u32 idx) const;
            virtual void SetVtxColor(u32 idx, ut::Color color);
            virtual u8 GetVtxColorElement(u32 idx) const;
            virtual void SetVtxColorElement(u32 idx, u8 value);
            virtual void Append(const TexMap& rTexMap);

            void Init(u8);
            void ReserveTexCoord(u8);
            void SetTexCoordNum(u8);

            ut::Color mVtxColors[4];
            detail::TexCoordAry mTexCoordAry;
        };
    };  // namespace lyt
};  // namespace nw4r
