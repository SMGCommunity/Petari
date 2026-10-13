#include "Game/Util/LayoutUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/CustomTagProcessor.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/LayoutPaneCtrl.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/J2DGraph/J2DPicture.hpp>
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <nw4r/lyt/layout.h>
#include <nw4r/lyt/material.h>
#include <nw4r/lyt/picture.h>
#include <nw4r/lyt/textBox.h>

template nw4r::lyt::TextBox* nw4r::ut::DynamicCast< nw4r::lyt::TextBox*, nw4r::lyt::Pane >(nw4r::lyt::Pane*);

template const nw4r::lyt::Pane* nw4r::ut::LinkList< nw4r::lyt::Pane, 4 >::ConstIterator::operator->() const;
template void JGeometry::TVec2< f32 >::set< f32 >(f32, f32);
template void JGeometry::TBox2< f32 >::set(const TVec2f&, const TVec2f&);

template nw4r::lyt::PaneList::Iterator nw4r::ut::LinkList< nw4r::lyt::Pane, 4 >::Iterator::operator++(int);

extern "C" int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);

void LayoutUtil_FORCE_MATCH_SDATA2() {
    (void)255.0f;
    (void)0.75f;
    (void)4.0f;
}

namespace {
    void setInfluencedAlphaToChild(nw4r::lyt::Pane* pPane) {
        pPane->SetInfluencedAlpha(true);

        nw4r::lyt::PaneList& rChildren = pPane->GetChildList();

        for (nw4r::lyt::PaneList::Iterator it = rChildren.GetBeginIter(); it != rChildren.GetEndIter(); it++) {
            it->SetInfluencedAlpha(true);
        }
    }

    void showPaneRecursive(nw4r::lyt::Pane* pPane) {
        pPane->SetVisible(true);

        for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
            showPaneRecursive(&*it);
        }
    }

    void hidePaneRecursive(nw4r::lyt::Pane* pPane) {
        pPane->SetVisible(false);

        for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
            hidePaneRecursive(&*it);
        }
    }

    void initFrameCtrlReverse(J3DFrameCtrl* pFrameCtrl) {
        pFrameCtrl->setAttribute(pFrameCtrl->EMode_RESET);
        pFrameCtrl->setRate(-pFrameCtrl->mRate);
        pFrameCtrl->setFrame(pFrameCtrl->mEnd);
    }

    bool getTextDrawRectRecursive(nw4r::ut::Rect* pRect, const nw4r::lyt::Pane* pPane, bool initialized) {
        const nw4r::lyt::PaneList& rChildren = pPane->GetChildList();

        for (nw4r::lyt::PaneList::ConstIterator it = rChildren.GetBeginIter(); it != rChildren.GetEndIter(); ++it) {
            initialized = getTextDrawRectRecursive(pRect, &*it, initialized);
        }

        const nw4r::lyt::TextBox* pTextBox = nw4r::ut::DynamicCast< const nw4r::lyt::TextBox* >(pPane);

        if (pTextBox != nullptr) {
            nw4r::ut::Rect rect = pTextBox->GetTextDrawRect(nw4r::lyt::DrawInfo());

            if (initialized) {
                if (rect.left < pRect->left) {
                    pRect->left = rect.left;
                }

                if (rect.top < pRect->top) {
                    pRect->top = rect.top;
                }

                if (rect.right > pRect->right) {
                    pRect->right = rect.right;
                }

                if (rect.bottom > pRect->bottom) {
                    pRect->bottom = rect.bottom;
                }
            } else {
                *pRect = rect;
                initialized = true;
            }
        }

        return initialized;
    }

    u32 getTextLineNumMaxRecursiveSub(const nw4r::lyt::Pane* pPane) {
        u32 max = 0;
        const nw4r::lyt::PaneList& rChildren = pPane->GetChildList();

        for (nw4r::lyt::PaneList::ConstIterator it = rChildren.GetBeginIter(); it != rChildren.GetEndIter(); ++it) {
            u32 count = getTextLineNumMaxRecursiveSub(&*it);

            if (count > max) {
                max = count;
            }
        }

        const nw4r::lyt::TextBox* pTextBox = nw4r::ut::DynamicCast< const nw4r::lyt::TextBox* >(pPane);

        if (pTextBox != nullptr) {
            u32 count = MR::countMessageLine(pTextBox->mTextBuf);

            if (count > max) {
                max = count;
            }
        }

        return max;
    }
};  // namespace

