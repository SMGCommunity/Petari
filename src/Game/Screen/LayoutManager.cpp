#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/LayoutGroupCtrl.hpp"
#include "Game/Screen/LayoutPaneCtrl.hpp"
#include "Game/System/Language.hpp"
#include "Game/System/LayoutHolder.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <cstdio>
#include <cstring>
#include <nw4r/lyt/group.h>
#include <nw4r/lyt/layout.h>
#include <nw4r/lyt/material.h>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/picture.h>
#include <nw4r/lyt/texMap.h>
#include <nw4r/lyt/textBox.h>
#include <nw4r/ut/Rect.h>
#include <nw4r/ut/RuntimeTypeInfo.h>
#include <revolution/mtx.h>
#include <revolution/types.h>

namespace {
    namespace Local {
        class BitFlagBase {
        public:
            virtual ~BitFlagBase() {
            }
            virtual void onBit(int) = 0;
            virtual void offBit(int) = 0;
            virtual bool isTrue(int) const = 0;
            virtual bool isAnythingTrue() const = 0;
        };

        template < int N >
        class BitFlag : public BitFlagBase {
        public:
            BitFlag() NO_INLINE {
                MR::zeroMemory(mBits, sizeof(mBits));
            }

            virtual bool isAnythingTrue() const;
            virtual void onBit(int);
            virtual void offBit(int);
            virtual bool isTrue(int) const;

            /* 0x04 */ u32 mBits[N / 32];
        };
    }  // namespace Local

    static const char* const cRemoveString[] = {
        "4x3",
        "16x9",
        "Replace",
    };
}  // namespace

namespace {
    namespace Local {
        inline void collectLanguagePanes(nw4r::lyt::Pane* pPane, BitFlagBase& rLanguagePanes, BitFlagBase& rCurrentLanguagePanes) {
            nw4r::lyt::PaneList& rPaneList = pPane->mChildList;
            s32 index = -1;
            for (nw4r::lyt::PaneList::Iterator it = rPaneList.GetBeginIter(); it != rPaneList.GetEndIter(); ++it) {
                index++;
                const char* pName = (*it).mName;
                u32 length = strlen(pName);
                if (length < 4) {
                    continue;
                }

                const char* pSuffix = pName + (length - 4);
                for (u32 i = 0; i < MR::getLanguageNum(); i++) {
                    if (strncmp(pSuffix, MR::getLanguagePrefixByIndex(i), 4) == 0) {
                        rLanguagePanes.onBit(index);
                        if (strncmp(pSuffix, MR::getCurrentLanguagePrefix(), 4) == 0) {
                            rCurrentLanguagePanes.onBit(index);
                        }

                        break;
                    }
                }
            }
        }

        inline void removeLanguagePanes(nw4r::lyt::Pane* pPane, const BitFlagBase& rLanguagePanes, const BitFlagBase& rCurrentLanguagePanes) {
            const char* pName;
            nw4r::lyt::PaneList& rChildren = pPane->mChildList;
            u32 length;
            s32 childIndex = 0;
            nw4r::lyt::PaneList::Iterator it = rChildren.GetBeginIter();
            if (rCurrentLanguagePanes.isAnythingTrue()) {
                while (it != rChildren.GetEndIter()) {
                    nw4r::lyt::PaneList::Iterator child = it;
                    it++;
                    if (rCurrentLanguagePanes.isTrue(childIndex)) {
                        pName = (*child).mName;
                        length = strlen(pName) - 4;
                        char name[17];
                        strncpy(name, pName, length);
                        name[length] = '\0';
                        (*child).SetName(name);
                    } else {
                        pPane->RemoveChild(&*child);
                    }

                    childIndex++;
                }
            } else {
                while (it != rChildren.GetEndIter()) {
                    nw4r::lyt::PaneList::Iterator child = it;
                    it++;
                    if (rLanguagePanes.isTrue(childIndex)) {
                        pPane->RemoveChild(&*child);
                    }

                    childIndex++;
                }
            }
        }
    }  // namespace Local
}  // namespace

