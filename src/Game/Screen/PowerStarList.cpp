#include "Game/Screen/PowerStarList.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/NWC24/NWC24Messenger.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/OdhConverter.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <nw4r/lyt/drawInfo.h>
#include <nw4r/lyt/pane.h>

extern "C" int swprintf(wchar_t* pBuffer, size_t length, const wchar_t* pFormat, ...);

#define MAX_PAGES 5
#define ITEMS_PER_PAGE 15
#define MAX_ITEMS MAX_PAGES* ITEMS_PER_PAGE

void PowerStarList_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    class ListItem {
    public:
        /// @brief Creates a new `ListItem`.
        ListItem() NO_INLINE : mType(-1), mRaceId() {
        }

        /* 0x0 */ s32 mType;
        union {
            /* 0x4 */ GalaxyStatusAccessor mGalaxyAccessor;
            /* 0x4 */ s32 mRaceId;
        };
    };

    static const char* const cGalaxyNamePaneTable[] = {
        "TxtStage1", "TxtStage2", "TxtStage3", "TxtStage4", "TxtStage5", "TxtStage6", "TxtStage7", "TxtStage8",
    };

    static const char* const cStarNumPaneTable[] = {
        "TxtStar1",
        "TxtStar2",
        "TxtStar3",
        "TxtStar4",
    };

    static const char* const cCoinPaneTable[] = {
        "TxtCoinPiece1",
        "TxtCoinPiece2",
        "TxtCoinPiece3",
        "TxtCoinPiece4",
    };

    static const char* const cCrownPaneTable[] = {
        "TxtComp1",
        "TxtComp2",
        "TxtComp3",
        "TxtComp4",
    };

    static const char* const cTimePaneTable[] = {
        "TxtTime1",
        "TxtTime2",
        "TxtTime3",
        "TxtTime4",
    };

    static const char* const cSeparatorPaneTable[] = {
        "PicBelt1",
        "PicBelt2",
        "PicBelt3",
        "PicBelt4",
    };

    static const char* const cSeparatorSizePaneTable[] = {
        "PicBelt1",
        "PicBelt2",
        "PicBelt3",
        "WinBelt4",
    };

    const f32 cSeparatorOffset = 20.0f;
    const f32 cTimePaneDefaultHeight = 72.0f;
    const char* cMailMessageID = "WiiMessageStarList";

    s32 getSortPriority(const GalaxyStatusAccessor* pAccessor) NO_INLINE {
        const char* pName = pAccessor->getName();

        if (pAccessor->getNormalScenarioNum() == 3) {
            return 1;
        }

        if (MR::isEqualString(pName, "HeavensDoorGalaxy")) {
            return 2;
        }

        if (pAccessor->isExistGrandStar()) {
            return 3;
        }

        if (GameDataConst::isQuestionGalaxy(pName)) {
            return 4;
        }

        return 5;
    }

    s32 getDisplayRaceBestTimeNum() NO_INLINE {
        s32 num = 0;

        for (s32 idx = 0; idx < 5; idx++) {
            if (RaceManagerFunction::hasPowerStarRaceScenario(idx)) {
                num++;
            }
        }

        return num;
    };

    void sortGalaxyAccessors(GalaxyStatusAccessor* pBegin, GalaxyStatusAccessor* pEnd) {
        for (GalaxyStatusAccessor* pCurrent = pBegin + 1; pCurrent != pEnd; pCurrent++) {
            for (GalaxyStatusAccessor* p = pCurrent; p != pBegin && getSortPriority(p) < getSortPriority(p - 1); p--) {
                GalaxyStatusAccessor temp = *p;
                *p = *(p - 1);
                *(p - 1) = temp;
            }
        }
    }

    s32 getSortedList(ListItem* pItems) {
        GalaxyStatusAccessor galaxyAccessors[MAX_ITEMS];
        s32 availableGalaxies = 0;

        ScenarioDataIter iter = MR::makeBeginScenarioDataIter();

        for (; !iter.isEnd();) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();
            bool open = accessor.getPowerStarNum() == 0 ? false : MR::isOnGameEventFlagGalaxyOpen(accessor.getName());

            if (open) {
                galaxyAccessors[availableGalaxies] = iter.makeAccessor();
                availableGalaxies++;
            }

            iter.goNext();
        }

        sortGalaxyAccessors(galaxyAccessors, galaxyAccessors + availableGalaxies);

        s32 outCount = 0;
        for (s32 readIndex = 0; readIndex < availableGalaxies; readIndex++) {
            pItems[outCount].mType = 0;
            pItems[outCount].mGalaxyAccessor = galaxyAccessors[readIndex];
            outCount++;
        }

        if (getDisplayRaceBestTimeNum() > 0) {
            while (outCount % ITEMS_PER_PAGE != 0) {
                pItems[outCount].mType = 3;
                outCount++;
            }

            while (outCount % ITEMS_PER_PAGE != 3) {
                pItems[outCount].mType = 3;
                outCount++;
            }

            for (s32 raceIdx = 0; raceIdx < 5; raceIdx++) {
                if (RaceManagerFunction::hasPowerStarRaceScenario(raceIdx)) {
                    pItems[outCount].mType = 1;
                    pItems[outCount].mRaceId = raceIdx;
                    outCount++;
                }
            }
        }

        return outCount;
    };
};  // namespace

