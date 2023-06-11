#pragma once

#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/ut/RuntimeTypeInfo.h"

namespace nw4r {
    namespace lyt {
        class Picture : public Pane {
            public:
                NW4R_UT_RUNTIME_TYPEINFO;

                Picture(const res::Picture *, const ResBlockSet &);
    
                virtual ~Picture();
                virtual const ut::Color GetVtxColor(u32) const;
                virtual void SetVtxColor(u32, ut::Color);
                virtual u8 GetVtxColorElement(u32) const;
                virtual void SetVtxColorElement(u32, u8);
                virtual void DrawSelf(const DrawInfo &);
                virtual void Append(const TexMap &);

                void Init(u8);
                void ReserveTexCoord(u8);

                ut::Color mVtxColors[4];
                detail::TexCoordAry mTexCoordAry;
        };
    };
};