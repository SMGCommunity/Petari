#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_animation.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/math/types.h"
#include <cstring>

namespace nw4r {
    namespace lyt {

        namespace detail {
            PaneBase::PaneBase() { 

            }

            inline PaneBase::~PaneBase() {

            }
        };

        NW4R_UT_RUNTIME_TYPEINFO_ROOT_DEFINITION(Pane);

        // not quite there yet
        Pane::~Pane() {
            PaneList::Iterator it = mChildList.GetBeginIter();
    
            while (it != mChildList.GetBeginIter()) {
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

        void Pane::SetName(const char *name) {
            strncpy(mName, name, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';
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

        const ut::Color Pane::GetVtxColor(u32) const {
            return ut::Color();
        }

        void Pane::SetVtxColor(u32 , ut::Color) {

        }

        u8 Pane::GetColorElement(u32 idx) const {
            switch (idx) {
                case 16:
                    return mAlpha;
                default:
                    return GetVtxColorElement(idx);
            }
        }

        void Pane::SetColorElement(u32 idx, u8 val) {
            switch (idx) {
                case 16:
                    mAlpha = val;
                    break;
                default:
                    SetVtxColorElement(idx, val);
            }
        }

        // Pane::GetVtxColorElement

        void Pane::SetVtxColorElement(u32, u8) {

        }

        Pane* Pane::FindPaneByName(const char* name, bool recursive) {
            if (detail::EqualsResName(mName, name)) {
                return this;
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    if (Pane* pane = it->FindPaneByName(name, true)) {
                        return pane;
                    }
                }
            }

            return nullptr;
        }
        
        Material* Pane::FindMaterialByName(const char* name, bool recursive) {
            if (mpMaterial != nullptr) {
                if (detail::EqualsMaterialName(mpMaterial->GetName(), name)) {
                    return mpMaterial;
                }
            }

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    if (Material* mat = it->FindMaterialByName(name, true)) {
                        return mat;
                    }
                }
            }

            return nullptr;
        }

        // nw4r::lyt::Pane::CalculateMtx

        void Pane::Draw(const DrawInfo &rInfo) {
            if (!IsVisible()) {
                return;
            }

            DrawSelf(rInfo);

            for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                it->Draw(rInfo);
            }
        }

        void Pane::DrawSelf(const DrawInfo &) {

        }

        void Pane::Animate(u32 opt) {
            AnimateSelf(opt);

            if (IsVisible() || opt & 1) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->Animate(opt);
                }
            }
        }

        void Pane::AnimateSelf(u32 opt) {
            for (AnimationList::Iterator it = mAnimList.GetBeginIter(); it != mAnimList.GetEndIter(); ++it) {
                if (it->IsEnable()) {
                    AnimTransform* trans = it->GetAnimTransform();
                    trans->Animate(it->GetIndex(), this);
                }
            }

            if (IsVisible() || opt & 1) {
                if (mpMaterial != nullptr) {
                    mpMaterial->Animate();
                }
            }
        }

        void Pane::BindAnimation(AnimTransform *pTrans, bool recursive) {
            pTrans->Bind(this, recursive); 
        }

        void Pane::UnbindAnimation(AnimTransform *pTrans, bool recursive) {
            UnbindAnimationSelf(pTrans);

            if (recursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->UnbindAnimation(pTrans, recursive);
                }
            }
        }

        void Pane::UnbindAllAnimation(bool recursive) {
            UnbindAnimation(nullptr, recursive);
        }

        void Pane::UnbindAnimationSelf(AnimTransform *pTrans) {
            if (mpMaterial != nullptr) {
                mpMaterial->UnbindAnimation(pTrans);
            }

            detail::UnbindAnimationLink(&mAnimList, pTrans);
        }

        void Pane::AddAnimationLink(AnimationLink *pLink) {
            mAnimList.PushBack(pLink);
        }

        AnimationLink* Pane::FindAnimationLinkSelf(AnimTransform *pTrans) {
            return detail::FindAnimationLink(&mAnimList, pTrans);
        }

        void Pane::SetAnimationEnable(AnimTransform *pTrans, bool enable, bool rescursive) {
            AnimationLink* link = FindAnimationLinkSelf(pTrans);
            
            if (link != nullptr) {
                link->SetEnable(enable);
            }

            if (mpMaterial != nullptr) {
                mpMaterial->SetAnimationEnable(pTrans, enable);
            }

            if (rescursive) {
                for (PaneList::Iterator it = mChildList.GetBeginIter(); it != mChildList.GetEndIter(); ++it) {
                    it->SetAnimationEnable(pTrans, enable, rescursive);
                }
            }
        }

        /*
        void Pane::LoadMtx(const DrawInfo &rInfo) { 
            math::MTX34 mtx;
            MtxPtr mtxPtr = nullptr;

            if (rInfo.IsMultipleViewMtxOnDraw()) {
                math::MTX34Mult(&mtx, &rInfo.GetViewMtx(), &mGlbMtx);
                mtxPtr = mtx;
            } 
            else {
                mtxPtr = mGlbMtx;
            }

            GXLoadPosMtxImm(mtxPtr, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
        }
        */

       const math::VEC2 Pane::GetVtxPos() const {
            math::VEC2 basePt(0, 0);

            switch (mBasePosition % 3) {
                case 0:
                default:
                    basePt.x = 0;
                    break;
                case 1:
                    basePt.x = -mSize.width / 2;
                    break;
                case 2:
                    basePt.x = -mSize.width;
                    break;
            }

            switch (mBasePosition / 3) {
                case 0:
                default:
                    basePt.y = 0;
                    break;
                case 1:
                    basePt.y = -mSize.height / 2;
                    break;
                case 2:
                    basePt.y = -mSize.height;
                    break;
            }

            return basePt;
       }

       Material* Pane::GetMaterial() const {
        return mpMaterial;
       }
    };
};