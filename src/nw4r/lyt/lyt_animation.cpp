#include <revolution.h>

#include <nw4r/lyt/common.h>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/group.h>
#include <nw4r/lyt/material.h>
#include <nw4r/lyt/animation.h>
#include <nw4r/lyt/layout.h>
#include <nw4r/lyt/resourceAccessor.h>
#include <new>

const f32 R_SAME_TOLERANCE = 1.0e-5F;
const f32 R_FRAME_TOLERANCE = 0.001F;

namespace nw4r {
    namespace lyt {

        namespace {

            inline bool RIsSame(const f32 a, const f32 b, const f32 tolerance = R_SAME_TOLERANCE) {
                f32 c = a - b;
                return (-tolerance < c && c < tolerance);
            }

            u16 GetStepCurveValue(f32 frame, const res::StepKey* keyArray, u32 keySize) {
                if (keySize == 1 || frame <= keyArray[0].frame) {
                    return keyArray[0].value;
                } else if (frame >= keyArray[keySize - 1].frame) {
                    return keyArray[keySize - 1].value;
                }

                int ikeyL = 0;
                int ikeyR = (int)keySize - 1;
                while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
                    int ikeyCenter = (ikeyL + ikeyR) / 2;
                    const res::StepKey& centerKey = keyArray[ikeyCenter];
                    if (frame < centerKey.frame) {
                        ikeyR = ikeyCenter;
                    } else {
                        ikeyL = ikeyCenter;
                    }
                }

                if (RIsSame(frame, keyArray[ikeyR].frame, R_FRAME_TOLERANCE)) {
                    return keyArray[ikeyR].value;
                } else {
                    return keyArray[ikeyL].value;
                }
            }

            f32 GetHermiteCurveValue(f32 frame, const res::HermiteKey* keyArray, u32 keySize) {
                if (keySize == 1 || frame <= keyArray[0].frame) {
                    return keyArray[0].value;
                } else if (frame >= keyArray[keySize - 1].frame) {
                    return keyArray[keySize - 1].value;
                }

                int ikeyL = 0;
                int ikeyR = (int)keySize - 1;
                while (ikeyL != ikeyR - 1 && ikeyL != ikeyR) {
                    int ikeyCenter = (ikeyL + ikeyR) / 2;
                    if (frame <= keyArray[ikeyCenter].frame) {
                        ikeyR = ikeyCenter;
                    } else {
                        ikeyL = ikeyCenter;
                    }
                }

                const res::HermiteKey& key0 = keyArray[ikeyL];
                const res::HermiteKey& key1 = keyArray[ikeyR];
                if (RIsSame(frame, key1.frame, R_FRAME_TOLERANCE)) {
                    if (ikeyR < keySize - 1 && key1.frame == keyArray[ikeyR + 1].frame) {
                        return keyArray[ikeyR + 1].value;
                    } else {
                        return key1.value;
                    }
                }
                f32 t1 = frame - key0.frame;
                f32 t2 = 1.0F / (key1.frame - key0.frame);
                f32 v0 = key0.value;
                f32 v1 = key1.value;
                f32 s0 = key0.slope;
                f32 s1 = key1.slope;

                f32 t1t1t2 = t1 * t1 * t2;
                f32 t1t1t2t2 = t1t1t2 * t2;
                f32 t1t1t1t2t2 = t1 * t1t1t2t2;
                f32 t1t1t1t2t2t2 = t1t1t1t2t2 * t2;

                return v0 * (2.0F * t1t1t1t2t2t2 - 3.0F * t1t1t2t2 + 1.0F) + v1 * (-2.0F * t1t1t1t2t2t2 + 3.0F * t1t1t2t2) +
                       s0 * (t1t1t1t2t2 - 2.0F * t1t1t2 + t1) + s1 * (t1t1t1t2t2 - t1t1t2);
            }

            void AnimatePaneSRT(Pane* pPane, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    const res::HermiteKey* keys = detail::ConvertOffsToPtr< res::HermiteKey >(pAnimTarget, pAnimTarget->keysOffset);
                    pPane->SetSRTElement(pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
                }
            }

            void AnimateVisibility(Pane* pPane, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    const res::StepKey* keys = detail::ConvertOffsToPtr< res::StepKey >(pAnimTarget, pAnimTarget->keysOffset);
                    pPane->SetVisible(0 != GetStepCurveValue(frame, keys, pAnimTarget->keyNum));
                }
            }

