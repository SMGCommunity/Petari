#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/PictureBookCloseButton.hpp"
#include "Game/Screen/PictureBookLayout.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <cstdio>

namespace {
    const s32 cBookOpenFrame = 60;
    // const s32 cBookCloseFrame
    const s32 cFadeFrame = 60;
    const s32 cWaitNoTextFrame = 60;
    const s32 cFadeTextFrame = 30;
    // const s32 cReadedSpeedRate
    const s32 cPageNextEndNormalSeStep = 81;
    const s32 cPageNextEndFastSeStep = 27;
    const char* cContentsPaneName[] = {
        "Chapter1",
        "Chapter2",
        "Chapter3",
        "Chapter4",
        "Chapter5",
        "Chapter6",
        "Chapter7",
        "Chapter8",
        "Chapter9",
    };
    const char* cContentsPointingPaneName[] = {
        "BoxButton1",
        "BoxButton2",
        "BoxButton3",
        "BoxButton4",
        "BoxButton5",
        "BoxButton6",
        "BoxButton7",
        "BoxButton8",
        "BoxButton9",
    };

    s32 getTextureNum(s32 chapterNo) {
        s32 textureNo = 0;

        char chapterArcName[64];
        snprintf(chapterArcName, sizeof(chapterArcName), "PictureBookChapter%d.arc", chapterNo);

        for (;;) {
            char pageTexName[64];
            snprintf(pageTexName, sizeof(pageTexName), "Chapter%dPage%d.bti", chapterNo, textureNo + 1);

            if (!MR::isExistResourceInArc(chapterArcName, pageTexName)) {
                break;
            }

            textureNo++;
        }

        return textureNo;
    }

    s32 getPageNum(s32 chapterNo) {
        for (s32 pageNo = 0; pageNo < 100; pageNo++) {
            char messageId[64];
            snprintf(messageId, sizeof(messageId), "PictureBookChapter%d_Page%d_%03d", chapterNo, pageNo + 1, 0);

            if (MR::isExistGameMessage(messageId)) {
                continue;
            }

            return pageNo;
        }

        return 1;
    }
};

namespace NrvPictureBookLayout {
    NEW_NERVE(PictureBookLayoutNrvOpen, PictureBookLayout, Open);
    NEW_NERVE(PictureBookLayoutNrvContentsSelect, PictureBookLayout, ContentsSelect);
    NEW_NERVE(PictureBookLayoutNrvContentsFadeOut, PictureBookLayout, ContentsFadeOut);
    NEW_NERVE(PictureBookLayoutNrvFadeIn, PictureBookLayout, FadeIn);
    NEW_NERVE(PictureBookLayoutNrvWaitNoText, PictureBookLayout, WaitNoText);
    NEW_NERVE(PictureBookLayoutNrvFadeInText, PictureBookLayout, FadeInText);
    NEW_NERVE(PictureBookLayoutNrvWaitWithText, PictureBookLayout, WaitWithText);
    NEW_NERVE(PictureBookLayoutNrvFadeOutText, PictureBookLayout, FadeOutText);
    NEW_NERVE(PictureBookLayoutNrvPageNext, PictureBookLayout, PageNext);
    NEW_NERVE(PictureBookLayoutNrvFadeOut, PictureBookLayout, FadeOut);
    NEW_NERVE(PictureBookLayoutNrvClose, PictureBookLayout, Close);
};

PictureBookLayout::PictureBookLayout(s32 chapterMin, s32 chapterMax, bool isRosettaReading) :
    LayoutActor("絵本レイアウト", true),
    mChapterMin(chapterMin),
    mChapterMax(chapterMax),
    mChapterRosettaMax(chapterMax),
    mChapterNo(1),
    mPageNo(0),
    mTextIndex(0),
    mNotReadedChapterNo(-1),
    mNotReadedPageNo(-1),
    mNotReadedTextIndex(-1),
    _44(nullptr),
    _48(nullptr),
    mTitleTexMap(nullptr),
    mCoverFrontTexMap(nullptr),
    mCoverBackTexMap(nullptr),
    mNextItemDir(1),
    mIsNextItemFast(false),
    mIconAButton(nullptr),
    mContentsButtonPaneController(nullptr),
    mCloseButton(nullptr)
{
    if (!isRosettaReading) {
        mContentsButtonPaneController = new ButtonPaneController*[getChapterMax()];
    }
}