LayoutManager::LayoutManager(const char* pLayoutName, bool useArchiveNamePrefix, u32 rootPaneAnimLayerNum, u32 textBoxBufferLength)
    : mLayoutHolder(), mLayout(), mAnimTransList(), mDrawInfo(), mIsScreenHidden(), _61(true), mIndDummyTexMap(), mPaneCount(), mPaneInfoList(),
      mGroupCtrlCount(), mGroupCtrlList(), mLayoutName() {
    if (useArchiveNamePrefix) {
        char fileNameWithoutExtension[0x60];
        char fileNameFromPrefix[0x80];
        MR::makeLayoutArchiveFileNameFromPrefix(fileNameFromPrefix, sizeof(fileNameFromPrefix), pLayoutName, true);
        MR::removeExtensionString(fileNameWithoutExtension, sizeof(fileNameWithoutExtension), fileNameFromPrefix);
        pLayoutName = MR::getBasename(fileNameWithoutExtension);
    }

    char archiveName[0x40];
    snprintf(archiveName, sizeof(archiveName), "%s.arc", pLayoutName);

    char fileName[0x40];
    MR::copyString(fileName, pLayoutName, sizeof(fileName));

    for (u32 i = 0; i < ARRAY_SIZE(::cRemoveString); i++) {
        char* pos = strstr(fileName, ::cRemoveString[i]);

        if (pos != nullptr) {
            pos[0] = '\0';
            break;
        }
    }

    u32 len = strlen(fileName) + 1;
    mLayoutName = new char[len];
    MR::copyString(mLayoutName, fileName, len);

    initArc(archiveName, pLayoutName);
    initPaneInfo();
    initGroupCtrlList();
    initDrawInfo();
    initTextBoxRecursive(mLayout->mpRootPane, nullptr, mLayoutName, textBoxBufferLength);
    replaceIndDummyTexture();

    if (rootPaneAnimLayerNum != 0) {
        createAndAddRootPaneCtrl(rootPaneAnimLayerNum);
    }

    mLayout->Animate(0);
    mLayout->CalculateMtx(mDrawInfo);
}

void LayoutManager::movement() {
    for (u32 i = 0; i < mPaneCount; i++) {
        if (mPaneInfoList[i].mPaneCtrl != nullptr) {
            mPaneInfoList[i].mPaneCtrl->movement();
        }
    }

    for (u32 i = 0; i < mGroupCtrlCount; i++) {
        if (mGroupCtrlList[i] != nullptr) {
            mGroupCtrlList[i]->movement();
        }
    }
}

void LayoutManager::calcAnim() {
    if (MR::isScreen16Per9()) {
        nw4r::math::VEC2 scale;
        scale.x = 0.75f;
        scale.y = 1.0f;
        mDrawInfo.SetLocationAdjustScale(scale);
    } else {
        nw4r::math::VEC2 scale;
        scale.x = 1.0f;
        scale.y = 1.0f;
        mDrawInfo.SetLocationAdjustScale(scale);
    }

    calcAnimWithoutLocationAdjust(mDrawInfo);
}

void LayoutManager::draw() const {
    if (mIsScreenHidden) {
        return;
    }

    MR::setupDrawForNW4RLayout(1.0f, true);
    mLayout->Draw(mDrawInfo);
}

void LayoutManager::addPaneCtrl(LayoutPaneCtrl* pPaneCtrl) {
    s32 index = getIndexOfPane(pPaneCtrl->mPane->mName);

    if (mPaneInfoList[index].mPaneCtrl == nullptr) {
        pPaneCtrl->mPaneIndex = index;
        mPaneInfoList[index].mPaneCtrl = pPaneCtrl;
    }
}

LayoutPaneCtrl* LayoutManager::createAndAddRootPaneCtrl(u32 animLayerNum) {
    s32 index = getIndexOfPane(mLayout->mpRootPane->mName);

    if (mPaneInfoList[index].mPaneCtrl != nullptr) {
        return mPaneInfoList[index].mPaneCtrl;
    }

    LayoutPaneCtrl* pPaneCtrl = new LayoutPaneCtrl(this, mLayout->mpRootPane->mName, animLayerNum);
    addPaneCtrl(pPaneCtrl);
    return pPaneCtrl;
}

