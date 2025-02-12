#pragma once

#include <revolution.h>
#include <cstddef>
#include "nw4r/ut/LinkList.h"

namespace nw4r {
    namespace lyt {
        namespace detail {
            typedef s16 ResS16;
            typedef u16 ResU16;
            typedef s32 ResS32;
            typedef u32 ResU32;

            template<typename T>
            inline const T* ConvertOffsToPtr(const void *baseAddr, unsigned int offs) {
                return reinterpret_cast<const T*>(static_cast<const u8*>(baseAddr) + offs);
            }
        };

        struct Size {
            Size(f32 aWidth, f32 aHeight) : width(aWidth), height(aHeight) {

            }

            f32 width;
            f32 height;
        };

        struct InflationLRTB {
            f32 l;
            f32 r;
            f32 t;
            f32 b;
        };

        struct WindowFrameSize {
            f32 l;
            f32 r;
            f32 t;
            f32 b;
        };


        class AnimTransform;

        class AnimationLink {
        public:
            AnimationLink();

            ut::LinkListNode mLink;
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

        #ifdef __MWERKS__
        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationList;
        #else
        typedef ut::LinkList<AnimationLink, 0> AnimationList;
        #endif

        struct AnimationGroupRef {
            AnimationGroupRef() : flag(0) {

            }

            const char* GetName() const { 
                return name;
            }

            char name[17];
            u8 flag;
            u8 padding[2];
        };

        struct AnimationShareInfo {

            const char* GetSrcPaneName() const { 
                return srcPaneName; 
            }

            const char* GetTargetGroupName() const {
                return targetGroupName;
            }

            char srcPaneName[17];
            char targetGroupName[17];
            u8 padding[2];
        };
    };
};
