#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/common.h"
#include "nw4r/lyt/group.h"
#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/pane.h"
#include "nw4r/lyt/resources.h"
#include "nw4r/lyt/util.h"
#include "revolution/mem/allocator.h"
#include "nw4r/ut/Rect.h"

namespace nw4r {
    namespace lyt {
        namespace {
            bool IsIncludeAnimationGroupRef(GroupContainer *pGroupContainer, const AnimationGroupRef *const groupRefs, u16 bindGroupNum, bool bDescendingBind, Pane *pTargetPane) {
                for (u16 grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
                    Group* const pGroup = pGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
                    PaneLinkList& paneList = pGroup->GetPaneList();
                    for (PaneLinkList::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); ++it) {
                        if (it->mTarget == pTargetPane) {
                            return true;
                        }

                        if (bDescendingBind) {
                            for (Pane* pParentPane = pTargetPane->GetParent(); pParentPane; pParentPane = pParentPane->GetParent()) {
                                if (it->mTarget == pParentPane) {
                                    return true;
                                }
                            }
                        }
                    }
                }

                return false;
            }
        };

        MEMAllocator* Layout::mspAllocator = nullptr;

        Layout::Layout() : mpRootPane(nullptr), mpGroupContainer(nullptr), mLayoutSize(0.0f, 0.0f) {

        }

        Layout::~Layout() {
            DeleteObj(mpGroupContainer);

            if (mpRootPane != nullptr && !mpRootPane->IsUserAllocated()) {
                DeleteObj(mpRootPane);
            }

            for (AnimTransformList::Iterator it = mAnimTransList.GetBeginIter(); it != mAnimTransList.GetEndIter();) {
                AnimTransformList::Iterator currIt = it++;
                mAnimTransList.Erase(currIt);
                DeleteObj(&(*currIt));
            }
        }

        bool Layout::Build(const void *lytResBuf, ResourceAccessor *pResAcsr) {
            const res::BinaryFileHeader *const pFileHead = static_cast<const res::BinaryFileHeader*>(lytResBuf);
            if (!detail::TestFileHeader(*pFileHead, 'RLYT')) {
                return false;
            }

            if (!detail::TestFileVersion(*pFileHead)) {
                return false;
            }

            ResBlockSet resBlockSet = { 0 };
            resBlockSet.pResAccessor = pResAcsr;
            Pane* pParentPane = nullptr;
            Pane* pLastPane = nullptr;
            bool bReadRootGroup = false;
            int groupNestLevel = 0;

            const void* dataPtr = static_cast<const u8*>(lytResBuf) + pFileHead->headerSize;
            for (int i = 0; i < pFileHead->dataBlocks; ++i) {
                const res::DataBlockHeader* pDataBlockHead = static_cast<const res::DataBlockHeader*>(dataPtr);
                switch (detail::GetSignatureInt(pDataBlockHead->kind)) {
                    case 'lyt1':
                    {
                        const res::Layout* pResLyt = static_cast<const res::Layout*>(dataPtr);
                        mLayoutSize = pResLyt->layoutSize;
                        break;
                    }
                    case 'txl1':
                        resBlockSet.pTextureList = static_cast<const res::TextureList*>(dataPtr);
                        break;
                    case 'fnl1':
                        resBlockSet.pFontList = static_cast<const res::FontList*>(dataPtr);
                        break;
                    case 'mat1':
                        resBlockSet.pMaterialList = static_cast<const res::MaterialList*>(dataPtr);
                        break;

                    case 'pan1':
                    case 'pic1':
                    case 'txt1':
                    case 'wnd1':
                    case 'bnd1':
                    {
                        Pane* pPane = BuildPaneObj(detail::GetSignatureInt(pDataBlockHead->kind), dataPtr, resBlockSet);

                        if (pPane != nullptr) {
                            if (mpRootPane == nullptr) {
                                mpRootPane = pPane;
                            }

                            if (pParentPane != nullptr) {
                                pParentPane->AppendChild(pPane);
                            }

                            pLastPane = pPane;
                        }
                    }
                    break;

                    case 'usd1':
                        pLastPane->SetExtUserDataList(reinterpret_cast<const res::ExtUserDataList*>(pDataBlockHead));
                        break;
                    case 'pas1':
                        pParentPane = pLastPane;
                        break;
                    case 'pae1':
                        pLastPane = pParentPane;
                        pParentPane = pLastPane->GetParent();
                        break;
                    case 'grp1':
                        {
                            if (!bReadRootGroup) {
                                bReadRootGroup = true;
                                mpGroupContainer = NewObj<GroupContainer>();
                            }
                            else {
                                if (mpGroupContainer != nullptr && groupNestLevel == 1) {
                                    if (Group* pGroup = NewObj<Group>(reinterpret_cast<const res::Group*>(pDataBlockHead), mpRootPane)) {
                                        mpGroupContainer->AppendGroup(pGroup);
                                    }
                                }
                            }
                            break;
                        }
                    case 'grs1':
                        groupNestLevel++;
                        break;
                    case 'gre1':
                        groupNestLevel--;
                        break;
                    default:
                        break;
                }

                dataPtr = static_cast<const u8*>(dataPtr) + pDataBlockHead->size;
            }

            return true;
        }