LayoutHolder* MR::createAndAddLayoutHolder(const char* pArcName) {
    return SingletonHolder< ResourceHolderManager >::get()->createAndAddLayoutHolder(pArcName, nullptr);
}

LayoutHolder* MR::createAndAddLayoutHolderRawData(const char* pArcPath) {
    return SingletonHolder< ResourceHolderManager >::get()->createAndAddLayoutHolderRawData(pArcPath);
}

void MR::createAndAddPaneCtrl(LayoutActor* pActor, const char* pPaneName, u32 animLayerNum) {
    pActor->getLayoutManager()->createAndAddPaneCtrl(pPaneName, animLayerNum);
}

void MR::createAndAddGroupCtrl(LayoutActor* pActor, const char* pGroupName, u32 animLayerNum) {
    pActor->getLayoutManager()->createAndAddGroupCtrl(pGroupName, animLayerNum);
}

bool MR::isExistPaneCtrl(LayoutActor* pActor, const char* pPaneName) {
    return pActor->getLayoutManager()->isExistPaneCtrl(pPaneName);
}

u8 MR::getPaneAlpha(const LayoutActor* pActor, const char* pPaneName) {
    return getPane(pActor, pPaneName)->mAlpha;
}

void MR::setInfluencedAlphaToChild(const LayoutActor* pActor) {
    ::setInfluencedAlphaToChild(getRootPane(pActor));
}

void MR::setLayoutAlpha(const LayoutActor* pActor, u8 alpha) {
    getRootPane(pActor)->mAlpha = alpha;
}

void MR::setLayoutAlphaFloat(const LayoutActor* pActor, f32 alpha) {
    f32 alphaFloat = clamp(alpha, 0.0f, 1.0f);

    setLayoutAlpha(pActor, alphaFloat * 255.0f);
}

void MR::setPaneAlpha(const LayoutActor* pActor, const char* pPaneName, u8 alpha) {
    getPane(pActor, pPaneName)->mAlpha = alpha;
}

void MR::setPaneAlphaFloat(const LayoutActor* pActor, const char* pPaneName, f32 alpha) {
    f32 alphaFloat = MR::clamp(alpha, 0.0f, 1.0f);

    setPaneAlpha(pActor, pPaneName, alphaFloat * 255.0f);
}

void MR::executeTextBoxRecursive(LayoutActor* pActor, const char* pPaneName, const TextBoxRecursiveOperation& rOperation) {
    nw4r::lyt::TextBox* pTextBox = nw4r::ut::DynamicCast< nw4r::lyt::TextBox* >(getPane(pActor, pPaneName));

    if (pTextBox != nullptr) {
        rOperation.execute(pTextBox);
    }

    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);

    for (nw4r::lyt::PaneList::Iterator it = pPane->GetChildList().GetBeginIter(); it != pPane->GetChildList().GetEndIter(); ++it) {
        executeTextBoxRecursive(pActor, it->mName, rOperation);
    }
}

void MR::setTextBoxGameMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
    setTextBoxMessageRecursive(pActor, pPaneName, getGameMessageDirect(pMessageId));
}

void MR::setTextBoxLayoutMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
    setTextBoxMessageRecursive(pActor, pPaneName, getLayoutMessageDirect(pMessageId));
}

void MR::setTextBoxSystemMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
    setTextBoxMessageRecursive(pActor, pPaneName, getSystemMessageDirect(pMessageId));
}

void MR::setTextBoxMessageRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pMessage) {
    executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetMessage(pMessage));
}

void MR::setTextBoxFormatRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pFormat, ...) {
    wchar_t message[256];
    va_list list;

    va_start(list, pFormat);
    vswprintf(message, ARRAY_SIZE(message), pFormat, list);
    va_end(list);

    setTextBoxMessageRecursive(pActor, pPaneName, message);
}

void MR::setTextBoxArgNumberRecursive(LayoutActor* pActor, const char* pPaneName, s32 number, s32 argIndex) {
    executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetArgNumber(number, argIndex));
}

void MR::setTextBoxArgStringRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pMessage, s32 argIndex) {
    executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetArgString(pMessage, argIndex));
}

void MR::setTextBoxArgGameMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId, s32 argIndex) {
    setTextBoxArgStringRecursive(pActor, pPaneName, getGameMessageDirect(pMessageId), argIndex);
}

