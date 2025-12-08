#pragma once

#include "nw4r/lyt/common.h"
#include "nw4r/lyt/pane.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/Color.h"

namespace nw4r {
    namespace lyt {
        class Window : public Pane {
        public:
            struct Content {
                ut::Color vtxColors[4];
                detail::TexCoordAry texCoordAry;
            };

            struct Frame {
                Frame() : textureFlip(0), pMaterial(nullptr) {}

                u8 textureFlip;
                Material* pMaterial;
            };

            NW4R_UT_RTTI_DECL(Window);

            Window(const res::Window*, const ResBlockSet&);

            virtual ~Window();
            virtual void DrawSelf(const DrawInfo&);
            virtual void AnimateSelf(u32);
            virtual const ut::Color GetVtxColor(u32) const;
            virtual void SetVtxColor(u32, ut::Color);
            virtual u8 GetVtxColorElement(u32) const;
            virtual void SetVtxColorElement(u32, u8);
            virtual Material* FindMaterialByName(const char*, bool);
            virtual void UnbindAnimationSelf(AnimTransform*);
            virtual void SetAnimationEnable(AnimTransform*, bool, bool);
            virtual Material* GetContentMaterial() const;
            virtual Material* GetFrameMaterial(u32) const;
            virtual void DrawContent(const math::VEC2&, const WindowFrameSize&, u8);
            virtual void DrawFrame(const math::VEC2&, const Frame&, const WindowFrameSize&, u8);
            virtual void DrawFrame4(const math::VEC2&, const Frame*, const WindowFrameSize&, u8);
            virtual void DrawFrame8(const math::VEC2&, const Frame*, const WindowFrameSize&, u8);

            WindowFrameSize GetFrameSize(u8 frameNum, const Frame* pFrames);

            InflationLRTB mContentInflation;
            Content mContent;
            Frame* mFrames;
            u8 mFrameNum;
        };
    };  // namespace lyt
};  // namespace nw4r