        AnimTransform* Layout::CreateAnimTransform() {
            AnimTransformBasic *const pAnimTrans = NewObj<AnimTransformBasic>();
            if (pAnimTrans != nullptr) {
                mAnimTransList.PushBack(pAnimTrans);
            }
            return pAnimTrans;
        }

        AnimTransform* Layout::CreateAnimTransform(const void *animResBuf, ResourceAccessor *pResAcsr) {
            return CreateAnimTransform(AnimResource(animResBuf), pResAcsr);
        }

        AnimTransform* Layout::CreateAnimTransform(const AnimResource &animRes, ResourceAccessor *pResAcsr) {
            const res::AnimationBlock* const pAnimBlock = animRes.GetResourceBlock();

            if (pAnimBlock == nullptr) {
                return nullptr;
            }

            AnimTransform* const pAnimTrans = CreateAnimTransform();
            if (pAnimTrans != nullptr) {
                pAnimTrans->SetResource(pAnimBlock, pResAcsr);
            }
            return pAnimTrans;
        }

        void Layout::BindAnimation(AnimTransform *pAnimTrans) {
            if (mpRootPane != nullptr) {
                mpRootPane->BindAnimation(pAnimTrans, true, false);
            }
        }

        void Layout::UnbindAnimation(AnimTransform *pAnimTrans) {
            if (mpRootPane != nullptr) {
                mpRootPane->UnbindAnimation(pAnimTrans, true);
            }
        }

        void Layout::UnbindAllAnimation() {
            UnbindAnimation(0);
        }

        bool Layout::BindAnimationAuto(const AnimResource &animRes, ResourceAccessor *pResAcsr) {
            if (mpRootPane == nullptr) {
                return false;
            }

            if (!animRes.GetResourceBlock()) {
                return false;
            }

            AnimTransform *const pAnimTrans = CreateAnimTransform();
            const u16 bindGroupNum = animRes.GetGroupNum();
            u16 animNum = 0;

            if (bindGroupNum == 0) {
                animNum = animRes.GetResourceBlock()->animContNum;
                pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);
                const bool bRecursive = true;
                mpRootPane->BindAnimation(pAnimTrans, bRecursive, true);
            }
            else {
                const AnimationGroupRef* const groupRefs = animRes.GetGroupArray();
                for (int grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
                    Group *const pGroup = mpGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
                    animNum += animRes.CalcAnimationNum(pGroup, animRes.IsDescendingBind());
                }

                pAnimTrans->SetResource(animRes.GetResourceBlock(), pResAcsr, animNum);

                for (int grpIdx = 0; grpIdx < bindGroupNum; ++grpIdx) {
                    Group* const pGroup = mpGroupContainer->FindGroupByName(groupRefs[grpIdx].GetName());
                    nw4r::lyt::BindAnimation(pGroup, pAnimTrans, animRes.IsDescendingBind(), true);
                }
            }

            const u16 animShareInfoNum = animRes.GetAnimationShareInfoNum();
            if (animShareInfoNum > 0) {
                const AnimationShareInfo* const animShareInfoAry = animRes.GetAnimationShareInfoArray();

                for (int i = 0; i < animShareInfoNum; ++i) {
                    Pane* const pSrcPane = mpRootPane->FindPaneByName(animShareInfoAry[i].GetSrcPaneName(), true);
                    detail::AnimPaneTree animPaneTree(pSrcPane, animRes);
                    if (!animPaneTree.IsEnabled()) {
                        continue;
                    }

                    Group* const pGroup = GetGroupContainer()->FindGroupByName(animShareInfoAry[i].GetTargetGroupName());
                    PaneLinkList& paneList = pGroup->GetPaneList();
                    u32 animIdx = 0;
                    for (PaneLinkList::Iterator it = paneList.GetBeginIter(); it != paneList.GetEndIter(); ++it, ++animIdx) {
                        if (it->mTarget != pSrcPane) {
                            if (bindGroupNum > 0) {
                                const bool bInclude = IsIncludeAnimationGroupRef(mpGroupContainer, animRes.GetGroupArray(), bindGroupNum, animRes.IsDescendingBind(), it->mTarget);

                                if (!bInclude) {
                                    continue;
                                }
                            }

                            animPaneTree.Bind(this, it->mTarget, pResAcsr);
                        }
                    }
                }
            }

            return true;
        }

        void Layout::SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable) {
            if (mpRootPane != nullptr) {
                mpRootPane->SetAnimationEnable(pAnimTrans, bEnable, true);
            }
        }

        void Layout::CalculateMtx(const DrawInfo &rInfo) {
            if (mpRootPane == nullptr) {
                return;
            }

            mpRootPane->CalculateMtx(rInfo);
        }

        void Layout::Draw(const DrawInfo &rInfo) {
            if (mpRootPane == nullptr) {
                return;
            }

            mpRootPane->Draw(rInfo);
        }

        void Layout::Animate(u32 opt) {
            if (mpRootPane == nullptr) {
                return;
            }

            mpRootPane->Animate(opt);
        }

        const ut::Rect Layout::GetLayoutRect() const {
            return ut::Rect(- mLayoutSize.width / 2, mLayoutSize.height / 2, mLayoutSize.width / 2, - mLayoutSize.height / 2);
        }

        // have to do more headers to decomp these two
        // nw4r::lyt::Layout::SetTagProcessor
        // nw4r::lyt::Layout::BuildPaneObj
    };
};