void setTextBoxVerticalPositionRecursive(LayoutActor* pActor, const char* pPaneName, u8 position) {
    MR::executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetVerticalPosition(position));
}

void MR::setTextBoxVerticalPositionTopRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxVerticalPositionRecursive(pActor, pPaneName, 0);
}

void MR::setTextBoxVerticalPositionCenterRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxVerticalPositionRecursive(pActor, pPaneName, 1);
}

void MR::setTextBoxVerticalPositionBottomRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxVerticalPositionRecursive(pActor, pPaneName, 2);
}

void setTextBoxHorizontalPositionRecursive(LayoutActor* pActor, const char* pPaneName, u8 position) {
    MR::executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetHorizontalPosition(position));
}

void MR::setTextBoxHorizontalPositionLeftRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxHorizontalPositionRecursive(pActor, pPaneName, 0);
}

void MR::setTextBoxHorizontalPositionCenterRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxHorizontalPositionRecursive(pActor, pPaneName, 1);
}

void MR::updateClearTimeTextBox(LayoutActor* pActor, const char* pPaneName, u32 step) {
    wchar_t clearTimeText[16];

    makeClearTimeString(clearTimeText, step);
    setTextBoxMessageRecursive(pActor, pPaneName, clearTimeText);
}

void MR::updateMinuteAndSecondTextBox(LayoutActor* pActor, const char* pPaneName, u32 step) {
    wchar_t minuteAndSecondText[16];

    makeMinuteAndSecondString(minuteAndSecondText, step);
    setTextBoxMessageRecursive(pActor, pPaneName, minuteAndSecondText);
}

void MR::setTextBoxFontRecursive(LayoutActor* pActor, const char* pPaneName, nw4r::ut::Font* pFont) {
    executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetFont(pFont));
}

void MR::showPane(LayoutActor* pActor, const char* pPaneName) {
    getPane(pActor, pPaneName)->SetVisible(true);
}

void MR::showPaneRecursive(LayoutActor* pActor, const char* pPaneName) {
    ::showPaneRecursive(getPane(pActor, pPaneName));
}

void MR::hidePane(LayoutActor* pActor, const char* pPaneName) {
    getPane(pActor, pPaneName)->SetVisible(false);
}

void MR::hidePaneRecursive(LayoutActor* pActor, const char* pPaneName) {
    ::hidePaneRecursive(getPane(pActor, pPaneName));
}

bool MR::isHiddenPane(const LayoutActor* pActor, const char* pPaneName) {
    return !getPane(pActor, pPaneName)->IsVisible();
}

void MR::showScreen(LayoutActor* pActor) {
    LayoutManager* pLayoutManager = pActor->getLayoutManager();

    pLayoutManager->mIsScreenHidden = false;
}

void MR::hideScreen(LayoutActor* pActor) {
    LayoutManager* pLayoutManager = pActor->getLayoutManager();

    pLayoutManager->mIsScreenHidden = true;
}

void MR::setFollowPos(const TVec2f* pFollowPos, const LayoutActor* pActor, const char* pPaneName) {
    pActor->getLayoutManager()->getPaneCtrl(pPaneName)->mFollowPos = pFollowPos;
}

void MR::setFollowTypeReplace(const LayoutActor* pActor, const char* pPaneName) {
    LayoutPaneCtrl* pCtrl = pActor->getLayoutManager()->getPaneCtrl(pPaneName);
    pCtrl->mFollowType = 0;
}

void MR::setFollowTypeAdd(const LayoutActor* pActor, const char* pPaneName) {
    LayoutPaneCtrl* pCtrl = pActor->getLayoutManager()->getPaneCtrl(pPaneName);
    pCtrl->mFollowType = 1;
}

void MR::copyPaneTrans(TVec2f* pTrans, const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    pTrans->x = pPane->mGlbMtx._03;
    pTrans->y = pPane->mGlbMtx._13;
    convertLayoutPosToScreenPos(pTrans, *pTrans);
}

f32 MR::getPaneTransX(const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    TVec2f trans(pPane->mGlbMtx._03, pPane->mGlbMtx._13);
    convertLayoutPosToScreenPos(&trans, trans);
    return trans.x;
}

f32 MR::getPaneTransY(const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    TVec2f trans(pPane->mGlbMtx._03, pPane->mGlbMtx._13);
    convertLayoutPosToScreenPos(&trans, trans);
    return trans.y;
}