void PictureBookLayout::init(const JMapInfoIter& rIter) {
    initLayoutManagerWithTextBoxBufferLength("PictureBook", 512, 1);
    MR::createAndAddPaneCtrl(this, "AButtonPosition", 1);
    MR::createAndAddPaneCtrl(this, "Text", 1);
    MR::connectToSceneLayout(this);
    initTexture();

    mIconAButton = MR::createAndSetupIconAButton(this, true, false);

    if (mContentsButtonPaneController != nullptr) {
        initContentsButton();

        mCloseButton = new PictureBookCloseButton(true);
        mCloseButton->initWithoutIter();
    }

    initNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeIn::sInstance);
}

void PictureBookLayout::appear() {
    mChapterNo = mChapterMin;
    mChapterMax = getChapterNumberMax();
    mPageNo = 0;
    mTextIndex = 0;
    mNotReadedChapterNo = -1;
    mNotReadedPageNo = -1;
    mNotReadedTextIndex = -1;
    mNextItemDir = 1;
    mIsNextItemFast = false;

    if (mContentsButtonPaneController == nullptr) {
        MR::hidePaneRecursive(this, "Contents");
    }
    else {
        hideContents();
    }

    LayoutActor::appear();

    if (mContentsButtonPaneController == nullptr) {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeIn::sInstance);
    }
    else {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvOpen::sInstance);
    }

    MR::requestMovementOn(this);
    MR::requestMovementOn(mIconAButton);
    MR::pauseOnCameraDirector();
    MR::deactivateGameSceneDraw3D();
}

void PictureBookLayout::kill() {
    LayoutActor::kill();
    mIconAButton->kill();

    if (mContentsButtonPaneController != nullptr) {
        mCloseButton->kill();
    }

    MR::startCurrentStageBGM();
    MR::pauseOffCameraDirector();
    MR::activateGameSceneDraw3D();
}

void PictureBookLayout::makeArchiveList(NameObjArchiveListCollector* pCollector, s32 chapterMin, s32 chapterMax, bool isRosettaReading) {
    pCollector->addArchive("PictureBook");

    for (s32 c = chapterMin; c <= chapterMax; c++) {
        char chapterArcName[64];

        snprintf(chapterArcName, sizeof(chapterArcName), "PictureBookChapter%d", c);
        pCollector->addArchive(chapterArcName);
    }

    pCollector->addArchive("PictureBookTexture");

    if (!isRosettaReading) {
        pCollector->addArchive("BackButton");
    }
}

s32 PictureBookLayout::getChapterMax() {
    return 9;
}

void PictureBookLayout::control() {
    if (mContentsButtonPaneController == nullptr) {
        return;
    }

    for (s32 i = 0; i < getChapterMax(); i++) {
        mContentsButtonPaneController[i]->update();
    }
}

void PictureBookLayout::initTexture() {
    s32 textureNum = 0;

    for (s32 c = mChapterMin; c <= mChapterMax; c++) {
        textureNum += getTextureNum(c);
    }

    _44 = new nw4r::lyt::TexMap*[textureNum];

    s32 textureIndex = 0;

    for (s32 c = mChapterMin; c <= mChapterMax; c++) {
        char chapterArcName[64];
        snprintf(chapterArcName, sizeof(chapterArcName), "PictureBookChapter%d.arc", c);

        for (s32 p = 0; p < getTextureNum(c); p++) {
            char pageTexName[64];
            snprintf(pageTexName, sizeof(pageTexName), "Chapter%dPage%d.bti", c, p + 1);

            _44[textureIndex + p] = MR::createLytTexMap(chapterArcName, pageTexName);
            textureIndex++;
        }
    }

    mTitleTexMap = MR::createLytTexMap("PictureBookTexture.arc", "PictureBookTitle.bti");
    mCoverFrontTexMap = MR::createLytTexMap("PictureBookTexture.arc", "PictureBookCoverFront.bti");
    mCoverBackTexMap = MR::createLytTexMap("PictureBookTexture.arc", "PictureBookCoverBack.bti");
}