LayoutPaneCtrl* LayoutManager::createAndAddPaneCtrl(const char* pName, u32 animLayerNum) {
    s32 index = getIndexOfPane(pName);

    if (mPaneInfoList[index].mPaneCtrl != nullptr) {
        return mPaneInfoList[index].mPaneCtrl;
    }

    LayoutPaneCtrl* pPaneCtrl = new LayoutPaneCtrl(this, pName, animLayerNum);
    addPaneCtrl(pPaneCtrl);
    return pPaneCtrl;
}

LayoutPaneCtrl* LayoutManager::getPaneCtrl(const char* pName) const {
    if (pName == nullptr) {
        return mPaneInfoList[0].mPaneCtrl;
    }

    s32 index = getIndexOfPane(pName);
    return mPaneInfoList[index].mPaneCtrl;
}

s32 LayoutManager::getIndexOfPane(const char* pName) const {
    u32 paneCount = mPaneCount;

    for (u32 i = 0; i < paneCount; i++) {
        if (strcmp(mPaneInfoList[i].mName, pName) == 0) {
            return i;
        }
    }

    return 0;
}

bool LayoutManager::isExistPaneCtrl(const char* pName) const {
    if (pName == nullptr) {
        return mPaneInfoList[0].mPaneCtrl != nullptr;
    }

    s32 index = getIndexOfPane(pName);
    return mPaneInfoList[index].mPaneCtrl != nullptr;
}

void LayoutManager::addGroupCtrl(LayoutGroupCtrl* pGroupCtrl) {
    s32 index = getIndexOfGroupCtrl(pGroupCtrl->mGroup->mName);
    mGroupCtrlList[index] = pGroupCtrl;

    for (u32 i = 0; i < pGroupCtrl->getPaneNum(); i++) {
        s32 paneIndex = getIndexOfPane(pGroupCtrl->getPane(i)->mName);
        LayoutGroupCtrlLink* pNext = mPaneInfoList[paneIndex].mGroupCtrlLink;
        LayoutGroupCtrlLink* pLink = new LayoutGroupCtrlLink;
        pLink->mGroupCtrl = pGroupCtrl;
        pLink->mNext = pNext;
        mPaneInfoList[paneIndex].mGroupCtrlLink = pLink;
    }
}

bool LayoutManager::isPointing(const nw4r::lyt::Pane* pPane, const TVec2f& rPos) const {
    Mtx inverse;
    PSMTXInverse(pPane->mGlbMtx, inverse);
    s32 horizontalPosition = static_cast< u8 >(pPane->mBasePosition % 3);
    s32 verticalPosition = static_cast< u8 >(pPane->mBasePosition / 3);
    TVec3f pos(0.0f, 0.0f, 0.0f);
    MR::convertScreenPosToLayoutPos(reinterpret_cast< TVec2f* >(&pos), rPos);
    PSMTXMultVec(inverse, &pos, &pos);

    switch (horizontalPosition) {
    case 0:
        if (pos.x < 0.0f || pPane->mSize.width < pos.x) {
            return false;
        }

        break;
    case 1:
        if (pos.x < -pPane->mSize.width / 2.0f || pPane->mSize.width / 2.0f < pos.x) {
            return false;
        }

        break;
    case 2:
        if (pos.x < -pPane->mSize.width || 0.0f < pos.x) {
            return false;
        }

        break;
    }

    switch (verticalPosition) {
    case 0:
        if (pos.y < -pPane->mSize.height || 0.0f < pos.y) {
            return false;
        }

        break;
    case 1:
        if (pos.y < -pPane->mSize.height / 2.0f || pPane->mSize.height / 2.0f < pos.y) {
            return false;
        }

        break;
    case 2:
        if (pos.y < 0.0f || pPane->mSize.height < pos.y) {
            return false;
        }

        break;
    }

    return true;
}