namespace NrvPowerStarList {
    NEW_NERVE(PowerStarListNrvAppear, PowerStarList, Appear);
    NEW_NERVE(PowerStarListNrvWait, PowerStarList, Wait);
    NEW_NERVE(PowerStarListNrvDisappear, PowerStarList, Disappear);
    NEW_NERVE(PowerStarListNrvPageNext, PowerStarList, PageNext);
    NEW_NERVE(PowerStarListNrvPagePrev, PowerStarList, PagePrev);
    NEW_NERVE(PowerStarListNrvCaptureStart, PowerStarList, CaptureStart);
    NEW_NERVE(PowerStarListNrvCaptureConfirm, PowerStarList, CaptureConfirm);
    NEW_NERVE(PowerStarListNrvCaptureWait, PowerStarList, CaptureWait);
    NEW_NERVE(PowerStarListNrvCaptureSend, PowerStarList, CaptureSend);
};  // namespace NrvPowerStarList

PowerStarList::PowerStarList()
    : LayoutActor("全パワースターリスト", true), mArrowUpButtonCtrl(), mArrowDownButtonCtrl(), mCaptureButtonCtrl(), mSysInfoWindow(), mPageNo(),
      mMailMessageLength(), mMailMessage() {
    mSeparatorArray = new Separator[4]();
}

PowerStarList::Separator::Separator() {
}

void PowerStarList::init(const JMapInfoIter& rIter) {
    initLayoutManager("AllStarList", 1);
    MR::createAndAddPaneCtrl(this, "PicBG", 1);
    MR::createAndAddPaneCtrl(this, "List1", 1);
    MR::createAndAddPaneCtrl(this, "List2", 1);
    MR::createAndAddPaneCtrl(this, "Title", 1);
    MR::createAndAddPaneCtrl(this, "Belt", 1);

    const char* pPaneName;
    for (u32 i = 0; i < ARRAY_SIZE(::cSeparatorPaneTable); i++) {
        pPaneName = ::cSeparatorPaneTable[i];
        MR::createAndAddPaneCtrl(this, pPaneName, 1);
        MR::setFollowPos(&mSeparatorArray[i]._4, this, pPaneName);
        MR::setFollowTypeAdd(this, pPaneName);
    }

    MR::createAdaptorAndConnectToWiiMessageBoard("全パワースターリスト(伝言板用描画)", MR::Functor(this, &PowerStarList::drawForMessageBoardCapture));

    mArrowUpButtonCtrl = createButtonController("ArrowUpButton", "BoxButton1_00");
    mArrowDownButtonCtrl = createButtonController("ArrowDownButton", "BoxButton1_01");
    mCaptureButtonCtrl = createButtonController("Capture", "BoxTButton");
    mSysInfoWindow = MR::createSysInfoWindow();
    MR::connectToSceneLayoutOnPause(mSysInfoWindow);
    MR::setTextBoxGameMessageRecursive(this, "Title", "AllStarList_Title");

    ListItem pItems[MAX_ITEMS];
    mPageNum = (::getSortedList(pItems) - 1) / ITEMS_PER_PAGE + 1;

    MR::setTextBoxGameMessageRecursive(this, "PageNumber", "AllStarList_Page");
    MR::setTextBoxGameMessageRecursive(this, "Photo", "AllStarList_Page");
    MR::hidePaneRecursive(this, "Photo");
    MR::createOdhConverter();
    initNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));

    mMailMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(::cMailMessageID)) + FileSelectFunc::getMiiNameBufferSize();
    mMailMessage = new wchar_t[mMailMessageLength];
}