void PictureBookLayout::initContentsButton() {
    char messageId[64];

    for (s32 i = 0; i < getChapterMax(); i++) {
        mContentsButtonPaneController[i] = new ButtonPaneController(this, cContentsPaneName[i], cContentsPointingPaneName[i], 0, false);
        mContentsButtonPaneController[i]->invalidateAppearance();

        snprintf(messageId, sizeof(messageId), "PictureBookChapter%d_Title", i + 1);

        MR::setTextBoxGameMessageRecursive(this, cContentsPaneName[i], messageId);
    }
}

bool PictureBookLayout::updateText() {
    if (mPageNo == 0) {
        char messageId[64];

        snprintf(messageId, sizeof(messageId), "PictureBookChapter%d_Title", mChapterNo);
        MR::setTextBoxGameMessageRecursive(this, "Title", messageId);

        return true;
    }
    else {
        char messageId[64];

        snprintf(messageId, sizeof(messageId), "PictureBookChapter%d_Page%d_%03d", mChapterNo, mPageNo, mTextIndex);

        if (MR::isExistGameMessage(messageId)) {
            MR::setTextBoxGameMessageRecursive(this, "Text", messageId);

            return true;
        }

        return false;
    }
}

void PictureBookLayout::updateTexture() {
    s32 textureNum = getTextureNum(mChapterNo);
    s32 pageNo;

    if (mNextItemDir > 0) {
        pageNo = mPageNo - 1;
    }
    else {
        pageNo = mPageNo;
    }

    if (pageNo == 0) {
        nw4r::lyt::TexMap* pTexMap = mTitleTexMap;

        MR::replacePaneTexture(this, "PicLeftPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnRightPage", pTexMap, 0);
    }
    else {
        s32 texMapIndex = textureNum + pageNo - 1;
        nw4r::lyt::TexMap* pTexMap = _48[texMapIndex % textureNum - 1];

        MR::replacePaneTexture(this, "PicLeftPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnRightPage", pTexMap, 0);
    }

    if (mNextItemDir > 0) {
        pageNo = mPageNo + 1;
    }
    else {
        pageNo = mPageNo;
    }

    if (pageNo == 0) {
        nw4r::lyt::TexMap* pTexMap = mTitleTexMap;

        MR::replacePaneTexture(this, "PicRightPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnLeftPage", pTexMap, 0);
    }
    else {
        s32 texMapIndex = textureNum + pageNo - 1;
        nw4r::lyt::TexMap* pTexMap = _48[texMapIndex % textureNum - 1];

        MR::replacePaneTexture(this, "PicRightPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnLeftPage", pTexMap, 0);
    }
}

bool PictureBookLayout::textNext() {
    if (mPageNo == 0) {
        return false;
    }

    mTextIndex += mNextItemDir;

    if (mTextIndex < 0) {
        return false;
    }

    return updateText();
}

bool PictureBookLayout::pageNext() {
    mPageNo += mNextItemDir;

    if (mPageNo < 0) {
        return false;
    }

    if (mPageNo != 0) {
        if (mNextItemDir > 0) {
            mTextIndex = 0;
        }
        else {
            mTextIndex = getCurrentMaxTextIndex();
        }
    }

    return updateText();
}

bool PictureBookLayout::chapterNext() {
    mChapterNo += mNextItemDir;

    if (mChapterMax < mChapterNo) {
        return false;
    }

    if (mNextItemDir > 0) {
        mPageNo = 0;
        mTextIndex = 0;
    }
    else {
        mPageNo = getPageNum(mChapterNo);
        mTextIndex = getCurrentMaxTextIndex();
    }

    return true;
}

void PictureBookLayout::updateTexMapChapterBase() {
    _48 = _44;

    for (s32 c = mChapterMin; c <= mChapterMax; c++) {
        if (c == mChapterNo) {
            break;
        }

        _48 = &_48[getTextureNum(c)];
    }
}

bool PictureBookLayout::isReadedCurrentText() const;

s32 PictureBookLayout::getReadSpeed() const {
    bool b = mContentsButtonPaneController != nullptr || mIsNextItemFast;

    if (b) {
        return 3;
    }
    else {
        return 1;
    }
}

bool PictureBookLayout::isBookEndCurrentText() const {
    return mChapterNo == 9
        && mPageNo == getPageNum(9)
        && mTextIndex == getCurrentMaxTextIndex();
}

