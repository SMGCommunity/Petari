#pragma once

#include "nw4r/ut/Color.h"
#include "nw4r/ut/LinkList.h"
#include "nw4r/ut/Rect.h"
#include "nw4r/ut/RuntimeTypeInfo.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/lyt/types.h"
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

                ut::LinkListNode mLink;         // _4
            };
        };

        class Pane;
        typedef ut::LinkList<Pane, offsetof(detail::PaneBase, mLink)> PaneList;

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

            Pane* mpParent;                     // _C
            PaneList mChildList;                // _10
            AnimationList mAnimList;            // _1C
            Material* mpMaterial;               // _28
            math::VEC3 mTranslate;              // _2C
            math::VEC3 mRotate;                 // _38
            math::VEC2 mScale;                  // _44
            Size mSize;                         // _4C
            math::MTX34 mMtx;                   // _54
            math::MTX34 mGlbMtx;                // _84
            u8 mAlpha;                          // _B4
            u8 mGlbAlpha;                       // _B5
            u8 mBasePosition;                   // _B6
            u8 mFlag;                           // _B7
            char mName[17];                     // _B8
            char mUserData[9];                  // _C9
            u8 mbUserAllocated;                 // _D2
            u8 pad;                             // _D3
        };
    };
};