void PowerStarList::calcAnim() {
    LayoutActor::calcAnim();
    setSeparatorPaneSize();
}

void PowerStarList::appear() {
    updateList(0, true);
    startScrollAnimNext(true);
    LayoutActor::appear();
    setNerve(GET_NERVE(PowerStarList, PowerStarListNrvAppear));
}

void PowerStarList::drawForMessageBoardCapture() {
    MR::showPaneRecursive(this, "Photo");

    const char* pPaneName = "TxtPhotoPage";
    MR::setTextBoxArgNumberRecursive(this, pPaneName, mPageNo + 1, 0);
    MR::setTextBoxArgNumberRecursive(this, pPaneName, mPageNum, 1);

    wchar_t pictureFont[256];
    MR::addPictureFontTagPlayerIcon(pictureFont);
    MR::setTextBoxMessageRecursive(this, "BlosBase", pictureFont);

    setTotalPowerStarNumForMessageBoardCapture();

    nw4r::lyt::DrawInfo drawInfo;
    MR::copyLayoutDrawInfoWithAspect(&drawInfo, this, false);
    MR::calcAnimLayoutWithDrawInfo(this, drawInfo);

    setSeparatorPaneSize();
    MR::drawLayoutWithDrawInfoWithoutProjectionSetup(this, drawInfo);

    MR::hidePaneRecursive(this, "Photo");

    calcAnim();
}

bool PowerStarList::isAppearing() const {
    return isNerve(GET_NERVE(PowerStarList, PowerStarListNrvAppear));
}

bool PowerStarList::isExecCapture() const {
    if (MR::isDead(this)) {
        return false;
    }

    return isNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureStart)) || isNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureConfirm)) ||
           isNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureWait)) || isNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureSend));
}

void PowerStarList::control() {
    mArrowUpButtonCtrl->update();
    mArrowDownButtonCtrl->update();
    mCaptureButtonCtrl->update();

    if (mArrowUpButtonCtrl->isPointingTrigger() || mArrowDownButtonCtrl->isPointingTrigger() || mCaptureButtonCtrl->isPointingTrigger()) {
        MR::startSystemSE("SE_SY_GALAMAP_CURSOR_ON");
    }
}

ButtonPaneController* PowerStarList::createButtonController(const char* pParam1, const char* pParam2) {
    ButtonPaneController* pController = new ButtonPaneController(this, pParam1, pParam2, 0, true);

    pController->mWaitAnimName = "Wait";
    pController->invalidateAppearance();

    return pController;
}

void PowerStarList::resetButtonAll() {
    mArrowUpButtonCtrl->forceToWait();
    mArrowDownButtonCtrl->forceToWait();
    mCaptureButtonCtrl->forceToWait();
}