LayoutGroupCtrl* LayoutManager::createAndAddGroupCtrl(const char* pName, u32 animLayerNum) {
    LayoutGroupCtrl* pGroupCtrl = new LayoutGroupCtrl(this, pName, animLayerNum);
    addGroupCtrl(pGroupCtrl);
    return pGroupCtrl;
}

s32 LayoutManager::getIndexOfGroupCtrl(const char* pName) const {
    u32 index = 0;
    nw4r::lyt::GroupList& rGroupList = mLayout->mpGroupContainer->mGroupList;
    for (nw4r::lyt::GroupList::Iterator it = rGroupList.GetBeginIter(); it != rGroupList.GetEndIter(); it++) {
        if (strcmp(it->mName, pName) == 0) {
            return index;
        }

        index++;
    }

    return index;
}

void LayoutManager::createPaneMtxRef(const char* pName) {
    s32 index = 0;
    if (pName != nullptr) {
        index = getIndexOfPane(pName);
    }

    mPaneInfoList[index].mMtxRef = new f32[3][4];
}

MtxPtr LayoutManager::getPaneMtxRef(const char* pName) const {
    s32 index = 0;
    if (pName != nullptr) {
        index = getIndexOfPane(pName);
    }

    return mPaneInfoList[index].mMtxRef;
}

bool LayoutManager::isExistPaneMtxRef(const char* pName) const {
    s32 index = 0;
    if (pName != nullptr) {
        index = getIndexOfPane(pName);
    }

    return mPaneInfoList[index].mMtxRef != nullptr;
}

bool LayoutManager::isPointing(const char* pName, const TVec2f& rPos) const {
    return isPointing(findPaneByName(pName), rPos);
}

nw4r::lyt::AnimTransform* LayoutManager::getAnimTransform(const char* pName) const {
    char fileName[0x80];
    snprintf(fileName, sizeof(fileName), "%s.brlan", pName);
    u32 hash = MR::getHashCodeLower(fileName);
    for (u32 i = 0; i < mLayoutHolder->mAnimRes.mCount; i++) {
        if (mLayoutHolder->isAnimationHashEqual(hash, i)) {
            return mAnimTransList[i];
        }
    }

    return nullptr;
}

void LayoutManager::bindPaneCtrlAnim(LayoutPaneCtrl* pPaneCtrl, nw4r::lyt::AnimTransform* pAnimTransform) {
    u32 index = pPaneCtrl->mPaneIndex;
    mPaneInfoList[index].mPane->UnbindAnimationSelf(pAnimTransform);
    mPaneInfoList[index].mPane->BindAnimation(pAnimTransform, false);
    index++;

    while (index < pPaneCtrl->mPaneIndex + mPaneInfoList[pPaneCtrl->mPaneIndex].mChildCount) {
        bindPaneCtrlAnimSub(index, pAnimTransform);
    }
}

void LayoutManager::bindPaneCtrlAnimSub(u32& rIndex, nw4r::lyt::AnimTransform* pAnimTransform) {
    if (mPaneInfoList[rIndex].mPaneCtrl != nullptr) {
        rIndex += mPaneInfoList[rIndex].mChildCount;
        return;
    }

    mPaneInfoList[rIndex].mPane->UnbindAnimationSelf(pAnimTransform);
    mPaneInfoList[rIndex].mPane->BindAnimation(pAnimTransform, false);
    u32 startIndex = rIndex;
    rIndex++;

    while (rIndex < startIndex + mPaneInfoList[startIndex].mChildCount) {
        bindPaneCtrlAnimSub(rIndex, pAnimTransform);
    }
}

void LayoutManager::unbindPaneCtrlAnim(LayoutPaneCtrl* pPaneCtrl, nw4r::lyt::AnimTransform* pAnimTransform) {
    u32 index = pPaneCtrl->mPaneIndex;
    mPaneInfoList[index].mPane->UnbindAnimationSelf(pAnimTransform);
    index++;

    while (index < pPaneCtrl->mPaneIndex + mPaneInfoList[pPaneCtrl->mPaneIndex].mChildCount) {
        unbindPaneCtrlAnimSub(index, pAnimTransform);
    }
}