void MR::setLayoutPosAtPaneTrans(LayoutActor* pActor, const LayoutActor* pFollowActor, const char* pPaneName) {
    TVec2f trans;
    copyPaneTrans(&trans, pFollowActor, pPaneName);
    pActor->setTrans(trans);
}

void MR::copyPaneScale(TVec2f* pScale, const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    pScale->set< f32 >(pPane->mScale.x, pPane->mScale.y);
}

void MR::setPaneScale(const LayoutActor* pActor, f32 x, f32 y, const char* pPaneName) {
    nw4r::math::VEC2 scale(x, y);
    getPane(pActor, pPaneName)->mScale = scale;
}

void MR::setLayoutScaleAtPaneScale(LayoutActor* pActor, const LayoutActor* pFollowActor, const char* pPaneName) {
    TVec2f scale;
    copyPaneScale(&scale, pFollowActor, pPaneName);
    setPaneScale(pActor, scale.x, scale.y, nullptr);
}

void MR::copyPaneRotate(TVec3f* pRotate, const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    pRotate->set< f32 >(pPane->mRotate.x, pPane->mRotate.y, pPane->mRotate.z);
}

void MR::setPaneRotate(const LayoutActor* pActor, f32 x, f32 y, f32 z, const char* pPaneName) {
    nw4r::math::VEC3 rotate;
    rotate.x = x;
    rotate.y = y;
    rotate.z = z;
    getPane(pActor, pPaneName)->mRotate = rotate;
}

void MR::setLayoutScalePosAtPaneScaleTrans(LayoutActor* pActor, const LayoutActor* pFollowActor, const char* pPaneName) {
    setLayoutPosAtPaneTrans(pActor, pFollowActor, pPaneName);
    setLayoutScaleAtPaneScale(pActor, pFollowActor, pPaneName);
}

void MR::setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(LayoutActor* pActor, const LayoutActor* pFollowActor, const char* pPaneName) {
    if (isExecuteCalcAnimLayout(pActor)) {
        setLayoutScalePosAtPaneScaleTrans(pActor, pFollowActor, pPaneName);
    }
}

nw4r::lyt::TexMap* MR::createLytTexMap(const char* pArcName, const char* pTextureName) {
    JUTTexture texture(loadTexFromArc(pArcName, pTextureName), 0);

    return new nw4r::lyt::TexMap(texture.getTexObj());
}

nw4r::lyt::TexMap* MR::createLytTexMap(ResTIMG* pImage) {
    JUTTexture texture(pImage, 0);

    return new nw4r::lyt::TexMap(texture.getTexObj());
}

nw4r::lyt::TexMap* MR::getLytTexMap(LayoutActor* pActor, const char* pPaneName, u8 textureIndex) {
    nw4r::lyt::Picture* pPicture = nw4r::ut::DynamicCast< nw4r::lyt::Picture* >(getPane(pActor, pPaneName));

    return const_cast< nw4r::lyt::TexMap* >(&pPicture->GetMaterial()->GetTexture(textureIndex));
}

void MR::replacePaneTexture(LayoutActor* pActor, const char* pPaneName, const nw4r::lyt::TexMap* pTexture, u8 textureIndex) {
    nw4r::lyt::Picture* pPicture = nw4r::ut::DynamicCast< nw4r::lyt::Picture* >(getPane(pActor, pPaneName));
    pPicture->GetMaterial()->SetTexture(textureIndex, *pTexture);
}

void MR::startAnim(LayoutActor* pActor, const char* pAnimName, u32 animLayer) {
    pActor->getLayoutManager()->getPaneCtrl(nullptr)->start(pAnimName, animLayer);

    if (pActor->mEffectKeeper != nullptr) {
        pActor->mEffectKeeper->changeAnim();
    }
}

void MR::startAnimAtFirstStep(LayoutActor* pActor, const char* pAnimName, u32 animLayer) {
    if (isFirstStep(pActor)) {
        startAnim(pActor, pAnimName, animLayer);
    }
}

void MR::startPaneAnim(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
    pActor->getLayoutManager()->getPaneCtrl(pPaneName)->start(pAnimName, animLayer);
}

void MR::startPaneAnimAtStep(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, s32 step, u32 animLayer) {
    if (isStep(pActor, step)) {
        startPaneAnim(pActor, pPaneName, pAnimName, animLayer);
    }
}

void MR::startPaneAnimAtFirstStep(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
    if (isFirstStep(pActor)) {
        startPaneAnim(pActor, pPaneName, pAnimName, animLayer);
    }
}