void PowerStarList::updateButtonAppearance() {
    if (mPageNum == 1) {
        mArrowDownButtonCtrl->_24 = false;
        mArrowUpButtonCtrl->_24 = false;

        MR::hidePane(this, "ArrowDownButton");
        MR::hidePane(this, "ArrowUpButton");
        MR::hidePane(this, "PageNumber");
    } else {
        MR::showPane(this, "ArrowDownButton");
        MR::showPane(this, "ArrowUpButton");
        MR::showPane(this, "PageNumber");

        if (mPageNo < mPageNum - 1) {
            mArrowDownButtonCtrl->_24 = true;
            MR::setPaneAlpha(this, "ArrowDownButton", -1);
        } else {
            mArrowDownButtonCtrl->_24 = false;
            MR::setPaneAlpha(this, "ArrowDownButton", 80);
        }

        if (mPageNo > 0) {
            mArrowUpButtonCtrl->_24 = true;
            MR::setPaneAlpha(this, "ArrowUpButton", -1);
        } else {
            mArrowUpButtonCtrl->_24 = false;
            MR::setPaneAlpha(this, "ArrowUpButton", 80);
        }

        const char* pPaneName = "PageNumber";
        MR::setTextBoxArgNumberRecursive(this, pPaneName, mPageNo + 1, 0);
        MR::setTextBoxArgNumberRecursive(this, pPaneName, mPageNum, 1);
    }
}

namespace {
    class TextBuffer {
    public:
        TextBuffer(LayoutActor* pActor, const char* const* pPaneNames, s32 rowsPerPane)
            : mActor(pActor), mPaneNames(pPaneNames), mRowsPerPane(rowsPerPane), mTail(mBuffer) {
            swprintf(mBuffer, ARRAY_SIZE(mBuffer), L"");
        }

        void addNewLine(s32 row) {
            s32 paneIndex = row / mRowsPerPane;

            if (row - paneIndex * mRowsPerPane != 0) {
                mTail = MR::addNewLine(mTail);
            }
        }

        void update(s32 row) {
            s32 paneIndex = row / mRowsPerPane;

            if (row - paneIndex * mRowsPerPane == 0) {
                MR::setTextBoxMessageRecursive(mActor, mPaneNames[paneIndex - 1], mBuffer);
                mTail = mBuffer;
                swprintf(mBuffer, ARRAY_SIZE(mBuffer), L"");
            }
        }

        void finish(s32 row) {
            s32 paneIndex = row / mRowsPerPane;

            if (row - paneIndex * mRowsPerPane == 0) {
                return;
            }

            MR::setTextBoxMessageRecursive(mActor, mPaneNames[paneIndex], mBuffer);
        }

        /* 0x000 */ LayoutActor* mActor;
        /* 0x004 */ const char* const* mPaneNames;
        /* 0x008 */ s32 mRowsPerPane;
        /* 0x00C */ wchar_t mBuffer[256];
        /* 0x20C */ wchar_t* mTail;
    };
};  // namespace