void LayoutManager::unbindPaneCtrlAnimSub(u32& rIndex, nw4r::lyt::AnimTransform* pAnimTransform) {
    if (mPaneInfoList[rIndex].mPaneCtrl != nullptr) {
        rIndex += mPaneInfoList[rIndex].mChildCount;
        return;
    }

    mPaneInfoList[rIndex].mPane->UnbindAnimationSelf(pAnimTransform);
    const u32 startIndex = rIndex;
    rIndex++;

    while (rIndex < startIndex + mPaneInfoList[startIndex].mChildCount) {
        unbindPaneCtrlAnimSub(rIndex, pAnimTransform);
    }
}

void LayoutManager::calcAnimWithoutLocationAdjust(const nw4r::lyt::DrawInfo& rDrawInfo) {
    u32 index = 0;
    animateRecursive(index, mLayout->mpRootPane);
    if (mIsScreenHidden) {
        return;
    }

    mLayout->CalculateMtx(rDrawInfo);
    for (u32 i = 0; i < mPaneCount; i++) {
        if (mPaneInfoList[i].mPaneCtrl != nullptr) {
            mPaneInfoList[i].mPaneCtrl->reflectFollowPos();
        }

        if (mPaneInfoList[i].mMtxRef != nullptr) {
            nw4r::lyt::Pane* pPane =
                mPaneInfoList[i].mPaneCtrl != nullptr ? mPaneInfoList[i].mPaneCtrl->mPane : findPaneByName(mPaneInfoList[i].mName);
            PSMTXCopy(pPane->mGlbMtx, mPaneInfoList[i].mMtxRef);
            if (MR::isScreen16Per9()) {
                mPaneInfoList[i].mMtxRef[0][3] *= MR::getScreenWidth() / 608.0f;
            }
        }
    }
}

nw4r::lyt::Group* LayoutManager::getGroup(const char* pName) const {
    nw4r::lyt::Group* pGroup = nullptr;
    nw4r::lyt::GroupList& rGroupList = mLayout->mpGroupContainer->mGroupList;
    for (nw4r::lyt::GroupList::Iterator it = rGroupList.GetBeginIter(); it != rGroupList.GetEndIter(); it++) {
        if (strcmp(it->mName, pName) == 0) {
            pGroup = &*it;
            break;
        }
    }

    return pGroup;
}

void LayoutManager::initArc(const char* pArchiveName, const char* pLayoutName) {
    mLayoutHolder = MR::createAndAddLayoutHolder(pArchiveName);

    char layoutResName[0x80];
    snprintf(layoutResName, sizeof(layoutResName), "%s.brlyt", pLayoutName);
    void* pLayoutRes = mLayoutHolder->GetResource('blyt', layoutResName, nullptr);

    mLayout = new nw4r::lyt::Layout();
    mLayout->Build(pLayoutRes, mLayoutHolder);

    if (mLayoutHolder->mAnimRes.mCount != 0) {
        mAnimTransList = new nw4r::lyt::AnimTransform*[mLayoutHolder->mAnimRes.mCount];

        for (u32 i = 0; i < mLayoutHolder->mAnimRes.mCount; i++) {
            void* pAnimRes = mLayoutHolder->mAnimRes.getRes(i);
            mAnimTransList[i] = mLayout->CreateAnimTransform(pAnimRes, mLayoutHolder);
        }
    }

    removeUnnecessaryPanes(mLayout->mpRootPane);
}

void LayoutManager::initDrawInfo() {
    nw4r::math::MTX34 mtx;
    PSMTXIdentity(mtx);

    mDrawInfo.mViewMtx = mtx;
    mDrawInfo.SetMultipleViewMtxOnDraw(true);
    mDrawInfo.mViewRect = mLayout->GetLayoutRect();
    mDrawInfo.SetLocationAdjust(true);
}