void MR::startAnimReverseOneTime(LayoutActor* pActor, const char* pAnimName, u32 animLayer) {
    LayoutPaneCtrl* pPaneCtrl = pActor->getLayoutManager()->getPaneCtrl(nullptr);

    pPaneCtrl->start(pAnimName, animLayer);
    ::initFrameCtrlReverse(pPaneCtrl->getFrameCtrl(animLayer));
}

void MR::startPaneAnimReverseOneTime(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
    LayoutPaneCtrl* pPaneCtrl = pActor->getLayoutManager()->getPaneCtrl(pPaneName);

    pPaneCtrl->start(pAnimName, animLayer);
    ::initFrameCtrlReverse(pPaneCtrl->getFrameCtrl(animLayer));
}

void MR::startAnimAndSetFrameAndStop(LayoutActor* pActor, const char* pAnimName, f32 animFrame, u32 animLayer) {
    startAnim(pActor, pAnimName, animLayer);
    setAnimFrameAndStop(pActor, animFrame, animLayer);
}

void MR::setAnimFrameAndStop(LayoutActor* pActor, f32 animFrame, u32 animLayer) {
    J3DFrameCtrl* pFrameCtrl = getAnimCtrl(pActor, animLayer);

    pFrameCtrl->setFrame(animFrame);
    pFrameCtrl->setRate(0.0f);
}

void MR::setAnimFrameAndStopAtEnd(LayoutActor* pActor, u32 animLayer) {
    setAnimFrameAndStop(pActor, getAnimFrameMax(pActor, animLayer), animLayer);
}

void MR::setAnimFrameAndStopAdjustTextWidth(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    nw4r::ut::Rect rect;
    ::getTextDrawRectRecursive(&rect, pPane, false);
    setAnimFrameAndStop(pActor, rect.GetWidth(), animLayer);
}

void MR::setAnimFrameAndStopAdjustTextHeight(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    nw4r::ut::Rect rect;
    ::getTextDrawRectRecursive(&rect, pPane, false);
    setAnimFrameAndStop(pActor, rect.GetHeight(), animLayer);
}

void MR::setPaneAnimFrameAndStop(LayoutActor* pActor, const char* pPaneName, f32 animFrame, u32 animLayer) {
    J3DFrameCtrl* pFrameCtrl = getPaneAnimCtrl(pActor, pPaneName, animLayer);

    pFrameCtrl->setFrame(animFrame);
    pFrameCtrl->setRate(0.0f);
}

void MR::setPaneAnimFrameAndStopAtEnd(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    setPaneAnimFrameAndStop(pActor, pPaneName, getPaneAnimFrameMax(pActor, pPaneName, animLayer), animLayer);
}

void MR::setAnimFrame(LayoutActor* pActor, f32 animFrame, u32 animLayer) {
    getAnimCtrl(pActor, animLayer)->setFrame(animFrame);
}

void MR::setPaneAnimFrame(LayoutActor* pActor, const char* pPaneName, f32 animFrame, u32 animLayer) {
    getPaneAnimCtrl(pActor, pPaneName, animLayer)->setFrame(animFrame);
}

void MR::setAnimRate(LayoutActor* pActor, f32 animRate, u32 animLayer) {
    getAnimCtrl(pActor, animLayer)->setRate(animRate);
}

void MR::setPaneAnimRate(LayoutActor* pActor, const char* pPaneName, f32 animRate, u32 animLayer) {
    getPaneAnimCtrl(pActor, pPaneName, animLayer)->setRate(animRate);
}

void MR::stopAnim(LayoutActor* pActor, u32 animLayer) {
    pActor->getLayoutManager()->getPaneCtrl(nullptr)->stop(animLayer);
}

void MR::stopPaneAnim(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    pActor->getLayoutManager()->getPaneCtrl(pPaneName)->stop(animLayer);
}

bool MR::isAnimStopped(const LayoutActor* pActor, u32 animLayer) {
    return pActor->getLayoutManager()->getPaneCtrl(nullptr)->isAnimStopped(animLayer);
}

bool MR::isPaneAnimStopped(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    return pActor->getLayoutManager()->getPaneCtrl(pPaneName)->isAnimStopped(animLayer);
}

f32 MR::getAnimFrame(const LayoutActor* pActor, u32 animLayer) {
    return getAnimCtrl(pActor, animLayer)->getFrame();
}