            void AnimateVertexColor(Pane* pPane, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    const res::HermiteKey* keys = detail::ConvertOffsToPtr< res::HermiteKey >(pAnimTarget, pAnimTarget->keysOffset);
                    f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
                    value += 0.5f;
                    u8 u8Val;
                    OSf32tou8(&value, &u8Val);
                    pPane->SetColorElement(pAnimTarget->target, u8Val);
                }
            }

            void AnimateMaterialColor(Material* pMaterial, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                using namespace nw4r;

                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    const res::HermiteKey* keys = detail::ConvertOffsToPtr< res::HermiteKey >(pAnimTarget, pAnimTarget->keysOffset);
                    f32 value = GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum);
                    value += 0.5f;
                    s16 s16Val;
                    OSf32tos16(&value, &s16Val);

                    s16Val = ut::Min(ut::Max(s16Val, s16(-1024)), s16(1023));
                    pMaterial->SetColorElement(pAnimTarget->target, s16Val);
                }
            }

            void AnimateTextureSRT(Material* pMaterial, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    if (pAnimTarget->id < pMaterial->GetTexSRTCap()) {
                        const res::HermiteKey* keys = detail::ConvertOffsToPtr< res::HermiteKey >(pAnimTarget, pAnimTarget->keysOffset);
                        pMaterial->SetTexSRTElement(pAnimTarget->id, pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
                    }
                }
            }

            void AnimateTexturePattern(Material* pMaterial, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame,
                                       void** tpls) {
                for (int j = 0; j < pAnimInfo->num; ++j) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[j]);

                    if (pAnimTarget->id < pMaterial->GetTextureNum()) {
                        const res::StepKey* keys = detail::ConvertOffsToPtr< res::StepKey >(pAnimTarget, pAnimTarget->keysOffset);
                        const u16 fileIdx = GetStepCurveValue(frame, keys, pAnimTarget->keyNum);

                        if (tpls[fileIdx]) {
                            pMaterial->GetTexturePtr(pAnimTarget->id)->ReplaceImage(static_cast< TPLPalettePtr >(tpls[fileIdx]));
                        }
                    }
                }
            }

            void AnimateIndTexSRT(Material* pMaterial, const res::AnimationInfo* pAnimInfo, const u32* animTargetOffsets, f32 frame) {
                for (int i = 0; i < pAnimInfo->num; ++i) {
                    const res::AnimationTarget* pAnimTarget = detail::ConvertOffsToPtr< res::AnimationTarget >(pAnimInfo, animTargetOffsets[i]);

                    if (pAnimTarget->id < pMaterial->GetIndTexSRTCap()) {
                        const res::HermiteKey* keys = detail::ConvertOffsToPtr< res::HermiteKey >(pAnimTarget, pAnimTarget->keysOffset);
                        pMaterial->SetIndTexSRTElement(pAnimTarget->id, pAnimTarget->target, GetHermiteCurveValue(frame, keys, pAnimTarget->keyNum));
                    }
                }
            }

            inline bool IsBindAnimation(Pane* pPane, AnimTransform* pAnimTrans) {
#if 1

                (void)pPane;
                (void)pAnimTrans;

#else

                if (pPane->FindAnimationLinkSelf(pAnimTrans)) {
                    return true;
                }

#endif

                return false;
            }

            inline bool IsBindAnimation(Material* pMaterial, AnimTransform* pAnimTrans) {
#if 1

                (void)pMaterial;
                (void)pAnimTrans;

#else

                if (pMaterial->FindAnimationLink(pAnimTrans)) {
                    return true;
                }

#endif

                return false;
            }

        }

        AnimTransform::AnimTransform() : mpRes(0), mFrame(0) {
        }

        AnimTransform::~AnimTransform() {
        }

        u16 AnimTransform::GetFrameSize() const {
            return mpRes->frameSize;
        }

        bool AnimTransform::IsLoopData() const {
            return mpRes->loop != 0;
        }

        AnimTransformBasic::AnimTransformBasic() : mpFileResAry(0), mAnimLinkAry(0), mAnimLinkNum(0) {
        }

        AnimTransformBasic::~AnimTransformBasic() {
            Layout::DeleteArray(mAnimLinkAry, mAnimLinkNum);
            Layout::DeletePrimArray(mpFileResAry);
        }

        void AnimTransformBasic::SetResource(const res::AnimationBlock* pRes, ResourceAccessor* pResAccessor) {
            SetResource(pRes, pResAccessor, pRes->animContNum);
        }

        void AnimTransformBasic::SetResource(const res::AnimationBlock* pBlock, ResourceAccessor* pResAccessor, u16 animNum) {
            mpRes = pBlock;
            mpFileResAry = nullptr;

            if (pBlock->fileNum > 0) {
                mpFileResAry = static_cast< void** >(Layout::AllocMemory(sizeof(void*) * pBlock->fileNum));
                if (mpFileResAry != nullptr) {
                    const u32* fileNameOffs = detail::ConvertOffsToPtr< u32 >(mpRes, sizeof(*mpRes));
                    for (int i = 0; i < mpRes->fileNum; i++) {
                        const char* const name = detail::GetStrTableStr(fileNameOffs, i);
                        mpFileResAry[i] = pResAccessor->GetResource('timg', name, 0);
                    }
                }
            }

            mAnimLinkAry = static_cast< AnimationLink* >(Layout::AllocMemory(sizeof(AnimationLink) * animNum));
            if (mAnimLinkAry != nullptr) {
                mAnimLinkNum = animNum;
                memset(mAnimLinkAry, 0, animNum * sizeof(AnimationLink));

                for (u16 i = 0; i < animNum; i++) {
                    new (&mAnimLinkAry[i]) AnimationLink();
                }
            }
        }

        void AnimTransformBasic::Bind(Pane* pPane, bool bRecursive) {
            AnimationLink* pCrAnimLink = 0;

            const u32* const animContOffsets = detail::ConvertOffsToPtr< u32 >(mpRes, mpRes->animContOffsetsOffset);
            for (u16 i = 0; i < mpRes->animContNum; ++i) {
                const res::AnimationContent& animCont = *detail::ConvertOffsToPtr< res::AnimationContent >(mpRes, animContOffsets[i]);
                if (animCont.type == ANIMCONTENTTYPE_PANE) {
                    if (Pane* const pFindPane = pPane->FindPaneByName(animCont.name, bRecursive)) {
                        if (!IsBindAnimation(pFindPane, this)) {
                            pCrAnimLink = Bind(pFindPane, pCrAnimLink, i);
                            if (!pCrAnimLink) {
                                break;
                            }
                        }
                    }
                } else {
                    if (Material* const pFindMat = pPane->FindMaterialByName(animCont.name, bRecursive)) {
                        if (!IsBindAnimation(pFindMat, this)) {
                            pCrAnimLink = Bind(pFindMat, pCrAnimLink, i);
                            if (!pCrAnimLink) {
                                break;
                            }
                        }
                    }
                }
            }
        }

        void AnimTransformBasic::Bind(Material* pMaterial) {
            AnimationLink* pCrAnimLink = 0;

            const u32* const animContOffsets = detail::ConvertOffsToPtr< u32 >(mpRes, mpRes->animContOffsetsOffset);
            for (u16 i = 0; i < mpRes->animContNum; ++i) {
                const res::AnimationContent& animCont = *detail::ConvertOffsToPtr< res::AnimationContent >(mpRes, animContOffsets[i]);
                if (animCont.type == ANIMCONTENTTYPE_MATERIAL) {
                    if (detail::EqualsMaterialName(pMaterial->GetName(), animCont.name)) {
                        if (!IsBindAnimation(pMaterial, this)) {
                            pCrAnimLink = Bind(pMaterial, pCrAnimLink, i);
                            if (!pCrAnimLink) {
                                break;
                            }
                        }
                    }
                }
            }
        }

        template < typename T >
        AnimationLink* AnimTransformBasic::Bind(T* pTarget, AnimationLink* pAnimLink, u16 idx) {
            pAnimLink = FindUnbindLink(pAnimLink);
            if (!pAnimLink) {
                return 0;
            }

            pAnimLink->Set(this, idx);
            pTarget->AddAnimationLink(pAnimLink);

            return ++pAnimLink;
        }

        void AnimTransformBasic::Animate(u32 idx, Pane* pPane) {
            u32 animContOffsets = detail::ConvertOffsToPtr< u32 >(mpRes, mpRes->animContOffsetsOffset)[idx];
            const res::AnimationContent* pAnimCont = detail::ConvertOffsToPtr< res::AnimationContent >(mpRes, animContOffsets);

            const u32* animInfoOffsets = detail::ConvertOffsToPtr< u32 >(pAnimCont, sizeof(*pAnimCont));
            for (int i = 0; i < pAnimCont->num; ++i) {
                const res::AnimationInfo* pAnimInfo = detail::ConvertOffsToPtr< res::AnimationInfo >(pAnimCont, animInfoOffsets[i]);
                const u32* animTargetOffsets = detail::ConvertOffsToPtr< u32 >(pAnimInfo, sizeof(*pAnimInfo));

                switch (pAnimInfo->kind) {
                case res::ANIMATIONTYPE_RLPA:
                    AnimatePaneSRT(pPane, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                case res::ANIMATIONTYPE_RLVI:
                    AnimateVisibility(pPane, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                case res::ANIMATIONTYPE_RLVC:
                    AnimateVertexColor(pPane, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                }
            }
        }

        void AnimTransformBasic::Animate(u32 idx, Material* pMaterial) {
            u32 animContOffsets = detail::ConvertOffsToPtr< u32 >(mpRes, mpRes->animContOffsetsOffset)[idx];
            const res::AnimationContent* pAnimCont = detail::ConvertOffsToPtr< res::AnimationContent >(mpRes, animContOffsets);

            const u32* animInfoOffsets = detail::ConvertOffsToPtr< u32 >(pAnimCont, sizeof(*pAnimCont));
            for (int i = 0; i < pAnimCont->num; ++i) {
                const res::AnimationInfo* pAnimInfo = detail::ConvertOffsToPtr< res::AnimationInfo >(pAnimCont, animInfoOffsets[i]);
                const u32* animTargetOffsets = detail::ConvertOffsToPtr< u32 >(pAnimInfo, sizeof(*pAnimInfo));

                switch (pAnimInfo->kind) {
                case res::ANIMATIONTYPE_RLMC:
                    AnimateMaterialColor(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                case res::ANIMATIONTYPE_RLTS:
                    AnimateTextureSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                case res::ANIMATIONTYPE_RLTP:
                    if (mpFileResAry) {
                        AnimateTexturePattern(pMaterial, pAnimInfo, animTargetOffsets, mFrame, mpFileResAry);
                    }
                    break;
                case res::ANIMATIONTYPE_RLIM:
                    AnimateIndTexSRT(pMaterial, pAnimInfo, animTargetOffsets, mFrame);
                    break;
                }
            }
        }

        AnimationLink* AnimTransformBasic::FindUnbindLink(AnimationLink* pLink) const {
            if (pLink == 0) {
                pLink = mAnimLinkAry;
            }

            while (pLink < mAnimLinkAry + mAnimLinkNum) {
                if (pLink->GetAnimTransform() == 0) {
                    return pLink;
                }

                ++pLink;
            }

            return 0;
        }

        AnimResource::AnimResource() {
            Init();
        }

        void AnimResource::Set(const void* anmResBuf) {
            Init();

            const res::BinaryFileHeader* const pFileHeader = static_cast< const res::BinaryFileHeader* >(anmResBuf);

            if (!detail::TestFileHeader(*pFileHeader, res::FILESIGNATURE_RLAN)) {
                return;
            }

            if (!detail::TestFileVersion(*pFileHeader)) {
                return;
            }

            mpFileHeader = pFileHeader;

            const res::DataBlockHeader* pDataBlockHead = detail::ConvertOffsToPtr< res::DataBlockHeader >(mpFileHeader, mpFileHeader->headerSize);
            for (int i = 0; i < mpFileHeader->dataBlocks; ++i) {
                switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
                case res::DATABLOCKKIND_PANEANIMTAG:
                    mpTagBlock = reinterpret_cast< const res::AnimationTagBlock* >(pDataBlockHead);
                    break;

                case res::DATABLOCKKIND_PANEANIMINFO:
                    mpResBlock = reinterpret_cast< const res::AnimationBlock* >(pDataBlockHead);
                    break;
                }

                pDataBlockHead = detail::ConvertOffsToPtr< res::DataBlockHeader >(pDataBlockHead, pDataBlockHead->size);
            }
        }

        void AnimResource::Init() {
            mpFileHeader = 0;
            mpResBlock = 0;
            mpTagBlock = 0;
        }

        namespace detail {

            AnimationLink* FindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans) {
                for (AnimationList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); ++it) {
                    if (pAnimTrans == it->GetAnimTransform()) {
                        return &(*it);
                    }
                }

                return 0;
            }

            AnimationLink* FindAnimationLink(AnimationList* pAnimList, const AnimResource& animRes) {
                for (AnimationList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter(); ++it) {
                    if (animRes.GetResourceBlock() == it->GetAnimTransform()->GetAnimResource()) {
                        return &(*it);
                    }
                }

                return 0;
            }

            void UnbindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans) {
                for (AnimationList::Iterator it = pAnimList->GetBeginIter(); it != pAnimList->GetEndIter();) {
                    AnimationList::Iterator currIt = it++;
                    if (!pAnimTrans || currIt->GetAnimTransform() == pAnimTrans) {
                        pAnimList->Erase(currIt);
                        currIt->Reset();
                    }
                }
            }

        }

    }
}