void LayoutManager::initPaneInfo() {
    mPaneCount = countPanes(mLayout->mpRootPane);
    mPaneInfoList = new LayoutPaneInfo[mPaneCount];

    u32 rIndex = 0;
    initPaneInfoRecursive(rIndex, mLayout->mpRootPane);
}

void LayoutManager::initPaneInfoRecursive(u32& rIndex, nw4r::lyt::Pane* pPane) {
    nw4r::lyt::PaneList& rPaneList = pPane->mChildList;

    mPaneInfoList[rIndex].mName = pPane->mName;
    mPaneInfoList[rIndex].mPaneCtrl = nullptr;
    mPaneInfoList[rIndex].mGroupCtrlLink = nullptr;
    mPaneInfoList[rIndex].mMtxRef = nullptr;
    mPaneInfoList[rIndex].mPane = pPane;

    const u32 startIndex = rIndex++;

    for (nw4r::lyt::PaneList::Iterator it = rPaneList.GetBeginIter(); it != rPaneList.GetEndIter(); ++it) {
        initPaneInfoRecursive(rIndex, &*it);
    }

    mPaneInfoList[startIndex].mChildCount = rIndex - startIndex;
}

u32 LayoutManager::countPanes(nw4r::lyt::Pane* pPane) {
    u32 count = 1;

    nw4r::lyt::PaneList& rPaneList = pPane->GetChildList();
    for (nw4r::lyt::PaneList::Iterator it = rPaneList.GetBeginIter(); it != rPaneList.GetEndIter(); ++it) {
        count += countPanes(&*it);
    }

    return count;
}

void LayoutManager::initGroupCtrlList() {
    mGroupCtrlCount = mLayout->mpGroupContainer->mGroupList.GetSize();
    mGroupCtrlList = new LayoutGroupCtrl*[mGroupCtrlCount];

    for (u32 i = 0; i < mGroupCtrlCount; i++) {
        mGroupCtrlList[i] = nullptr;
    }
}

void LayoutManager::initTextBoxRecursive(nw4r::lyt::Pane* pPane, nw4r::lyt::Pane* pUserDataPane, const char* pLayoutName, u32 textBoxBufferLength) {
    nw4r::lyt::TextBox* pTextBox;

    const nw4r::ut::detail::RuntimeTypeInfo* pTextBoxRuntimeInfo = &nw4r::lyt::TextBox::typeInfo;
    if (pPane != nullptr && pPane->GetRuntimeTypeInfo()->IsDerivedFrom(pTextBoxRuntimeInfo)) {
        pTextBox = static_cast< nw4r::lyt::TextBox* >(pPane);
    } else {
        pTextBox = nullptr;
    }

    char userData[9];
    LayoutCoreUtil::getPaneUserData(pPane, userData);

    if (!MR::isEqualString("", userData)) {
        pUserDataPane = pPane;
    }

    if (pTextBox != nullptr) {
        if (pUserDataPane != nullptr) {
            char messageID[0x100];
            LayoutCoreUtil::getPaneUserData(pUserDataPane, userData);
            MR::getLayoutMessageID(messageID, pLayoutName, userData);
            LayoutCoreUtil::initTextBoxPane(pTextBox, messageID, 0x100);
        } else {
            LayoutCoreUtil::initTextBoxPane(pTextBox, nullptr, textBoxBufferLength);
        }
    }

    nw4r::lyt::PaneList& rPaneList = pPane->mChildList;
    for (nw4r::lyt::PaneList::Iterator it = rPaneList.GetBeginIter(); it != rPaneList.GetEndIter(); ++it) {
        initTextBoxRecursive(&*it, pUserDataPane, pLayoutName, textBoxBufferLength);
    }
}

void LayoutManager::animateRecursive(u32& rIndex, nw4r::lyt::Pane* pPane) {
    if (mPaneInfoList[rIndex].mPaneCtrl != nullptr) {
        mPaneInfoList[rIndex].mPaneCtrl->calcAnim();
    }

    for (LayoutGroupCtrlLink* pLink = mPaneInfoList[rIndex].mGroupCtrlLink; pLink != nullptr; pLink = pLink->mNext) {
        pLink->mGroupCtrl->calcAnim();
    }

    pPane->AnimateSelf(1);
    rIndex++;

    for (nw4r::lyt::PaneList::Iterator it = pPane->mChildList.GetBeginIter(); it != pPane->mChildList.GetEndIter(); ++it) {
        animateRecursive(rIndex, &*it);
    }
}

