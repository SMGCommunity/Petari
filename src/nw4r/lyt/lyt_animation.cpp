#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/resourceAccessor.h"
#include "nw4r/lyt/resources.h"

namespace nw4r {
    namespace lyt {
        AnimTransform::AnimTransform() : mpRes(nullptr), mFrame(0) {}

        AnimTransform::~AnimTransform() {}

        u16 AnimTransform::GetFrameSize() const {
            return mpRes->frameSize;
        }

        bool AnimTransform::IsLoopData() const {
            return mpRes->loop != 0;
        }

        AnimTransformBasic::AnimTransformBasic() : mpFileResAry(nullptr), mAnimLinkAry(nullptr), mAnimLinkNum(0) {}

        AnimTransformBasic::~AnimTransformBasic() {
            Layout::DeleteArray(mAnimLinkAry, mAnimLinkNum);
            Layout::DeletePrimArray(mpFileResAry);
        }

        void AnimTransformBasic::SetResource(const res::AnimationBlock* pBlock, ResourceAccessor* pResAccessor) {
            SetResource(pBlock, pResAccessor, pBlock->animContNum);
        }

        void AnimTransformBasic::SetResource(const res::AnimationBlock* pBlock, ResourceAccessor* pResAccessor, u16 animNum) {
            mpRes = pBlock;
            mpFileResAry = nullptr;

            if (pBlock->fileNum > 0) {
                mpFileResAry = Layout::NewArray< void* >(pBlock->fileNum);
                if (mpFileResAry != nullptr) {
                    const u32* fileNameOffs = detail::ConvertOffsToPtr< u32 >(mpRes, sizeof(*mpRes));
                    for (int i = 0; i < mpRes->fileNum; i++) {
                        const char* const name = detail::GetStrTableStr(fileNameOffs, i);
                        mpFileResAry[i] = pResAccessor->GetResource('timg', name, 0);
                    }
                }
            }

            mAnimLinkAry = Layout::NewArray< AnimationLink >(animNum);
            if (mAnimLinkAry != nullptr) {
                mAnimLinkNum = animNum;
                memset(mAnimLinkAry, 0, pBlock->animContNum * sizeof(AnimationLink));

                for (u16 i = 0; i < pBlock->animContNum; i++) {
                    new (&mAnimLinkAry[i]) AnimationLink();
                }
            }
        }
    };  // namespace lyt
};  // namespace nw4r