f32 MR::getPaneAnimFrame(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    return getPaneAnimCtrl(pActor, pPaneName, animLayer)->getFrame();
}

s16 MR::getAnimFrameMax(const LayoutActor* pActor, u32 animLayer) {
    return getAnimCtrl(pActor, animLayer)->getEnd();
}

s16 MR::getPaneAnimFrameMax(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    return getPaneAnimCtrl(pActor, pPaneName, animLayer)->getEnd();
}

s16 MR::getAnimFrameMax(const LayoutActor* pActor, const char* pAnimName) {
    return pActor->getLayoutManager()->getAnimTransform(pAnimName)->GetFrameSize();
}

J3DFrameCtrl* MR::getAnimCtrl(const LayoutActor* pActor, u32 animLayer) {
    return pActor->getLayoutManager()->getPaneCtrl(nullptr)->getFrameCtrl(animLayer);
}

J3DFrameCtrl* MR::getPaneAnimCtrl(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
    return pActor->getLayoutManager()->getPaneCtrl(pPaneName)->getFrameCtrl(animLayer);
}

void MR::emitEffect(LayoutActor* pActor, const char* pEffectName) {
    pActor->mEffectKeeper->createEmitter(pEffectName);
}

void MR::deleteEffect(LayoutActor* pActor, const char* pEffectName) {
    pActor->mEffectKeeper->deleteEmitter(pEffectName);
}

void MR::forceDeleteEffect(LayoutActor* pActor, const char* pEffectName) {
    pActor->mEffectKeeper->forceDeleteEmitter(pEffectName);
}

void MR::deleteEffectAll(LayoutActor* pActor) {
    pActor->mEffectKeeper->deleteEmitterAll();
}

void MR::forceDeleteEffectAll(LayoutActor* pActor) {
    pActor->mEffectKeeper->forceDeleteEmitterAll();
}

void MR::setEffectHostMtx(LayoutActor* pActor, const char* pParam2, MtxPtr pHostMtx) {
    getEffect(pActor, pParam2)->setHostMtx(pHostMtx);
}

void MR::setEffectRate(LayoutActor* pActor, const char* pEffectName, f32 rate) {
    getEffect(pActor, pEffectName)->setRate(rate, -1);
}

void MR::setEffectDirectionalSpeed(LayoutActor* pActor, const char* pEffectName, f32 speed) {
    getEffect(pActor, pEffectName)->setDirectionalSpeed(speed, -1);
}

void MR::pauseOffEffectAll(LayoutActor* pActor) {
    MultiEmitter* pEmitter;
    PaneEffectKeeper* pKeeper = pActor->mEffectKeeper;

    for (s32 i = 0; i < pKeeper->mEmitters.size(); i++) {
        pEmitter = pKeeper->mEmitters.begin()[i];

        if (pEmitter != nullptr && pEmitter->isValid()) {
            pEmitter->pauseOff(-1);
        }
    }
}

bool MR::isRegisteredEffect(const LayoutActor* pActor, const char* pParam2) {
    if (pParam2 != nullptr) {
        return pActor->mEffectKeeper->getEmitter(pParam2) != nullptr;
    }

    return pActor->mEffectKeeper != nullptr;
}

void MR::copyLayoutDrawInfoWithAspect(nw4r::lyt::DrawInfo* pDrawInfo, const LayoutActor* pActor, bool aspect) {
    *pDrawInfo = pActor->getLayoutManager()->mDrawInfo;

    if (aspect) {
        pDrawInfo->mFlag.locationAdjust = true;
        pDrawInfo->mLocationAdjustScale = nw4r::math::VEC2(0.75f, 1.0f);
    } else {
        pDrawInfo->mFlag.locationAdjust = false;
        pDrawInfo->mLocationAdjustScale = nw4r::math::VEC2(1.0f, 1.0f);
    }
}

void MR::calcAnimLayoutWithDrawInfo(const LayoutActor* pActor, const nw4r::lyt::DrawInfo& rDrawInfo) {
    if (isExecuteCalcAnimLayout(pActor)) {
        pActor->getLayoutManager()->calcAnimWithoutLocationAdjust(rDrawInfo);
    }
}

void MR::drawLayoutWithDrawInfoWithoutProjectionSetup(const LayoutActor* pActor, const nw4r::lyt::DrawInfo& rDrawInfo) {
    if (isExecuteDrawLayout(pActor)) {
        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        pActor->getLayoutManager()->mLayout->Draw(rDrawInfo);
    }
}