void PowerStarList::updateList(s32 pageNumber, bool isSinglePage) {
    if (pageNumber >= 0) {
        mPageNo = pageNumber;
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cGalaxyNamePaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cGalaxyNamePaneTable[idx], L"");
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cStarNumPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cStarNumPaneTable[idx], L"");
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cCoinPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cCoinPaneTable[idx], L"");
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cCrownPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cCrownPaneTable[idx], L"");
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cTimePaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cTimePaneTable[idx], L"");
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cSeparatorPaneTable); idx++) {
        MR::hidePaneRecursive(this, ::cSeparatorPaneTable[idx]);
    }

    ListItem listItems[MAX_ITEMS];

    s32 itemCount = ::getSortedList(listItems);
    mPageNum = (itemCount - 1) / ITEMS_PER_PAGE + 1;

    TextBuffer galaxyBuffer(this, ::cGalaxyNamePaneTable, 4);
    TextBuffer starBuffer(this, ::cStarNumPaneTable, 8);
    TextBuffer coinBuffer(this, ::cCoinPaneTable, 8);
    TextBuffer crownBuffer(this, ::cCrownPaneTable, 8);
    TextBuffer timeBuffer(this, ::cTimePaneTable, 8);

    ListItem* pEnd;
    bool isOdd = mPageNo % 2;
    s32 row;
    s32 rowLimit = isSinglePage ? 15 : 30;
    s32 bestTimeNum = ::getDisplayRaceBestTimeNum();
    ListItem* pItem = listItems + mPageNo * ITEMS_PER_PAGE;
    pEnd = listItems + itemCount;
    row = isOdd;
    s32 prevSeparator = -1;
    s32 galaxyCount;
    s32 separatorRows = 0;
    galaxyCount = 0;

    for (; pItem != pEnd; pItem++) {
        switch (pItem->mType) {
        case 0: {
            galaxyBuffer.mTail = makeGalaxyNameText(galaxyBuffer.mBuffer, &pItem->mGalaxyAccessor, row % 4 == 0);
            starBuffer.mTail = makeStarNumText(starBuffer.mTail, &pItem->mGalaxyAccessor, row % 8 == 0);
            coinBuffer.mTail = makeCoinText(coinBuffer.mTail, &pItem->mGalaxyAccessor, row % 8 == 0);
            crownBuffer.mTail = makeCrownText(crownBuffer.mTail, &pItem->mGalaxyAccessor, row % 8 == 0);
            timeBuffer.addNewLine(row);

            s32 priority = ::getSortPriority(&pItem->mGalaxyAccessor);
            s32 separator;
            if (priority <= 1) {
                separator = 0;
            } else {
                separator = priority <= 3 ? 1 : 2;
            }

            if (prevSeparator != separator) {
                if (prevSeparator >= 0) {
                    tryShowSeparator(prevSeparator, ::cSeparatorOffset * separatorRows);
                }

                TVec2f& rOffset = mSeparatorArray[separator]._4;
                rOffset.x = 0.0f;
                rOffset.y = ::cSeparatorOffset * row;
                prevSeparator = separator;
                separatorRows = 0;
            }

            separatorRows++;
            galaxyCount++;
            break;
        }
        case 1:
            galaxyBuffer.mTail = makeRaceNameText(galaxyBuffer.mBuffer, pItem->mRaceId, row % 4 == 0);
            timeBuffer.mTail = makeRaceTimeText(timeBuffer.mBuffer, pItem->mRaceId, row % 8 == 0);
            starBuffer.addNewLine(row);
            coinBuffer.addNewLine(row);
            crownBuffer.addNewLine(row);

            if (tryShowSeparator(3, ::cTimePaneDefaultHeight + ::cSeparatorOffset * (bestTimeNum - 1))) {
                TVec2f& rOffset = mSeparatorArray[3]._4;
                rOffset.x = 0.0f;
                rOffset.y = ::cSeparatorOffset * row;
            }

            break;
        case 3:
            galaxyBuffer.addNewLine(row);
            starBuffer.addNewLine(row);
            coinBuffer.addNewLine(row);
            crownBuffer.addNewLine(row);
            timeBuffer.addNewLine(row);
            break;
        }

        row++;
        galaxyBuffer.update(row);
        starBuffer.update(row);
        coinBuffer.update(row);
        crownBuffer.update(row);
        timeBuffer.update(row);

        if (rowLimit <= row - isOdd) {
            break;
        }
    }

    if (galaxyCount % ITEMS_PER_PAGE > 0) {
        separatorRows += ITEMS_PER_PAGE - galaxyCount % ITEMS_PER_PAGE;
    }

    tryShowSeparator(prevSeparator, ::cSeparatorOffset * separatorRows);
    galaxyBuffer.finish(row);
    starBuffer.finish(row);
    coinBuffer.finish(row);
    crownBuffer.finish(row);
    timeBuffer.finish(row);
}

