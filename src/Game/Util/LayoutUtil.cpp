#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/LayoutPaneCtrl.hpp"
#include "Game/Screen/PaneEffectKeeper.hpp"
#include "Game/Screen/SimpleLayout.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>
#include <nw4r/lyt/layout.h>

extern "C" int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);

void setTextBoxVerticalPositionRecursive(LayoutActor* pActor, const char* pPaneName, u8 position) {
    MR::executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetVerticalPosition(position));
}

void setTextBoxHorizontalPositionRecursive(LayoutActor* pActor, const char* pPaneName, u8 position) {
    MR::executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetHorizontalPosition(position));
}

namespace {
    void showPaneRecursive(nw4r::lyt::Pane*);
    void hidePaneRecursive(nw4r::lyt::Pane*);
    void initFrameCtrlReverse(J3DFrameCtrl*);
    bool getTextDrawRectRecursive(nw4r::ut::Rect*, const nw4r::lyt::Pane*, bool);
    u32  getTextLineNumMaxRecursiveSub(const nw4r::lyt::Pane*);

    f32 getCometColorAnimFrameFromId(s32 id) {
        switch (id) {
        case 0:
            return 0.0f;
        case 1:
            return 4.0f;
        case 2:
            return 1.0f;
        case 3:
            return 2.0f;
        case 4:
            return 3.0f;
        default:
            return 0.0f;
        }
    }
}; // namespace

namespace MR {
    LayoutHolder* createAndAddLayoutHolder(const char* pArcName) {
        return SingletonHolder<ResourceHolderManager>::get()->createAndAddLayoutHolder(pArcName, nullptr);
    }

    LayoutHolder* createAndAddLayoutHolderRawData(const char* pArcPath) {
        return SingletonHolder<ResourceHolderManager>::get()->createAndAddLayoutHolderRawData(pArcPath);
    }

    void createAndAddPaneCtrl(LayoutActor* pActor, const char* pPaneName, u32 animLayerNum) {
        pActor->getLayoutManager()->createAndAddPaneCtrl(pPaneName, animLayerNum);
    }

    void createAndAddGroupCtrl(LayoutActor* pActor, const char* pGroupName, u32 animLayerNum) {
        pActor->getLayoutManager()->createAndAddGroupCtrl(pGroupName, animLayerNum);
    }

    bool isExistPaneCtrl(LayoutActor* pActor, const char* pPaneName) {
        return pActor->getLayoutManager()->isExistPaneCtrl(pPaneName);
    }