bool MR::isStep(const LayoutActor* pActor, s32 step) {
    return pActor->getNerveStep() == step;
}

bool MR::isFirstStep(const LayoutActor* pActor) {
    return isStep(pActor, 0);
}

bool MR::isLessStep(const LayoutActor* pActor, s32 step) {
    if (!isNewNerve(pActor) && pActor->getNerveStep() < step) {
        return true;
    }

    return false;
}

bool MR::isGreaterStep(const LayoutActor* pActor, s32 step) {
    return pActor->getNerveStep() > step;
}

bool MR::isGreaterEqualStep(const LayoutActor* pActor, s32 step) {
    return pActor->getNerveStep() >= step;
}

bool MR::isIntervalStep(const LayoutActor* pActor, s32 step) {
    return pActor->getNerveStep() % step == 0;
}

bool MR::isNewNerve(const LayoutActor* pActor) {
    return pActor->getNerveStep() < 0;
}

f32 MR::calcNerveRate(const LayoutActor* pActor, s32 stepMax) {
    return stepMax <= 0 ? 1.0f : clamp(static_cast< f32 >(pActor->getNerveStep()) / stepMax, 0.0f, 1.0f);
}

f32 MR::calcNerveRate(const LayoutActor* pActor, s32 stepMin, s32 stepMax) {
    return clamp(normalize(pActor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);
}

f32 MR::calcNerveEaseInRate(const LayoutActor* pActor, s32 stepMax) {
    return getEaseInValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
}

f32 MR::calcNerveEaseInValue(const LayoutActor* pActor, s32 stepMin, s32 stepMax, f32 valueMin, f32 valueMax) {
    return getEaseInValue(calcNerveRate(pActor, stepMin, stepMax), valueMin, valueMax, 1.0f);
}

void MR::setNerveAtStep(LayoutActor* pActor, const Nerve* pNerve, s32 step) {
    if (pActor->getNerveStep() == step) {
        pActor->setNerve(pNerve);
    }
}

void MR::setNerveAtAnimStopped(LayoutActor* pActor, const Nerve* pNerve, u32 animLayer) {
    if (isAnimStopped(pActor, animLayer)) {
        pActor->setNerve(pNerve);
    }
}

void MR::setNerveAtPaneAnimStopped(LayoutActor* pActor, const char* pPaneName, const Nerve* pNerve, u32 animLayer) {
    if (isPaneAnimStopped(pActor, pPaneName, animLayer)) {
        pActor->setNerve(pNerve);
    }
}

void MR::killAtAnimStopped(LayoutActor* pActor, u32 animLayer) {
    if (isAnimStopped(pActor, animLayer)) {
        pActor->kill();
    }
}

bool MR::isDead(const LayoutActor* pActor) {
    return pActor->mFlag.mIsDead;
}

bool MR::isHiddenLayout(const LayoutActor* pActor) {
    return pActor->mFlag.mIsHidden;
}

void MR::showLayout(LayoutActor* pActor) {
    pActor->mFlag.mIsHidden = false;
    pActor->mFlag.mIsOffCalcAnim = false;
}

void MR::hideLayout(LayoutActor* pActor) {
    pActor->mFlag.mIsHidden = true;
    pActor->mFlag.mIsOffCalcAnim = true;
}

bool MR::isStopAnimFrame(const LayoutActor* pActor) {
    return pActor->mFlag.mIsStopAnimFrame;
}

void MR::stopAnimFrame(LayoutActor* pActor) {
    pActor->mFlag.mIsStopAnimFrame = true;
}

void MR::releaseAnimFrame(LayoutActor* pActor) {
    pActor->mFlag.mIsStopAnimFrame = false;
}

void MR::onCalcAnim(LayoutActor* pActor) {
    pActor->mFlag.mIsOffCalcAnim = false;
}

void MR::offCalcAnim(LayoutActor* pActor) {
    pActor->mFlag.mIsOffCalcAnim = true;
}

bool MR::isExecuteCalcAnimLayout(const LayoutActor* pActor) {
    if (pActor->mFlag.mIsDead) {
        return false;
    }

    if (pActor->mLayoutManager == nullptr) {
        return false;
    }

    return !pActor->mFlag.mIsOffCalcAnim;
}

bool MR::isExecuteDrawLayout(const LayoutActor* pActor) {
    if (pActor->mFlag.mIsDead) {
        return false;
    }

    if (pActor->mLayoutManager == nullptr) {
        return false;
    }

    return !pActor->mFlag.mIsHidden;
}

SimpleLayout* MR::createSimpleLayout(const char* pName, const char* pArcName, u32 rootPaneAnimLayerNum) {
    SimpleLayout* simpleLayout;

    simpleLayout = new SimpleLayout(pName, pArcName, rootPaneAnimLayerNum, MR::DrawType_Layout);
    simpleLayout->initWithoutIter();

    return simpleLayout;
}

SimpleLayout* MR::createSimpleLayoutTalkParts(const char* pName, const char* pArcName, u32 rootPaneAnimLayerNum) {
    SimpleLayout* simpleLayout;

    simpleLayout = new SimpleLayout(pName, pArcName, rootPaneAnimLayerNum, MR::DrawType_0x44);
    simpleLayout->initWithoutIter();

    return simpleLayout;
}

nw4r::lyt::Pane* MR::getPane(const LayoutActor* pActor, const char* pPaneName) {
    return pActor->getLayoutManager()->getPane(pPaneName);
}

nw4r::lyt::Pane* MR::getRootPane(const LayoutActor* pActor) {
    return pActor->getLayoutManager()->getPane(nullptr);
}

void MR::calcTextBoxRectRecursive(TBox2f* pBox, const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);
    nw4r::ut::Rect rect;

    if (::getTextDrawRectRecursive(&rect, pPane, false)) {
        TVec2f min;
        TVec2f max;
        convertPaneLocalPosToScreenPos(&min, pPane, TVec2f(rect.left, rect.top));
        convertPaneLocalPosToScreenPos(&max, pPane, TVec2f(rect.right, rect.bottom));
        pBox->set(min, max);
    } else {
        pBox->i.zero();
        pBox->f.zero();
    }
}