wchar_t* PowerStarList::makeGalaxyNameText(wchar_t* pBuffer, const GalaxyStatusAccessor* pAccessor, bool isFirstLine) const {
    s32 offset;
    if (isFirstLine) {
        const wchar_t* pName = MR::getGalaxyNameShortOnCurrentLanguage(pAccessor->getName());
        offset = swprintf(pBuffer, 0x100, pName);
    } else {
        const wchar_t* pName = MR::getGalaxyNameShortOnCurrentLanguage(pAccessor->getName());
        offset = swprintf(pBuffer, 0x100, L"%ls%s%ls", pBuffer, "\n", pName);
    }

    return pBuffer + offset;
}

wchar_t* PowerStarList::makeStarNumText(wchar_t* pBuffer, const GalaxyStatusAccessor* pAccessor, bool isFirstLine) const {
    wchar_t* pText = pBuffer;
    if (!isFirstLine) {
        *pText = L'\n';
        pText++;
    }

    const char* pName = pAccessor->getName();
    for (s32 idx = 0; idx < pAccessor->getPowerStarNum(); idx++) {
        s32 starId = idx + 1;
        if (!MR::isOnGameEventFlagPowerStarSuccess(pName, starId)) {
            continue;
        } else if (GameDataConst::isGrandStar(pName, starId)) {
            *pText = L'K';
            pText++;
        } else if (GameDataConst::isPowerStarGreen(pName, starId)) {
            *pText = L'O';
            pText++;
        } else if (GameDataConst::isPowerStarRed(pName, starId)) {
            *pText = L'S';
            pText++;
        } else if (pAccessor->isCometStar(starId)) {
            *pText = L'e';
            pText++;
        } else {
            *pText = L'7';
            pText++;
        }
    }

    *pText = L'\0';
    return pText;
}

wchar_t* PowerStarList::makeCoinText(wchar_t* pBuffer, const GalaxyStatusAccessor* pAccessor, bool isFirstLine) const {
    if (!isFirstLine) {
        *pBuffer = L'\n';
        pBuffer++;
    }

    s32 coinScore = MR::getCoinBestScore(pAccessor->getName());

    pBuffer = MR::addPictureFontTag(pBuffer, L'A');

    *pBuffer = coinScore / 100 + L'0';

    coinScore -= coinScore / 100 * 100;
    s32 value = coinScore / 10;

    pBuffer[1] = value + L'0';
    coinScore -= 10 * value;
    pBuffer[2] = coinScore + L'0';
    pBuffer[3] = L'\0';

    return pBuffer + 3;
}

wchar_t* PowerStarList::makeCrownText(wchar_t* pBuffer, const GalaxyStatusAccessor* pAccessor, bool isFirstLine) const {
    if (!isFirstLine) {
        *pBuffer = L'\n';
        pBuffer++;
    }

    if (MR::isStarComplete(pAccessor->getName())) {
        *pBuffer = L'P';
        pBuffer++;
    }

    *pBuffer = L'\0';
    return pBuffer;
}

wchar_t* PowerStarList::makeRaceNameText(wchar_t* pBuffer, int raceId, bool isFirstLine) const {
    s32 offset;
    if (isFirstLine) {
        const wchar_t* pName = MR::getRaceNameOnCurrentLanguage(raceId);
        offset = swprintf(pBuffer, 0x100, pName);
    } else {
        const wchar_t* pName = MR::getRaceNameOnCurrentLanguage(raceId);
        offset = swprintf(pBuffer, 0x100, L"%ls%s%ls", pBuffer, "\n", pName);
    }

    return pBuffer + offset;
}

wchar_t* PowerStarList::makeRaceTimeText(wchar_t* pBuffer, int raceId, bool isFirstLine) const {
    wchar_t raceBest[10];
    MR::makeRaceBestTimeString(raceBest, raceId);

    s32 offset;
    if (isFirstLine) {
        offset = swprintf(pBuffer, 0x100, raceBest);
    } else {
        offset = swprintf(pBuffer, 0x100, L"%ls%s%ls", pBuffer, "\n", raceBest);
    }

    return pBuffer + offset;
}

