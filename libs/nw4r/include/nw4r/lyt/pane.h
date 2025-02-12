#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/Color.h"
#include "nw4r/ut/LinkList.h"
#include "nw4r/ut/RuntimeTypeInfo.h"
#include "nw4r/lyt/types.h"
#include "nw4r/lyt/resources.h"
#include <cstddef>

namespace nw4r {
    namespace ut {
        struct Rect;
    };

    namespace lyt {
        namespace detail {
            class PaneBase {
            public:
                PaneBase();
                virtual ~PaneBase();

                ut::LinkListNode mLink;
            };
        };

        class AnimTransform;
        class AnimationLink;
        class AnimResource;
        class Material;
        class DrawInfo;
        class Pane;

        #ifdef __MWERKS__
        typedef ut::LinkList<Pane, offsetof(detail::PaneBase, mLink)> PaneList;
        #else
        typedef ut::LinkList<Pane, 0> PaneList;
        #endif

        class Pane : public detail::PaneBase {
        public:
            NW4R_UT_RUNTIME_TYPEINFO;
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
            virtual Pane* FindPaneByName(const char *, bool);
            virtual Material* FindMaterialByName(const char *, bool);
            virtual void BindAnimation(AnimTransform *, bool, bool);
            virtual void UnbindAnimation(AnimTransform *, bool);
            virtual void UnbindAllAnimation(bool);
            virtual void UnbindAnimationSelf(AnimTransform *);
            virtual AnimationLink* FindAnimationLinkSelf(AnimTransform *);
            virtual AnimationLink* FindAnimationLinkSelf(const AnimResource&);
            virtual void SetAnimationEnable(AnimTransform *, bool, bool);
            virtual void SetAnimationEnable(const AnimResource &, bool, bool);
            virtual u8 GetMaterialNum() const;
            virtual Material* GetMaterial() const;
            virtual Material* GetMaterial(u32) const;
            virtual void LoadMtx(const DrawInfo &);

            bool IsUserAllocated() const {
                return mbUserAllocated;
            }
            
            Pane* GetParent() const {
                return mpParent;
            }

            void AppendChild(Pane *);

            void SetExtUserDataList(const res::ExtUserDataList *pBlock) {
                mpExtUserDataList = pBlock;
            }

            Pane* mpParent;
            PaneList mChildList;
            AnimationList mAnimList;
            Material* mpMaterial;
            math::VEC3 mTranslate;
            math::VEC3 mRotate;
            math::VEC2 mScale;
            math::VEC2 mSize;
            math::MTX34 mMtx;
            math::MTX34 mGlbMtx;
            const res::ExtUserDataList* mpExtUserDataList;
            u8 mAlpha;
            u8 mGlbAlpha;
            u8 mBasePosition;
            u8 mFlag;
            char mName[17];
            char mUserData[9];
            u8 mbUserAllocated;
            u8 mPadding;
        };
    };
};