void PictureBookLayout::setTextAlpha(f32 alpha) {
    MR::setPaneAlphaFloat(this, "Text", alpha);
    MR::setPaneAlphaFloat(this, "Title", alpha);
    MR::setPaneAlphaFloat(this, "Contents", alpha);
}

s32 PictureBookLayout::getChapterNumberMax() const {
    if (mContentsButtonPaneController == nullptr) {
        return mChapterRosettaMax;
    }

    return MR::getPictureBookChapterAlreadyRead();
}

bool PictureBookLayout::isValidCloseButton() const {
    if (mContentsButtonPaneController == nullptr) {
        return false;
    }

    return isNerve(&NrvPictureBookLayout::PictureBookLayoutNrvOpen::sInstance)
        || isNerve(&NrvPictureBookLayout::PictureBookLayoutNrvContentsSelect::sInstance)
        || isNerve(&NrvPictureBookLayout::PictureBookLayoutNrvOpen::sInstance)
        || mPageNo == 0;
}

bool PictureBookLayout::isSelectedCloseButton() const {
    return isValidCloseButton() && mCloseButton->isSelected();
}

s32 PictureBookLayout::getCurrentMaxTextIndex() const {
    s32 chapterNo;
    s32 pageNo;

    pageNo = mPageNo;
    chapterNo = mChapterNo;

    for (s32 i = 0; i < 100; i++) {
        char messageId[64];
        snprintf(messageId, sizeof(messageId), "PictureBookChapter%d_Page%d_%03d", chapterNo, pageNo, i);

        if (MR::isExistGameMessage(messageId)) {
            continue;
        }

        return i - 1;
    }

    return 0;
}

void PictureBookLayout::exeOpen() {
    nw4r::lyt::TexMap* pTexMap;

    if (MR::isFirstStep(this)) {
        updateTexMapChapterBase();
        updateText();
        MR::hidePaneRecursive(this, "Text");
        MR::hidePaneRecursive(this, "Title");
        MR::hidePaneRecursive(this, "PicToneDown");
        pTexMap = mCoverFrontTexMap;
        MR::replacePaneTexture(this, "PicLeftPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnRightPage", pTexMap, 0);
        pTexMap = mCoverBackTexMap;
        MR::replacePaneTexture(this, "PicRightPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnLeftPage", pTexMap, 0);
        MR::startAnim(this, "Appear", 0);
        MR::setAnimFrameAndStop(this, 0.0f, 0);
        MR::openWipeFade(cFadeFrame);
        MR::startStageBGM("STM_PROLOGUE_01", false);
    }

    if (MR::isStep(this, cBookOpenFrame)) {
        MR::setAnimRate(this, 1.0f, 0);
    }

    if (MR::isStep(this, 140)) {
        MR::startSystemSE("SE_SY_PICTUREBOOK_NEXT_ST", -1, -1);
    }

    s32 animFrameMax = MR::getAnimFrameMax(this, (u32)0);
    s32 stepMin = animFrameMax + cBookOpenFrame;
    s32 stepMax = stepMin + cFadeTextFrame;

    if (MR::isStep(this, stepMin)) {
        for (s32 i = 0; i < mChapterMax; i++) {
            mContentsButtonPaneController[i]->appear();
        }

        mCloseButton->appear();
    }

    if (MR::isGreaterEqualStep(this, stepMin)) {
        setTextAlpha(MR::calcNerveRate(this, stepMin, stepMax));

        if (isValidCloseButton()) {
            MR::requestStarPointerModePictureBook(this);
        }
    }

    MR::setNerveAtStep(this, &NrvPictureBookLayout::PictureBookLayoutNrvContentsSelect::sInstance, stepMax);
}

void PictureBookLayout::exeContentsSelect() {
    if (isValidCloseButton()) {
        MR::requestStarPointerModePictureBook(this);
    }

    if (mCloseButton->trySelect()) {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvContentsFadeOut::sInstance);
    }
    else {
        for (s32 i = 0; i < mChapterMax; i++) {
            if (mContentsButtonPaneController[i]->isPointingTrigger()) {
                MR::startSystemSE("SE_SY_PICBOOK_CONTENTS_CUR", -1, -1);
            }

            if (mContentsButtonPaneController[i]->trySelect()) {
                MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
                mChapterNo = i + 1;
                mCloseButton->disappear();
                setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvContentsFadeOut::sInstance);
                break;
            }
        }
    }
}

