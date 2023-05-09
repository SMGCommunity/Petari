#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/LinkList.h"
#include <cstddef>

namespace nw4r {
    namespace lyt {

        struct Size {
            Size(f32 w, f32 h) {
                width = w;
                height = h;
            }

            f32 width;
            f32 height;
        };

        class AnimTransform;

        class AnimationLink {
        public:

            AnimTransform* GetAnimTransform() const {
                return mAnimTrans;
            }

            u16 GetIndex() const {
                return mIdx;
            }

            bool IsEnable() const {
                return !mbDisable;
            }

            void SetEnable(bool en) {
                mbDisable = !en;
            }

            ut::LinkListNode mLink;
        private:
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

        namespace detail {
            template<typename T>
            inline void SetBit(T* pBits, int pos, bool val) {
                const T mask = T(~(1 << pos));
                *pBits &= mask;
                *pBits |= (val ? 1 : 0) << pos;
            }

            template<typename T>
            inline bool TestBit(T bits, int pos) {
                const T mask = T(1 << pos);
                return 0 != (bits & mask);
            }
        };

        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationList; 
    };
};

typedef nw4r::math::VEC2 (*TexCoordQuad)[4];
typedef const nw4r::math::VEC2 (*ConstTexCoordQuad)[4];