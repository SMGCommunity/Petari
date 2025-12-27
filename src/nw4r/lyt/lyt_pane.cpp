#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/common.h"
#include "nw4r/lyt/drawInfo.h"
#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/material.h"
#include "nw4r/lyt/pane.h"
#include "nw4r/math/triangular.h"
#include <cstdio>

namespace nw4r {
    void ReverseYAxis(math::MTX34* pMtx) {
        pMtx->m[0][1] = -pMtx->m[0][1];
        pMtx->m[1][1] = -pMtx->m[1][1];
        pMtx->m[2][1] = -pMtx->m[2][1];
    }
}  // namespace nw4r

namespace nw4r {
    namespace lyt {
        NW4R_UT_RUNTIME_TYPEINFO_ROOT_DEFINITION(Pane);

        namespace detail {
            PaneBase::PaneBase() {}

            PaneBase::~PaneBase() {}
        }  // namespace detail

        Pane::Pane(const res::Pane* pRes) {
            Init();

            mBasePosition = pRes->basePosition;

            SetName(pRes->name);
            SetUserData(pRes->userData);

            mTranslate = pRes->translate;
            mRotate = pRes->rotate;
            mScale = pRes->scale;
            mSize = pRes->size;

            mAlpha = pRes->alpha;
            mGlbAlpha = mAlpha;
            mFlag = pRes->flag;
        }

        Pane::~Pane() {
            for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter();) {
                PaneList::Iterator currIt = it++;
                mChildList.Erase(currIt);
                if (!currIt->IsUserAllocated()) {
                    Layout::DeleteObj(&(*currIt));
                }
            }

            UnbindAnimationSelf(0);

            if (mpMaterial && !mpMaterial->IsUserAllocated()) {
                Layout::DeleteObj(mpMaterial);
            }
        }

        void Pane::Init() {
            mpParent = NULL;
            mpMaterial = NULL;
            mbUserAllocated = false;
        }

        void Pane::SetName(const char* pName) {
            strncpy(mName, pName, 0x10);
            mName[0x10] = '\0';
        }

        void Pane::SetUserData(const char* pUserData) {
            strncpy(mUserData, pUserData, 8);
            mUserData[8] = '\0';
        }

        void Pane::AppendChild(Pane* pChild) {
            InsertChild(mChildList.GetEndIter(), pChild);
        }

        void Pane::InsertChild(PaneList::Iterator next, Pane* pChild) {
            mChildList.Insert(next, pChild);
            pChild->mpParent = this;
        }

        void Pane::RemoveChild(Pane* pChild) {
            mChildList.Erase(pChild);
            pChild->mpParent = nullptr;
        }

        const ut::Color Pane::GetVtxColor(u32 idx) const {
            return -1;
        }

        void Pane::SetVtxColor(u32 idx, ut::Color color) {}

        u8 Pane::GetColorElement(u32 idx) const {
            switch (idx) {
            case 0x10: {
                return mAlpha;
            }

            default: {
                return GetVtxColorElement(idx);
            }
            }
        }

        void Pane::SetColorElement(u32 idx, u8 value) {
            switch (idx) {
            case 0x10: {
                mAlpha = value;
                break;
            }

            default: {
                SetVtxColorElement(idx, value);
                break;
            }
            }
        }

        u8 Pane::GetVtxColorElement(u32 idx) const {
            return 0xFF;
        }

        void Pane::SetVtxColorElement(u32 idx, u8 value) {}

        Pane* Pane::FindPaneByName(const char* pName, bool recursive) {
            if (detail::EqualsResName(mName, pName)) {
                return this;
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    if (Pane* pPane = it->FindPaneByName(pName, recursive)) {
                        return pPane;
                    }
                }
            }