void PictureBookLayout::exeContentsFadeOut() {
    s32 stepMin = MR::getPaneAnimFrameMax(this, cContentsPaneName[mChapterNo - 1], 0);
    s32 stepMax = stepMin + cFadeTextFrame;

    if (MR::isGreaterEqualStep(this, stepMin)) {
        setTextAlpha(1.0f - MR::calcNerveRate(this, stepMin, stepMax));
    }

    if (MR::isLessStep(this, stepMax)) {
        if (isValidCloseButton()) {
            MR::requestStarPointerModePictureBook(this);
        }
    }

    if (isSelectedCloseButton()) {
        if (MR::isGreaterStep(this, stepMax)) {
            if (MR::isDead(mCloseButton)) {
                hideContents();
                setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvClose::sInstance);
            }
        }
    }
    else {
        if (MR::isStep(this, stepMax)) {
            MR::closeWipeFade(cFadeFrame);
        }

        if (MR::isGreaterStep(this, stepMax) && !MR::isWipeActive()) {
            hideContents();
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeIn::sInstance);
        }
    }
}

void PictureBookLayout::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        updateTexMapChapterBase();
        updateText();
        MR::hidePaneRecursive(this, "Text");
        MR::hidePaneRecursive(this, "Title");
        MR::hidePaneRecursive(this, "PicToneDown");
        MR::showPaneRecursive(this, "PicToneDown");

        f32 alpha;

        if (mNextItemDir > 0) {
            alpha = 0.0f;
        }
        else {
            alpha = 1.0f;
        }

        MR::setPaneAlphaFloat(this, "PicToneDown", alpha);
        updateTexture();
        MR::startAnim(this, "PageNext", 0);

        f32 animFrame;

        if (mNextItemDir > 0) {
            animFrame = MR::getAnimFrameMax(this, (u32)0);
        }
        else {
            animFrame = 0.0f;
        }

        MR::setAnimFrameAndStop(this, animFrame, 0);
        MR::openWipeFade(cFadeFrame / getReadSpeed());

        if (mContentsButtonPaneController == nullptr) {
            MR::startStageBGM("STM_PROLOGUE_01", false);
        }
    }

    if (!MR::isWipeActive()) {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvWaitNoText::sInstance);
    }
}

void PictureBookLayout::exeWaitNoText() {
    bool b = mContentsButtonPaneController != nullptr || mIsNextItemFast;

    MR::setNerveAtStep(this, &NrvPictureBookLayout::PictureBookLayoutNrvFadeInText::sInstance, b ? 0 : cWaitNoTextFrame);
}