nw4r::lyt::Pane* LayoutManager::findPaneByName(const char* pName) const {
    u32 paneCount = mPaneCount;
    for (u32 i = 0; i < paneCount; i++) {
        if (strcmp(mPaneInfoList[i].mName, pName) == 0) {
            return mPaneInfoList[i].mPane;
        }
    }

    return nullptr;
}

void LayoutManager::replaceIndDummyTexture() {
    if (!mLayoutHolder->isExistResOther("IndDummy.tpl")) {
        return;
    }

    JUTTexture screenTex(MR::getScreenResTIMG(), static_cast< u8 >(0));

    mIndDummyTexMap = new nw4r::lyt::TexMap(screenTex.getTexObj());

    void* pIndDummyResStart = mLayoutHolder->getResOther("IndDummy.tpl");
    void* pIndDummyResEnd = reinterpret_cast< void* >(-1);

    for (u32 i = 0; i < mLayoutHolder->getResOtherNum(); i++) {
        void* pRes = mLayoutHolder->getResOther(i);

        if (pIndDummyResStart < pRes && pIndDummyResEnd > pRes) {
            pIndDummyResEnd = pRes;
        }
    }

    for (u32 paneIdx = 0; paneIdx < mPaneCount; paneIdx++) {
        nw4r::lyt::Pane* pCurrPane = mPaneInfoList[paneIdx].mPane;
        nw4r::lyt::Picture* pPicPane = nw4r::ut::DynamicCast< nw4r::lyt::Picture* >(pCurrPane);

        if (pPicPane == nullptr) {
            continue;
        }

        nw4r::lyt::Material* pMaterial = pPicPane->GetMaterial();

        for (u8 texMapIdx = 0; texMapIdx < pMaterial->GetTextureNum(); texMapIdx++) {
            void* pImage = pMaterial->GetTexture(texMapIdx).mImage;

            if (pImage < pIndDummyResStart) {
                continue;
            }

            if (pImage >= pIndDummyResEnd) {
                continue;
            }

            pMaterial->SetTexture(texMapIdx, *mIndDummyTexMap);
        }
    }
}

void LayoutManager::removeUnnecessaryPanes(nw4r::lyt::Pane* pPane) {
    Local::BitFlag< 128 > languagePanes;
    Local::BitFlag< 128 > currentLanguagePanes;
    Local::collectLanguagePanes(pPane, languagePanes, currentLanguagePanes);
    if (languagePanes.isAnythingTrue()) {
        Local::removeLanguagePanes(pPane, languagePanes, currentLanguagePanes);
    } else {
        nw4r::lyt::PaneList& rChildren = pPane->mChildList;
        for (nw4r::lyt::PaneList::Iterator it = rChildren.GetBeginIter(); it != rChildren.GetEndIter(); ++it) {
            removeUnnecessaryPanes(&*it);
        }
    }
}

namespace {
    namespace Local {
        template < int N >
        bool BitFlag< N >::isAnythingTrue() const {
            for (u32 i = 0; i < N / 32; i++) {
                if (mBits[i] != 0) {
                    return true;
                }
            }

            return false;
        }

        template < int N >
        void BitFlag< N >::onBit(int index) {
            int word = index / 32;
            index -= word * 32;
            mBits[word] |= 1 << index;
        }

        template < int N >
        void BitFlag< N >::offBit(int index) {
            int word = index / 32;
            index -= word * 32;
            mBits[word] &= ~(1 << index);
        }

        template < int N >
        bool BitFlag< N >::isTrue(int index) const {
            int word = index / 32;
            index -= word * 32;
            return (mBits[word] & (1 << index)) != 0;
        }
    }  // namespace Local
}  // namespace