            return nullptr;
        }

        Material* Pane::FindMaterialByName(const char* pName, bool recursive) {
            if (mpMaterial != NULL && detail::EqualsMaterialName(mpMaterial->GetName(), pName)) {
                return mpMaterial;
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    if (Material* pMat = it->FindMaterialByName(pName, recursive)) {
                        return pMat;
                    }
                }
            }

            return nullptr;
        }

        void Pane::CalculateMtx(const DrawInfo& rInfo) {
            if (!IsVisible() && !rInfo.IsInvisiblePaneCalculateMtx()) {
                return;
            }

            math::MTX34 mtx1, mtx2;
            math::MTX34 rotateMtx;

            math::VEC2 scale = mScale;
            if (rInfo.IsLocationAdjust() && IsLocationAdjust()) {
                scale.x *= rInfo.GetLocationAdjustScale().x;
                scale.y *= rInfo.GetLocationAdjustScale().y;
            }

            PSMTXScale(mtx2, scale.x, scale.y, 1.0f);

            PSMTXRotRad(rotateMtx, 'x', NW4R_MATH_DEG_TO_RAD(mRotate.x));
            PSMTXConcat(rotateMtx, mtx2, mtx1);

            PSMTXRotRad(rotateMtx, 'y', NW4R_MATH_DEG_TO_RAD(mRotate.y));
            PSMTXConcat(rotateMtx, mtx1, mtx2);

            PSMTXRotRad(rotateMtx, 'z', NW4R_MATH_DEG_TO_RAD(mRotate.z));
            PSMTXConcat(rotateMtx, mtx2, mtx1);

            PSMTXTransApply(mtx1, mMtx, mTranslate.x, mTranslate.y, mTranslate.z);

            if (mpParent != NULL) {
                math::MTX34Mult(&mGlbMtx, &mpParent->mGlbMtx, &mMtx);
            } else if (rInfo.IsMultipleViewMtxOnDraw()) {
                mGlbMtx = mMtx;
            } else {
                math::MTX34Mult(&mGlbMtx, &rInfo.GetViewMtx(), &mMtx);
            }

            if (rInfo.IsInfluencedAlpha() && mpParent != NULL) {
                mGlbAlpha = static_cast< u8 >(mAlpha * rInfo.GetGlobalAlpha());
            } else {
                mGlbAlpha = mAlpha;
            }

            f32 glbAlpha = rInfo.GetGlobalAlpha();
            bool influenced = rInfo.IsInfluencedAlpha();
            bool modifyInfo = IsInfluencedAlpha() && mAlpha != 255;

            if (modifyInfo) {
                DrawInfo& rMtInfo = const_cast< DrawInfo& >(rInfo);
                rMtInfo.SetGlobalAlpha(glbAlpha * mAlpha * (1.0f / 255.0f));
                rMtInfo.SetInfluencedAlpha(true);
            }

            CalculateMtxChild(rInfo);

            if (modifyInfo) {
                DrawInfo& rMtInfo = const_cast< DrawInfo& >(rInfo);
                rMtInfo.SetGlobalAlpha(glbAlpha);
                rMtInfo.SetInfluencedAlpha(influenced);
            }
        }

        void Pane::CalculateMtxChild(const DrawInfo& rInfo) {
            for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                it->CalculateMtx(rInfo);
            }
        }

        void Pane::Draw(const DrawInfo& rInfo) {
            if (!IsVisible()) {
                return;
            }

            DrawSelf(rInfo);
            for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                it->Draw(rInfo);
            }
        }

        void Pane::DrawSelf(const DrawInfo& rInfo) {}

        void Pane::Animate(u32 option) {
            AnimateSelf(option);

            if (IsVisible() || !(option & 1)) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->Animate(option);
                }
            }
        }

        void Pane::AnimateSelf(u32 option) {
            for (AnimationList::Iterator it = mAnimList.GetBeginIter(); it != mAnimList.GetEndIter(); ++it) {
                if (it->IsEnable()) {
                    AnimTransform* animTrans = it->GetAnimTransform();
                    animTrans->Animate(it->GetIndex(), this);
                }
            }

            if (IsVisible() || !(option & 1)) {
                if (mpMaterial != NULL) {
                    mpMaterial->Animate();
                }
            }
        }

        void Pane::BindAnimation(AnimTransform* pAnimTrans, bool recursive) {
            pAnimTrans->Bind(this, recursive);
        }

        void Pane::UnbindAnimation(AnimTransform* pAnimTrans, bool recursive) {
            UnbindAnimationSelf(pAnimTrans);

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->UnbindAnimation(pAnimTrans, recursive);
                }
            }
        }

        void Pane::UnbindAllAnimation(bool recursive) {
            UnbindAnimation(nullptr, recursive);
        }

        void Pane::UnbindAnimationSelf(AnimTransform* pAnimTrans) {
            if (mpMaterial != NULL) {
                mpMaterial->UnbindAnimation(pAnimTrans);
            }

            detail::UnbindAnimationLink(&mAnimList, pAnimTrans);
        }

        void Pane::AddAnimationLink(AnimationLink* pAnimLink) {
            mAnimList.PushBack(pAnimLink);
        }

        AnimationLink* Pane::FindAnimationLinkSelf(AnimTransform* pAnimTrans) {
            return detail::FindAnimationLink(&mAnimList, pAnimTrans);
        }

        void Pane::SetAnimationEnable(AnimTransform* pAnimTrans, bool enable, bool recursive) {
            AnimationLink* pAnimLink = FindAnimationLinkSelf(pAnimTrans);

            if (pAnimLink != NULL) {
                pAnimLink->SetEnable(enable);
            }

            if (mpMaterial != NULL) {
                mpMaterial->SetAnimationEnable(pAnimTrans, enable);
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->SetAnimationEnable(pAnimTrans, enable, recursive);
                }
            }
        }

        void Pane::LoadMtx(const DrawInfo& rInfo) {
            math::MTX34 mtx;
            math::MTX34* pMtx = NULL;

            if (rInfo.IsMultipleViewMtxOnDraw()) {
                math::MTX34Mult(&mtx, &rInfo.GetViewMtx(), &mGlbMtx);

                if (rInfo.IsYAxisUp()) {
                    ReverseYAxis(&mtx);
                }

                pMtx = &mtx;
            } else if (rInfo.IsYAxisUp()) {
                math::MTX34Copy(&mtx, &mGlbMtx);
                pMtx = &mtx;

                ReverseYAxis(&mtx);
            } else {
                pMtx = &mGlbMtx;
            }

            GXLoadPosMtxImm(*pMtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
        }

        math::VEC2 Pane::GetVtxPos() const {
            math::VEC2 base(0.0f, 0.0f);

            switch (mBasePosition % 3) {
            default:
            case 0: {
                base.x = 0.0f;
                break;
            }

            case 1: {
                base.x = -mSize.width / 2;
                break;
            }

            case 2: {
                base.x = -mSize.width;
                break;
            }
            }

            switch (mBasePosition / 3) {
            default:
            case 0: {
                base.y = 0.0f;
                break;
            }

            case 1: {
                base.y = -mSize.height / 2;
                break;
            }

            case 2: {
                base.y = -mSize.height;
                break;
            }
            }

            return base;
        }

        Material* Pane::GetMaterial() const {
            return mpMaterial;
        }

    };  // namespace lyt
};  // namespace nw4r
