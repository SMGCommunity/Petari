#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/LayoutGroupCtrl.hpp"
#include "Game/Screen/LayoutPaneCtrl.hpp"
#include "Game/System/LayoutHolder.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
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
#include <cstdio>
#include <cstring>

namespace {
    static const char* const cRemoveString[] = {
        "4x3",
        "16x9",
        "Replace",
    };
}

LayoutManager::LayoutManager(const char* pLayoutName, bool a2, u32 rootPaneAnimLayerNum, u32 textBoxBufferLength)
    : mLayoutHolder(), mLayout(), mAnimTransList(), mDrawInfo(), mIsScreenHidden(), _61(true), mIndDummyTexMap(), mPaneCount(), mPaneInfoList(),
      mGroupCtrlCount(), mGroupCtrlList(), mLayoutName() {
    if (a2) {
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

    for (u32 i = 0; i < ARRAY_SIZE(cRemoveString); i++) {
        char* pos = strstr(fileName, cRemoveString[i]);

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
    mPaneInfoList[rIndex]._8 = 0;
    mPaneInfoList[rIndex]._C = 0;
    mPaneInfoList[rIndex].mPane = pPane;

    u32 startIndex = rIndex++;

    for (nw4r::lyt::PaneList::Iterator it = rPaneList.GetBeginIter(); it != rPaneList.GetEndIter(); ++it) {
        initPaneInfoRecursive(rIndex, &*it);  // TODO * operator being inlined
    }

    mPaneInfoList[startIndex].mChildCount = rIndex - startIndex;
}

// TODO: instruction swap
u32 LayoutManager::countPanes(nw4r::lyt::Pane* pPane) {
    u32 count = 1;

    nw4r::lyt::PaneList& rPaneList = pPane->mChildList;
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
    if (pPane && pPane->GetRuntimeTypeInfo()->IsDerivedFrom(pTextBoxRuntimeInfo)) {
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

void LayoutManager::replaceIndDummyTexture() {
    if (!mLayoutHolder->isExistResOther("IndDummy.tpl")) {
        return;
    }

    JUTTexture screenTex(MR::getScreenResTIMG(), static_cast< u8 >(0));

    mIndDummyTexMap = new nw4r::lyt::TexMap(screenTex.getTexObj());

    // ???
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
        nw4r::lyt::Picture* pPicPane;
        const nw4r::ut::detail::RuntimeTypeInfo* pPictureRuntimeInfo = &nw4r::lyt::Picture::typeInfo;

        if (pCurrPane && pCurrPane->GetRuntimeTypeInfo()->IsDerivedFrom(pPictureRuntimeInfo)) {
            pPicPane = static_cast<nw4r::lyt::Picture*>(pCurrPane);
        }
        else {
            pPicPane = nullptr;
        }

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

}
