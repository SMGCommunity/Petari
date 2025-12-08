#pragma once

#include "nw4r/ut/LinkList.h"
#include <cstddef>
#include <mem.h>
#include <revolution.h>

namespace nw4r {
    namespace lyt {
        namespace detail {
            typedef s16 ResS16;
            typedef u16 ResU16;
            typedef s32 ResS32;
            typedef u32 ResU32;

            template < typename T >
            inline const T* ConvertOffsToPtr(const void* baseAddr, unsigned int offs) {
                return reinterpret_cast< const T* >(static_cast< const u8* >(baseAddr) + offs);
            }

            template < typename T >
            inline bool TestBit(T bits, int pos) {
                const T mask = T(1 << pos);

                return 0 != (bits & mask);
            }
        };  // namespace detail

        struct Size {
            Size() : width(0.0f), height(0.0f) {}

            Size(f32 aWidth, f32 aHeight) : width(aWidth), height(aHeight) {}

            f32 width;
            f32 height;
        };

        inline bool operator==(const Size& a, const Size& b) {
            return a.width == b.width && a.height == b.height;
        }

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

        struct TexCoordGen {
            TexCoordGen() : reserve(0) { Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY); }

            TexCoordGen(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx)
                : reserve(0)

            {
                Set(aTexGenType, aTexGenSrc, aTexMtx);
            }

            void Set(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx) {
                texGenType = aTexGenType;
                texGenSrc = aTexGenSrc;
                texMtx = u8(aTexMtx);
            }

            GXTexGenType GetTexGenType() const { return GXTexGenType(texGenType); }
            GXTexGenSrc GetTexGenSrc() const { return GXTexGenSrc(texGenSrc); }
            u32 GetTexMtx() const { return texMtx; }

            u8 texGenType;
            u8 texGenSrc;
            u8 texMtx;
            u8 reserve;
        };

        class AnimTransform;

        class AnimationLink {
        public:
            AnimationLink() { Reset(); }

            void Reset() { Set(nullptr, 0, false); }

            void Set(AnimTransform* pTrans, u16 idx, bool dis) {
                mAnimTrans = pTrans;
                mIdx = idx;
                mbDisable = dis;
            }

            bool IsEnable() const { return !mbDisable; }
            void SetEnable(bool enable) { mbDisable = !enable; }
            u16 GetIndex() const { return mIdx; }
            AnimTransform* GetAnimTransform() const { return mAnimTrans; }

            ut::LinkListNode mLink;
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

#ifdef __MWERKS__
        typedef ut::LinkList< AnimationLink, offsetof(AnimationLink, mLink) > AnimationList;
#else
        typedef ut::LinkList< AnimationLink, 0 > AnimationList;
#endif

        struct AnimationGroupRef {
            AnimationGroupRef() : flag(0) {
                memset(name, 0, sizeof(name));
                memset(padding, 0, sizeof(padding));
            }

            const char* GetName() const { return name; }

            char name[17];
            u8 flag;
            u8 padding[2];
        };

        struct AnimationShareInfo {
            const char* GetSrcPaneName() const { return srcPaneName; }

            const char* GetTargetGroupName() const { return targetGroupName; }

            char srcPaneName[17];
            char targetGroupName[17];
            u8 padding[2];
        };
    };  // namespace lyt
};  // namespace nw4r