void PictureBookLayout::exeFadeInText() {
    if (MR::isFirstStep(this)) {
        if (mPageNo == 0) {
            MR::showPaneRecursive(this, "Title");

            if (mContentsButtonPaneController != nullptr) {
                mCloseButton->appear();
            }
        }
        else {
            MR::showPaneRecursive(this, "Text");
            MR::showPaneRecursive(this, "PicToneDown");
            MR::startPaneAnim(this, "Text", "TextColor", 0);

            if (isReadedCurrentText()) {
                MR::setPaneAnimFrameAndStop(this, "Text", 1.0f, 0);
            }
            else {
                MR::setPaneAnimFrameAndStop(this, "Text", 0.0f, 0);
            }
        }

        if (mChapterNo == 7) {
            if (mPageNo <= 5 && MR::isStopOrFadeoutBgmName("STM_PROLOGUE_01")) {
                MR::startStageBGM("STM_PROLOGUE_01", false);
            }

            if (mPageNo == 6) {
                MR::stopStageBGM(120);
            }

            if (mPageNo == 7 && MR::isStopOrFadeoutBgmName("STM_PROLOGUE_01_B")) {
                MR::startStageBGM("STM_PROLOGUE_01_B", false);
            }

            if (mPageNo == 8) {
                if (mTextIndex >= 1) {
                    MR::stopStageBGM(120);
                }
                else if (MR::isStopOrFadeoutBgmName("STM_PROLOGUE_01_B")) {
                    MR::startStageBGM("STM_PROLOGUE_01_B", false);
                }
            }
        }
        else if (MR::isStopOrFadeoutBgmName("STM_PROLOGUE_01")) {
            MR::startStageBGM("STM_PROLOGUE_01", 0);
        }

        if (isBookEndCurrentText()) {
            MR::setTextBoxHorizontalPositionCenterRecursive(this, "Text");
            MR::setTextBoxVerticalPositionCenterRecursive(this, "Text");
        }
        else {
            MR::setTextBoxHorizontalPositionLeftRecursive(this, "Text");
            MR::setTextBoxVerticalPositionTopRecursive(this, "Text");
        }

        if (mIsNextItemFast && !isReadedCurrentText()) {
            mIsNextItemFast = false;
        }
    }

    if (isValidCloseButton()) {
        MR::requestStarPointerModePictureBook(this);
    }

    s32 step = 30 / getReadSpeed();
    f32 alpha = MR::calcNerveRate(this, step);

    MR::setPaneAlphaFloat(this, "Text", alpha);
    MR::setPaneAlphaFloat(this, "Title", alpha);
    MR::setPaneAlphaFloat(this, "Contents", alpha);
    MR::setPaneAlphaFloat(this, "PicToneDown", getFadeInAlphaTextBG(alpha));
    MR::setNerveAtStep(this, &NrvPictureBookLayout::PictureBookLayoutNrvWaitWithText::sInstance, step);
}

void PictureBookLayout::exeWaitWithText() {
    if (MR::isFirstStep(this)) {
        mIconAButton->openWithTurn();
    }

    if (isValidCloseButton()) {
        MR::requestStarPointerModePictureBook(this);
    }

    if (isValidCloseButton() && mCloseButton->trySelect()) {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeOutText::sInstance);
    }
    else {
        bool isTriggerNextPage = MR::testCorePadTriggerA(WPAD_CHAN0)
            || MR::testCorePadTriggerRight(WPAD_CHAN0)
            || MR::testSubPadStickTriggerRight(WPAD_CHAN0);

        if (isTriggerNextPage) {
            MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);

            if (mNextItemDir > 0 && !isReadedCurrentText()) {
                mNotReadedChapterNo = mChapterNo;
                mNotReadedPageNo = mPageNo;
                mNotReadedTextIndex = mTextIndex;
            }

            if (isValidCloseButton()) {
                mCloseButton->disappear();
            }

            mNextItemDir = 1;

            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeOutText::sInstance);
        }
        else {
            bool isTriggerPrevPage = MR::testCorePadTriggerLeft(WPAD_CHAN0)
                || MR::testSubPadStickTriggerLeft(WPAD_CHAN0);

            if (isTriggerPrevPage) {
                if (mChapterMin < mChapterNo || mPageNo > 0 || mTextIndex > 0) {
                    MR::startSystemSE("SE_SY_TALK_FOCUS_ITEM", -1, -1);

                    if (isValidCloseButton()) {
                        mCloseButton->disappear();
                    }

                    mIsNextItemFast = true;
                    mNextItemDir = -1;

                    setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeOutText::sInstance);
                }
            }
        }
    }
}

void PictureBookLayout::exeFadeOutText() {
    if (MR::isFirstStep(this)) {
        mIconAButton->term();
    }

    if (isValidCloseButton()) {
        MR::requestStarPointerModePictureBook(this);
    }

    s32 step = 30 / getReadSpeed();
    f32 alpha = 1.0f - MR::calcNerveRate(this, step);

    MR::setPaneAlphaFloat(this, "Text", alpha);
    MR::setPaneAlphaFloat(this, "Title", alpha);
    MR::setPaneAlphaFloat(this, "Contents", alpha);
    MR::setPaneAlphaFloat(this, "PicToneDown", getFadeOutAlphaTextBG(alpha));

    if (isSelectedCloseButton()) {
        if (MR::isGreaterStep(this, step) && MR::isDead(mCloseButton)) {
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvClose::sInstance);
        }
    }
    else if (MR::isStep(this, step)) {
        if (textNext()) {
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeInText::sInstance);
        }
        else if (pageNext()) {
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvPageNext::sInstance);
        }
        else if (chapterNext()) {
            if (MR::isPlayingStageBgmName("STM_PROLOGUE_01_B")) {
                MR::stopStageBGM(120);
            }

            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeOut::sInstance);
        }
        else if (mContentsButtonPaneController == nullptr) {
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeOut::sInstance);
        }
        else {
            setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvClose::sInstance);
        }
    }
}

