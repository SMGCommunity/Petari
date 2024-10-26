#pragma once

#include "nw4r/ut/Color.hpp"
#include "nw4r/ut/LinkList.hpp"
#include "nw4r/ut/Rect.hpp"
#include "nw4r/ut/RuntimeTypeInfo.hpp"
#include "nw4r/lyt/lyt_drawInfo.hpp"
#include "nw4r/lyt/lyt_resources.hpp"
#include "nw4r/lyt/types.hpp"
#include <cstddef>
#include <revolution.h>

namespace nw4r {
    namespace lyt {

        class Material;

        namespace detail {
            class PaneBase {
            public:
                PaneBase();

                virtual ~PaneBase();

                ut::LinkListNode mLink;         // 0x4
            };
        };

        class Pane;
        #ifdef __MWERKS__
        typedef ut::LinkList<Pane, offsetof(detail::PaneBase, mLink)> PaneList;
        #else
        typedef ut::LinkList<Pane, 0> PaneList;
        #endif


        class Pane : public detail::PaneBase {
        public:
            NW4R_UT_RUNTIME_TYPEINFO;

            Pane(const res::Pane *);

            virtual ~Pane();
            virtual void CalculateMtx(const DrawInfo &);
            virtual void Draw(const DrawInfo &);
            virtual void DrawSelf(const DrawInfo &);
            virtual void Animate(u32);
            virtual void AnimateSelf(u32);
            virtual const ut::Color GetVtxColor(u32) const;
            virtual void SetVtxColor(u32, ut::Color);
            virtual u8 GetColorElement(u32) const;
            virtual void SetColorElement(u32, u8);
            virtual u8 GetVtxColorElement(u32) const;
            virtual void SetVtxColorElement(u32, u8);
            virtual Pane* FindPaneByName(const char *, bool recursive = true);
            virtual Material* FindMaterialByName(const char *, bool);
            virtual void BindAnimation(AnimTransform *, bool recursive = true);
            virtual void UnbindAnimation(AnimTransform *, bool);
            virtual void UnbindAllAnimation(bool);
            virtual void UnbindAnimationSelf(AnimTransform *);
            virtual AnimationLink* FindAnimationLinkSelf(AnimTransform *);
            virtual void SetAnimationEnable(AnimTransform *, bool, bool);
            virtual Material* GetMaterial() const;
            virtual void LoadMtx(const DrawInfo &);

            void AppendChild(Pane *);

            void InsertChild(PaneList::Iterator, Pane *);

            void RemoveChild(Pane *);
            void SetName(const char *);
            void AddAnimationLink(AnimationLink *);

            const math::VEC2 GetVtxPos() const;

            bool IsVisible() const {
                return detail::TestBit(mFlag, 0);
            }

            void SetVisible(bool visible) {
                detail::SetBit(&mFlag, 0, visible);
            }

            bool IsUserAllocated() const {
                return mbUserAllocated;
            }

            Pane* mpParent;                     // 0xC
            PaneList mChildList;                // 0x10
            AnimationList mAnimList;            // 0x1C
            Material* mpMaterial;               // 0x28
            math::VEC3 mTranslate;              // 0x2C
            math::VEC3 mRotate;                 // 0x38
            math::VEC2 mScale;                  // 0x44
            Size mSize;                         // 0x4C
            math::MTX34 mMtx;                   // 0x54
            math::MTX34 mGlbMtx;                // 0x84
            u8 mAlpha;                          // 0xB4
            u8 mGlbAlpha;                       // 0xB5
            u8 mBasePosition;                   // 0xB6
            u8 mFlag;                           // 0xB7
            char mName[17];                     // 0xB8
            char mUserData[9];                  // 0xC9
            u8 mbUserAllocated;                 // 0xD2
            u8 pad;                             // 0xD3
        };
    };
};
