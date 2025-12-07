#pragma once

#include "nw4r/lyt/common.h"
#include "nw4r/lyt/material.h"
#include "nw4r/lyt/resources.h"
#include "nw4r/ut/LinkList.h"
#include <revolution/types.h>

namespace nw4r {
    namespace lyt {
        class Pane;
        class Material;
        class AnimationLink;
        class ResourceAccessor;
        class Group;
        class Layout;

        namespace res {
            struct AnimationBlock;
        };

        class AnimTransform {
        public:
            AnimTransform();

            virtual ~AnimTransform();
            virtual void SetResource(const res::AnimationBlock*, ResourceAccessor*) = 0;
            virtual void SetResource(const res::AnimationBlock*, ResourceAccessor*, u16) = 0;
            virtual void Bind(Pane*, bool) = 0;
            virtual void Bind(Material*) = 0;
            virtual void Animate(u32, Pane*) = 0;
            virtual void Animate(u32, Material*) = 0;

            u16 GetFrameSize() const;
            bool IsLoopData() const;

            ut::LinkListNode mLink;
            const res::AnimationBlock* mpRes;
            f32 mFrame;
        };

        class AnimTransformBasic : public AnimTransform {
        public:
            AnimTransformBasic();

            virtual ~AnimTransformBasic();
            virtual void SetResource(const res::AnimationBlock*, ResourceAccessor*);
            virtual void SetResource(const res::AnimationBlock*, ResourceAccessor*, u16);
            virtual void Bind(Pane*, bool);
            virtual void Bind(Material*);
            virtual void Animate(u32, Pane*);
            virtual void Animate(u32, Material*);

            void** mpFileResAry;
            AnimationLink* mAnimLinkAry;
            u16 mAnimLinkNum;
        };

        class AnimResource {
        public:
            AnimResource();

            explicit AnimResource(const void* anmResBuf) { Set(anmResBuf); }

            void Set(const void*);

            const res::AnimationBlock* GetResourceBlock() const { return mpResBlock; }

            bool IsDescendingBind() const;

            u16 GetGroupNum() const;
            const AnimationGroupRef* GetGroupArray() const;

            u16 GetAnimationShareInfoNum() const;
            const AnimationShareInfo* GetAnimationShareInfoArray() const;

            u16 CalcAnimationNum(Pane*, bool) const;
            u16 CalcAnimationNum(Material*) const;
            u16 CalcAnimationNum(Group*, bool) const;

            const res::BinaryFileHeader* mpFileHeader;
            const res::AnimationBlock* mpResBlock;
            const res::AnimationTagBlock* mpTagBlock;
            const res::AnimationShareBlock* mpShareBlock;
        };

        namespace detail {
            class AnimPaneTree {
            public:
                AnimPaneTree() { Init(); }

                AnimPaneTree(Pane* pTargetPane, const AnimResource& animRes) {
                    Init();
                    Set(pTargetPane, animRes);
                }

                bool IsEnabled() const { return mLinkNum > 0; }

                AnimTransform* Bind(Layout*, Pane*, ResourceAccessor*) const;

                void Set(Pane*, const AnimResource&);
                void Init();

                AnimResource mAnimRes;
                u16 mAnimPaneIdx;
                u16 mLinkNum;
                u16 mAnimMatIdxs[9];
                u8 mAnimMatCnt;
            };

            AnimationLink* FindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans);
            AnimationLink* FindAnimationLink(AnimationList* pAnimList, const AnimResource& animRes);
            void UnbindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans);
        };  // namespace detail
    };  // namespace lyt
};  // namespace nw4r