void PowerStarList::startScrollAnimNext(bool set) {
    const char* pAnimName = mPageNo % 2 ? "TurnOverPage2" : "TurnOverPage1";

    MR::startPaneAnim(this, "List1", pAnimName, 0);
    MR::startPaneAnim(this, "List2", pAnimName, 0);
    MR::startPaneAnim(this, "PicBG", pAnimName, 0);
    MR::startPaneAnim(this, "Belt", pAnimName, 0);

    if (set) {
        MR::setPaneAnimFrameAndStop(this, "List1", 0.0f, 0);
        MR::setPaneAnimFrameAndStop(this, "List2", 0.0f, 0);
        MR::setPaneAnimFrameAndStop(this, "PicBG", 0.0f, 0);
        MR::setPaneAnimFrameAndStop(this, "Belt", 0.0f, 0);
    }
}

void PowerStarList::startScrollAnimPrev() {
    const char* pAnimName = mPageNo % 2 ? "TurnOverPage2" : "TurnOverPage1";

    MR::startPaneAnimReverseOneTime(this, "List1", pAnimName, 0);
    MR::startPaneAnimReverseOneTime(this, "List2", pAnimName, 0);
    MR::startPaneAnimReverseOneTime(this, "PicBG", pAnimName, 0);
    MR::startPaneAnimReverseOneTime(this, "Belt", pAnimName, 0);
}

bool PowerStarList::tryShowSeparator(s32 separatorIdx, f32 myFloat) {
    const char* pPaneName = ::cSeparatorPaneTable[separatorIdx];

    if (MR::isHiddenPane(this, pPaneName)) {
        MR::showPaneRecursive(this, pPaneName);
        mSeparatorArray[separatorIdx]._0 = myFloat;

        return true;
    }

    return false;
}

void PowerStarList::setSeparatorPaneSize() {
    const char* pPaneName;
    for (u32 idx = 0; idx < ARRAY_SIZE(::cSeparatorSizePaneTable); idx++) {
        pPaneName = ::cSeparatorSizePaneTable[idx];

        if (MR::isHiddenPane(this, pPaneName)) {
            continue;
        }

        nw4r::lyt::Pane* pPane = MR::getPane(this, pPaneName);
        nw4r::lyt::Size size = pPane->mSize;
        size.height = mSeparatorArray[idx]._0;
        pPane->mSize = size;
    }
}

void PowerStarList::setTotalPowerStarNumForMessageBoardCapture() {
    wchar_t message[256];
    wchar_t* pMessage;
    pMessage = MR::addPictureFontTag(MR::addPictureFontTag(message, L'7'), L'@');

    s32 powerStarNum = MR::getPowerStarNum();
    if (MR::getPowerStarNum() >= 100) {
        pMessage[0] = powerStarNum / 100 + L'0';
        pMessage++;
        powerStarNum -= powerStarNum / 100 * 100;
    }

    if (MR::getPowerStarNum() >= 10) {
        pMessage[0] = powerStarNum / 10 + L'0';
        pMessage++;
        powerStarNum -= powerStarNum / 10 * 10;
    }

    pMessage[0] = powerStarNum + L'0';
    pMessage[1] = L'\0';

    MR::setTextBoxMessageRecursive(this, "TxtStarTotal", message);
}

void PowerStarList::exeAppear() {
    ButtonPaneController* pController;

    if (MR::isFirstStep(this)) {
        pController = mArrowUpButtonCtrl;
        pController->appear();
        pController->_24 = false;

        pController = mArrowDownButtonCtrl;
        pController->appear();
        pController->_24 = false;

        pController = mCaptureButtonCtrl;
        pController->appear();
        pController->_24 = false;

        updateButtonAppearance();
        MR::startAnim(this, "Appear", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        mArrowUpButtonCtrl->_24 = true;
        mArrowDownButtonCtrl->_24 = true;
        mCaptureButtonCtrl->_24 = true;
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));
    }
}