void PictureBookLayout::exePageNext() {
    if (MR::isFirstStep(this)) {
        MR::hidePaneRecursive(this, "Text");
        MR::hidePaneRecursive(this, "Title");
        MR::hidePaneRecursive(this, "PicToneDown");
        updateTexture();

        if (mNextItemDir > 0) {
            MR::startAnim(this, "PageNext", 0);
        }
        else {
            MR::startAnimReverseOneTime(this, "PageNext", 0);
        }

        if (mIsNextItemFast) {
            MR::startSystemSE("SE_SY_PICTUREBOOK_NEXT_F_ST", -1, -1);
        }
        else {
            MR::startSystemSE("SE_SY_PICTUREBOOK_NEXT_ST", -1, -1);
        }

        MR::setAnimRate(this, mNextItemDir * getReadSpeed(), 0);
    }

    if (mIsNextItemFast) {
        if (MR::isStep(this, cPageNextEndFastSeStep)) {
            MR::startSystemSE("SE_SY_PICTUREBOOK_NEXT_F_ED", -1, -1);
        }
    }
    else if (MR::isStep(this, cPageNextEndNormalSeStep)) {
        MR::startSystemSE("SE_SY_PICTUREBOOK_NEXT_ED", -1, -1);
    }

    MR::setNerveAtAnimStopped(this, &NrvPictureBookLayout::PictureBookLayoutNrvWaitNoText::sInstance, 0);
}

void PictureBookLayout::exeFadeOut() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeFade(cFadeFrame / getReadSpeed());

        if (mContentsButtonPaneController == nullptr) {
            MR::stopStageBGM(cFadeFrame / getReadSpeed());
        }
    }

    if (MR::isWipeActive()) {
        return;
    }

    if (mChapterMax >= mChapterNo) {
        setNerve(&NrvPictureBookLayout::PictureBookLayoutNrvFadeIn::sInstance);
    }
    else {
        kill();
    }
}

void PictureBookLayout::exeClose() {
    nw4r::lyt::TexMap* pTexMap;

    if (MR::isFirstStep(this)) {
        MR::hidePaneRecursive(this, "Text");
        MR::hidePaneRecursive(this, "Title");
        MR::hidePaneRecursive(this, "PicToneDown");
        MR::startAnim(this, "End", 0);

        if (isSelectedCloseButton()) {
            pTexMap = mTitleTexMap;

            MR::replacePaneTexture(this, "PicLeftPage", pTexMap, 0);
            MR::replacePaneTexture(this, "PicTurnRightPage", pTexMap, 0);
        }
        else {
            pTexMap = _48[mPageNo - 2];

            MR::replacePaneTexture(this, "PicLeftPage", pTexMap, 0);
            MR::replacePaneTexture(this, "PicTurnRightPage", pTexMap, 0);
        }

        pTexMap = mCoverBackTexMap;

        MR::replacePaneTexture(this, "PicRightPage", pTexMap, 0);
        MR::replacePaneTexture(this, "PicTurnLeftPage", pTexMap, 0);
    }

    s32 step = MR::getAnimFrameMax(this, (u32)0);

    if (MR::isStep(this, step)) {
        MR::closeWipeFade(cFadeFrame);
        MR::stopStageBGM(cFadeFrame);
    }

    if (MR::isStep(this, 150)) {
        MR::startSystemSE("SE_SY_PICTUREBOOK_END", -1, -1);
    }

    if (MR::isGreaterStep(this, step)) {
        if (!MR::isWipeActive()) {
            kill();
        }
    }
}

void PictureBookLayout::hideContents() {
    for (s32 i = 0; i < getChapterMax(); i++) {
        mContentsButtonPaneController[i]->forceToHide();
    }
}

// PictureBookLayout::getFadeInAlphaTextBG
// PictureBookLayout::getFadeOutAlphaTextBG