u32 MR::getTextLineNumMaxRecursive(const LayoutActor* pActor, const char* pPaneName) {
    nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);

    if (pPane != nullptr) {
        return ::getTextLineNumMaxRecursiveSub(pPane);
    }

    return 0;
}

void MR::invalidateParentAnim(LayoutActor* pActor) {
    LayoutManager* pLayoutManager = pActor->getLayoutManager();

    pLayoutManager->_61 = 0;
}

namespace {
    f32 getCometColorAnimFrameFromId(s32 id) {
        f32 frame = 0.0f;

        switch (id) {
        case 0:
            return frame;
        case 1:
            frame = 4.0f;
            break;
        case 2:
            frame = 1.0f;
            break;
        case 3:
            frame = 2.0f;
            break;
        case 4:
            frame = 3.0f;
            break;
        default:
            break;
        }

        return frame;
    }
};  // namespace

void MR::setCometPaneAnimFromId(LayoutActor* pActor, const char* pPaneName, int cometId, u32 animLayer) {
    startPaneAnim(pActor, pPaneName, "Color", animLayer);
    setPaneAnimFrameAndStop(pActor, pPaneName, ::getCometColorAnimFrameFromId(cometId), animLayer);
}

void MR::setTextBoxNumberRecursive(LayoutActor* pActor, const char* pPaneName, s32 number) {
    setTextBoxFormatRecursive(pActor, pPaneName, L"%d", number);
}

void MR::clearTextBoxMessageRecursive(LayoutActor* pActor, const char* pPaneName) {
    setTextBoxMessageRecursive(pActor, pPaneName, L"");
}

IconAButton* MR::createAndSetupIconAButton(LayoutActor* pActor, bool connectToScene, bool connectToPause) {
    IconAButton* iconAButton;

    iconAButton = new IconAButton(connectToScene, connectToPause);
    iconAButton->initWithoutIter();
    iconAButton->setFollowActorPane(pActor, "AButtonPosition");

    return iconAButton;
}

void MR::setCometAnimFromId(LayoutActor* pActor, int cometId, u32 animLayer) {
    startAnim(pActor, "Color", animLayer);
    setAnimFrameAndStop(pActor, ::getCometColorAnimFrameFromId(cometId), animLayer);
}

void LayoutUtil_FORCE_MATCH(nw4r::lyt::Pane* pPane, J2DPicture* pPicture) {
    pPane->SetInfluencedAlpha(true);
    pPicture->J2DPicture::draw(0.0f, 0.0f, static_cast< u8 >(0), false, false, false);
    pPicture->J2DPicture::draw(0.0f, 0.0f, static_cast< u8 >(1), false, false, false);
}