void PowerStarList::exeWait() {
    if (MR::isFirstStep(this)) {
        startScrollAnimNext(true);
        updateList(-1, false);
        updateButtonAppearance();
    }

    if (mPageNo < mPageNum - 1 && (mArrowDownButtonCtrl->trySelect() || MR::testCorePadButtonDown(0) || MR::testSubPadStickTriggerDown(0))) {
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvPageNext));
    } else if (mPageNo > 0 && (mArrowUpButtonCtrl->trySelect() || MR::testCorePadButtonUp(0) || MR::testSubPadStickTriggerUp(0))) {
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvPagePrev));
    } else if (mCaptureButtonCtrl->trySelect()) {
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureStart));
    }
}

void PowerStarList::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mArrowUpButtonCtrl->_24 = false;
        mArrowDownButtonCtrl->_24 = false;
        mCaptureButtonCtrl->_24 = false;

        updateList(-1, true);

        MR::startAnim(this, mPageNo % 2 ? "End2" : "End", 0);
    }

    MR::killAtAnimStopped(this, 0);
}

void PowerStarList::exePageNext() {
    if (MR::isFirstStep(this)) {
        startScrollAnimNext(false);
        mPageNo++;
        MR::startSystemSE("SE_SY_GALAMAP_SCROLL");
    }

    if (MR::isPaneAnimStopped(this, "List1", 0) && MR::isPaneAnimStopped(this, "List2", 0)) {
        resetButtonAll();
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));
    }
}

void PowerStarList::exePagePrev() {
    if (MR::isFirstStep(this)) {
        updateList(mPageNo - 1, false);
        startScrollAnimPrev();
        MR::startSystemSE("SE_SY_GALAMAP_SCROLL");
    }

    if (MR::isPaneAnimStopped(this, "List1", 0) && MR::isPaneAnimStopped(this, "List2", 0)) {
        resetButtonAll();
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));
    }
}

void PowerStarList::exeCaptureStart() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_GALAMAP_CAPTURE");
    }

    if (mCaptureButtonCtrl->isDecidedWait()) {
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureConfirm));
    }
}

void PowerStarList::exeCaptureConfirm() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear("AllStarList_ConfirmCapture", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center,
                               SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindow)) {
        resetButtonAll();

        if (mSysInfoWindow->isSelectedYes()) {
            setNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureWait));
        } else {
            setNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));
        }
    }
}

void PowerStarList::exeCaptureWait() {
    if (MR::isFirstStep(this)) {
        MR::requestCaptureOdhImage();
    }

    if (MR::isRequestedCaptureOdhImage()) {
        return;
    }

    setNerve(GET_NERVE(PowerStarList, PowerStarListNrvCaptureSend));
}

void PowerStarList::exeCaptureSend() {
    if (MR::isFirstStep(this)) {
        const wchar_t* pUserName = GameDataFunction::getUserName();
        const wchar_t* pMessage = MR::getGameMessageDirect(::cMailMessageID);
        ReplaceTagFunction::ReplaceArgs(mMailMessage, mMailMessageLength, pMessage, pUserName);

        MR::SendMailObj sendMailObj = MR::SendMailObj("スターリスト");

        sendMailObj.setMessageDirect(mMailMessage);
        sendMailObj.setSenderID("WiiMessageTitle");
        sendMailObj.setImage(MR::getOdhImage(), MR::getOdhImageSize());
        sendMailObj.setBGDisable();
        sendMailObj.setLedOff();
        sendMailObj.send();
    }

    if (MR::isMailSent("スターリスト")) {
        MR::termMail("スターリスト");
        setNerve(GET_NERVE(PowerStarList, PowerStarListNrvWait));
    }
}