    void setTextBoxGameMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
        setTextBoxMessageRecursive(pActor, pPaneName, getGameMessageDirect(pMessageId));
    }

    void setTextBoxLayoutMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
        setTextBoxMessageRecursive(pActor, pPaneName, getLayoutMessageDirect(pMessageId));
    }

    void setTextBoxSystemMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId) {
        setTextBoxMessageRecursive(pActor, pPaneName, getSystemMessageDirect(pMessageId));
    }

    void setTextBoxMessageRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pMessage) {
        executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetMessage(pMessage));
    }

    void setTextBoxFormatRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pFormat, ...) {
        wchar_t message[256];
        va_list list;

        va_start(list, pFormat);
        vswprintf(message, sizeof(message) / sizeof(*message), pFormat, list);
        va_end(list);

        setTextBoxMessageRecursive(pActor, pPaneName, message);
    }

    void setTextBoxArgNumberRecursive(LayoutActor* pActor, const char* pPaneName, s32 number, s32 param4) {
        executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetArgNumber(number, param4));
    }

    void setTextBoxArgStringRecursive(LayoutActor* pActor, const char* pPaneName, const wchar_t* pMessage, s32 param4) {
        executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetArgString(pMessage, param4));
    }

    void setTextBoxArgGameMessageRecursive(LayoutActor* pActor, const char* pPaneName, const char* pMessageId, s32 param4) {
        setTextBoxArgStringRecursive(pActor, pPaneName, getGameMessageDirect(pMessageId), param4);
    }

    void setTextBoxVerticalPositionTopRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxVerticalPositionRecursive(pActor, pPaneName, 0);
    }

    void setTextBoxVerticalPositionCenterRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxVerticalPositionRecursive(pActor, pPaneName, 1);
    }

    void setTextBoxVerticalPositionBottomRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxVerticalPositionRecursive(pActor, pPaneName, 2);
    }

    void setTextBoxHorizontalPositionLeftRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxHorizontalPositionRecursive(pActor, pPaneName, 0);
    }

    void setTextBoxHorizontalPositionCenterRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxHorizontalPositionRecursive(pActor, pPaneName, 1);
    }

    void updateClearTimeTextBox(LayoutActor* pActor, const char* pPaneName, u32 step) {
        wchar_t clearTimeText[16];

        makeClearTimeString(clearTimeText, step);
        setTextBoxMessageRecursive(pActor, pPaneName, clearTimeText);
    }

    void updateMinuteAndSecondTextBox(LayoutActor* pActor, const char* pPaneName, u32 step) {
        wchar_t minuteAndSecondText[16];

        makeMinuteAndSecondString(minuteAndSecondText, step);
        setTextBoxMessageRecursive(pActor, pPaneName, minuteAndSecondText);
    }

    void setTextBoxFontRecursive(LayoutActor* pActor, const char* pPaneName, nw4r::ut::Font* pFont) {
        executeTextBoxRecursive(pActor, pPaneName, TextBoxRecursiveSetFont(pFont));
    }

    void showPaneRecursive(LayoutActor* pActor, const char* pPaneName) {
        ::showPaneRecursive(getPane(pActor, pPaneName));
    }

    void hidePaneRecursive(LayoutActor* pActor, const char* pPaneName) {
        ::hidePaneRecursive(getPane(pActor, pPaneName));
    }

    void showScreen(LayoutActor* pActor) {
        LayoutManager* pLayoutManager = pActor->getLayoutManager();

        pLayoutManager->mIsScreenHidden = false;
    }

    void hideScreen(LayoutActor* pActor) {
        LayoutManager* pLayoutManager = pActor->getLayoutManager();

        pLayoutManager->mIsScreenHidden = true;
    }

    void setFollowPos(const TVec2f* pFollowPos, const LayoutActor* pActor, const char* pPaneName) {
        pActor->getLayoutManager()->getPaneCtrl(pPaneName)->mFollowPos = pFollowPos;
    }

    void setFollowTypeReplace(const LayoutActor* pActor, const char* pPaneName) {
        pActor->getLayoutManager()->getPaneCtrl(pPaneName)->mFollowType = 0;
    }

    void setFollowTypeAdd(const LayoutActor* pActor, const char* pPaneName) {
        pActor->getLayoutManager()->getPaneCtrl(pPaneName)->mFollowType = 1;
    }

    void startAnimAtFirstStep(LayoutActor* pActor, const char* pAnimName, u32 animLayer) {
        if (isFirstStep(pActor)) {
            startAnim(pActor, pAnimName, animLayer);
        }
    }

    void startPaneAnim(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
        pActor->getLayoutManager()->getPaneCtrl(pPaneName)->start(pAnimName, animLayer);
    }

    void startPaneAnimAtStep(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, s32 step, u32 animLayer) {
        if (isStep(pActor, step)) {
            startPaneAnim(pActor, pPaneName, pAnimName, animLayer);
        }
    }

    void startPaneAnimAtFirstStep(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
        if (isFirstStep(pActor)) {
            startPaneAnim(pActor, pPaneName, pAnimName, animLayer);
        }
    }

    void startAnimReverseOneTime(LayoutActor* pActor, const char* pAnimName, u32 animLayer) {
        LayoutPaneCtrl* pPaneCtrl = pActor->getLayoutManager()->getPaneCtrl(nullptr);

        pPaneCtrl->start(pAnimName, animLayer);
        ::initFrameCtrlReverse(pPaneCtrl->getFrameCtrl(animLayer));
    }

    void startPaneAnimReverseOneTime(LayoutActor* pActor, const char* pPaneName, const char* pAnimName, u32 animLayer) {
        LayoutPaneCtrl* pPaneCtrl = pActor->getLayoutManager()->getPaneCtrl(pPaneName);

        pPaneCtrl->start(pAnimName, animLayer);
        ::initFrameCtrlReverse(pPaneCtrl->getFrameCtrl(animLayer));
    }

    void startAnimAndSetFrameAndStop(LayoutActor* pActor, const char* pAnimName, f32 animFrame, u32 animLayer) {
        startAnim(pActor, pAnimName, animLayer);
        setAnimFrameAndStop(pActor, animFrame, animLayer);
    }

    void setAnimFrameAndStop(LayoutActor* pActor, f32 animFrame, u32 animLayer) {
        J3DFrameCtrl* pFrameCtrl = getAnimCtrl(pActor, animLayer);

        pFrameCtrl->mCurrentFrame = animFrame;
        pFrameCtrl->mSpeed = 0.0f;
    }

    void setAnimFrameAndStopAtEnd(LayoutActor* pActor, u32 animLayer) {
        setAnimFrameAndStop(pActor, getAnimCtrl(pActor, animLayer)->mEndFrame, animLayer);
    }

    void setPaneAnimFrameAndStop(LayoutActor* pActor, const char* pPaneName, f32 animFrame, u32 animLayer) {
        J3DFrameCtrl* pFrameCtrl = getPaneAnimCtrl(pActor, pPaneName, animLayer);

        pFrameCtrl->mCurrentFrame = animFrame;
        pFrameCtrl->mSpeed = 0.0f;
    }

    void setPaneAnimFrameAndStopAtEnd(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        setPaneAnimFrameAndStop(pActor, pPaneName, getPaneAnimCtrl(pActor, pPaneName, animLayer)->mEndFrame, animLayer);
    }

    void setAnimFrame(LayoutActor* pActor, f32 animFrame, u32 animLayer) {
        getAnimCtrl(pActor, animLayer)->mCurrentFrame = animFrame;
    }

    void setPaneAnimFrame(LayoutActor* pActor, const char* pPaneName, f32 animFrame, u32 animLayer) {
        getPaneAnimCtrl(pActor, pPaneName, animLayer)->mCurrentFrame = animFrame;
    }

    void setAnimRate(LayoutActor* pActor, f32 animRate, u32 param3) {
        getAnimCtrl(pActor, param3)->mSpeed = animRate;
    }

    void setPaneAnimRate(LayoutActor* pActor, const char* pPaneName, f32 animRate, u32 animLayer) {
        getPaneAnimCtrl(pActor, pPaneName, animLayer)->mSpeed = animRate;
    }

    void stopAnim(LayoutActor* pActor, u32 animLayer) {
        pActor->getLayoutManager()->getPaneCtrl(nullptr)->stop(animLayer);
    }

    void stopPaneAnim(LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        pActor->getLayoutManager()->getPaneCtrl(pPaneName)->stop(animLayer);
    }

    bool isAnimStopped(const LayoutActor* pActor, u32 animLayer) {
        return pActor->getLayoutManager()->getPaneCtrl(nullptr)->isAnimStopped(animLayer);
    }

    bool isPaneAnimStopped(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        return pActor->getLayoutManager()->getPaneCtrl(pPaneName)->isAnimStopped(animLayer);
    }

    f32 getAnimFrame(const LayoutActor* pActor, u32 param2) {
        return getAnimCtrl(pActor, param2)->mCurrentFrame;
    }

    f32 getPaneAnimFrame(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        return getPaneAnimCtrl(pActor, pPaneName, animLayer)->mCurrentFrame;
    }

    s16 getAnimFrameMax(const LayoutActor* pActor, u32 param2) {
        return getAnimCtrl(pActor, param2)->mEndFrame;
    }

    s16 getPaneAnimFrameMax(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        return getPaneAnimCtrl(pActor, pPaneName, animLayer)->mEndFrame;
    }

    s16 getAnimFrameMax(const LayoutActor* pActor, const char* pAnimName) {
        return pActor->getLayoutManager()->getAnimTransform(pAnimName)->GetFrameSize();
    }

    J3DFrameCtrl* getAnimCtrl(const LayoutActor* pActor, u32 animLayer) {
        return pActor->getLayoutManager()->getPaneCtrl(nullptr)->getFrameCtrl(animLayer);
    }

    J3DFrameCtrl* getPaneAnimCtrl(const LayoutActor* pActor, const char* pPaneName, u32 animLayer) {
        return pActor->getLayoutManager()->getPaneCtrl(pPaneName)->getFrameCtrl(animLayer);
    }

    void setEffectHostMtx(LayoutActor* pActor, const char* pParam2, MtxPtr pHostMtx) {
        getEffect(pActor, pParam2)->setHostMtx(pHostMtx);
    }

    bool isRegisteredEffect(const LayoutActor* pActor, const char* pParam2) {
        if (pParam2 != nullptr) {
            return pActor->mPaneEffectKeeper->getEmitter(pParam2) != nullptr;
        } else {
            return pActor->mPaneEffectKeeper != nullptr;
        }
    }

    void calcAnimLayoutWithDrawInfo(const LayoutActor* pActor, const nw4r::lyt::DrawInfo& rDrawInfo) {
        if (isExecuteCalcAnimLayout(pActor)) {
            pActor->getLayoutManager()->calcAnimWithoutLocationAdjust(rDrawInfo);
        }
    }

    void drawLayoutWithDrawInfoWithoutProjectionSetup(const LayoutActor* pActor, const nw4r::lyt::DrawInfo& rDrawInfo) {
        if (isExecuteDrawLayout(pActor)) {
            GXSetCullMode(GX_CULL_NONE);
            GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
            pActor->getLayoutManager()->mLayout->Draw(rDrawInfo);
        }
    }

    bool isStep(const LayoutActor* pActor, s32 step) {
        return pActor->getNerveStep() == step;
    }

    bool isFirstStep(const LayoutActor* pActor) {
        return isStep(pActor, 0);
    }

    bool isLessStep(const LayoutActor* pActor, s32 step) {
        if (!isNewNerve(pActor) && pActor->getNerveStep() < step) {
            return true;
        }

        return false;
    }

    bool isGreaterStep(const LayoutActor* pActor, s32 step) {
        return pActor->getNerveStep() > step;
    }

    bool isGreaterEqualStep(const LayoutActor* pActor, s32 step) {
        return pActor->getNerveStep() >= step;
    }

    bool isIntervalStep(const LayoutActor* pActor, s32 step) {
        return pActor->getNerveStep() % step == 0;
    }

    bool isNewNerve(const LayoutActor* pActor) {
        return pActor->getNerveStep() < 0;
    }

    f32 calcNerveRate(const LayoutActor* pActor, s32 stepMax) {
        return stepMax <= 0 ? 1.0f : clamp(static_cast<f32>(pActor->getNerveStep()) / stepMax, 0.0f, 1.0f);
    }

    f32 calcNerveRate(const LayoutActor* pActor, s32 stepMin, s32 stepMax) {
        return clamp(normalize(pActor->getNerveStep(), stepMin, stepMax), 0.0f, 1.0f);
    }

    f32 calcNerveEaseInRate(const LayoutActor* pActor, s32 stepMax) {
        return getEaseInValue(calcNerveRate(pActor, stepMax), 0.0f, 1.0f, 1.0f);
    }

    f32 calcNerveEaseInValue(const LayoutActor* pActor, s32 stepMin, s32 stepMax, f32 valueMin, f32 valueMax) {
        return getEaseInValue(calcNerveRate(pActor, stepMin, stepMax), valueMin, valueMax, 1.0f);
    }

    void setNerveAtStep(LayoutActor* pActor, const Nerve* pNerve, s32 step) {
        if (pActor->getNerveStep() == step) {
            pActor->setNerve(pNerve);
        }
    }

    void setNerveAtAnimStopped(LayoutActor* pActor, const Nerve* pNerve, u32 animLayer) {
        if (isAnimStopped(pActor, animLayer)) {
            pActor->setNerve(pNerve);
        }
    }

    void setNerveAtPaneAnimStopped(LayoutActor* pActor, const char* pPaneName, const Nerve* pNerve, u32 animLayer) {
        if (isPaneAnimStopped(pActor, pPaneName, animLayer)) {
            pActor->setNerve(pNerve);
        }
    }

    void killAtAnimStopped(LayoutActor* pActor, u32 animLayer) {
        if (isAnimStopped(pActor, animLayer)) {
            pActor->kill();
        }
    }

    bool isDead(const LayoutActor* pActor) {
        return pActor->mFlag.mIsDead;
    }

    bool isHiddenLayout(const LayoutActor* pActor) {
        return pActor->mFlag.mIsHidden;
    }

    void showLayout(LayoutActor* pActor) {
        pActor->mFlag.mIsHidden = false;
        pActor->mFlag.mIsOffCalcAnim = false;
    }

    void hideLayout(LayoutActor* pActor) {
        pActor->mFlag.mIsHidden = true;
        pActor->mFlag.mIsOffCalcAnim = true;
    }

    bool isStopAnimFrame(const LayoutActor* pActor) {
        return pActor->mFlag.mIsStopAnimFrame;
    }

    void stopAnimFrame(LayoutActor* pActor) {
        pActor->mFlag.mIsStopAnimFrame = true;
    }

    void releaseAnimFrame(LayoutActor* pActor) {
        pActor->mFlag.mIsStopAnimFrame = false;
    }

    void onCalcAnim(LayoutActor* pActor) {
        pActor->mFlag.mIsOffCalcAnim = false;
    }

    void offCalcAnim(LayoutActor* pActor) {
        pActor->mFlag.mIsOffCalcAnim = true;
    }

    bool isExecuteCalcAnimLayout(const LayoutActor* pActor) {
        if (pActor->mFlag.mIsDead) {
            return false;
        }

        if (pActor->mManager == nullptr) {
            return false;
        }

        return !pActor->mFlag.mIsOffCalcAnim;
    }

    bool isExecuteDrawLayout(const LayoutActor* pActor) {
        if (pActor->mFlag.mIsDead) {
            return false;
        }

        if (pActor->mManager == nullptr) {
            return false;
        }

        return !pActor->mFlag.mIsHidden;
    }

    SimpleLayout* createSimpleLayout(const char* pName, const char* pArcName, u32 param3) {
        SimpleLayout* pSimpleLayout = new SimpleLayout(pName, pArcName, param3, 60);

        pSimpleLayout->initWithoutIter();

        return pSimpleLayout;
    }

    SimpleLayout* createSimpleLayoutTalkParts(const char* pName, const char* pArcName, u32 param3) {
        SimpleLayout* pSimpleLayout = new SimpleLayout(pName, pArcName, param3, 68);

        pSimpleLayout->initWithoutIter();

        return pSimpleLayout;
    }

    nw4r::lyt::Pane* getPane(const LayoutActor* pActor, const char* pPaneName) {
        return pActor->getLayoutManager()->getPane(pPaneName);
    }

    nw4r::lyt::Pane* getRootPane(const LayoutActor* pActor) {
        return pActor->getLayoutManager()->getPane(nullptr);
    }

    u32 getTextLineNumMaxRecursive(const LayoutActor* pActor, const char* pPaneName) {
        nw4r::lyt::Pane* pPane = getPane(pActor, pPaneName);

        if (pPane != nullptr) {
            return ::getTextLineNumMaxRecursiveSub(pPane);
        } else {
            return 0;
        }
    }

    void invalidateParentAnim(LayoutActor* pActor) {
        LayoutManager* pLayoutManager = pActor->getLayoutManager();

        pLayoutManager->_61 = 0;
    }

    void setCometPaneAnimFromId(LayoutActor* pActor, const char* pPaneName, int cometId, u32 animLayer) {
        startPaneAnim(pActor, pPaneName, "Color", animLayer);
        setPaneAnimFrameAndStop(pActor, pPaneName, ::getCometColorAnimFrameFromId(cometId), animLayer);
    }

    void setTextBoxNumberRecursive(LayoutActor* pActor, const char* pPaneName, s32 number) {
        setTextBoxFormatRecursive(pActor, pPaneName, L"%d", number);
    }

    void clearTextBoxMessageRecursive(LayoutActor* pActor, const char* pPaneName) {
        setTextBoxMessageRecursive(pActor, pPaneName, L"");
    }

    IconAButton* createAndSetupIconAButton(LayoutActor* pActor, bool param2, bool param3) {
        IconAButton* pIconAButton = new IconAButton(param2, param3);

        pIconAButton->initWithoutIter();
        pIconAButton->setFollowActorPane(pActor, "AButtonPosition");

        return pIconAButton;
    }

    void setCometAnimFromId(LayoutActor* pActor, int cometId, u32 animLayer) {
        startAnim(pActor, "Color", animLayer);
        setAnimFrameAndStop(pActor, ::getCometColorAnimFrameFromId(cometId), animLayer);
    }
}; // namespace MR
