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
#include "Game/Util/Functor.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <nw4r/lyt/drawInfo.h>
#include <nw4r/lyt/pane.h>
// #include <wprintf>

extern int swprintf(wchar_t* s, size_t n, const wchar_t* format, ...);

#define MAX_PAGES 5
#define ITEMS_PER_PAGE 15
#define MAX_ITEMS MAX_PAGES* ITEMS_PER_PAGE

namespace {
    class ListItem {
    public:
        /// @brief Creates a new `ListItem`.
        ListItem() NO_INLINE : _0(-1), _4() {
        }

        /* 0x0 */ s32 _0;
        union {
            /* 0x4 */ GalaxyStatusAccessor* _4Accessor;
            /* 0x4 */ s32 _4;
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

    static const char* const cSeparatorSizeTable[] = {
        "PicBelt1",
        "PicBelt2",
        "PicBelt3",
        "WinBelt4",
    };

    const char* cMailMessageID = "WiiMessageStarList";

    const wchar_t cTemp = 0;

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

    s32 getSortedList(ListItem* pItems) {
        GalaxyStatusAccessor* galaxyAccessors[MAX_ITEMS];
        void* auStack_28[10];

        for (s32 idx = 0; idx < ARRAY_SIZE(galaxyAccessors); idx++) {
            galaxyAccessors[idx] = nullptr;
        }

        for (s32 idx = 0; idx < ARRAY_SIZE(auStack_28); idx++) {
            auStack_28[idx] = 0;
        }

        s32 availableGalaxies = 0;
        s32 byteOffset = 0;

        ScenarioDataIter iter = MR::makeBeginScenarioDataIter();

        for (; !iter.isEnd();) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();
            bool open = accessor.getPowerStarNum() == 0 ? false : MR::isOnGameEventFlagGalaxyOpen(accessor.getName());

            if (open) {
                *galaxyAccessors[availableGalaxies] = iter.makeAccessor();
                availableGalaxies++;
            }

            iter.goNext();
        }

        for (u32 idx = 0; idx != availableGalaxies; idx++) {
            for (u32 curIndex = idx; getSortPriority(galaxyAccessors[curIndex]) < getSortPriority(galaxyAccessors[curIndex - 1]); curIndex--) {
                GalaxyStatusAccessor* tmp = galaxyAccessors[curIndex];
                galaxyAccessors[curIndex] = galaxyAccessors[curIndex - 1];
                galaxyAccessors[curIndex - 1] = tmp;
            }
        }

        s32 outCount = 0;
        for (s32 readIndex = 0; availableGalaxies > 0; availableGalaxies--, outCount++) {
            pItems[outCount]._0 = 0;
            readIndex++;
            pItems[outCount]._4Accessor = galaxyAccessors[readIndex];
        }

        if (getDisplayRaceBestTimeNum() > 0) {
            while (outCount % ITEMS_PER_PAGE != 0) {
                pItems[outCount]._0 = 3;
                outCount++;
            }

            while (outCount % ITEMS_PER_PAGE != 3) {
                pItems[outCount]._0 = 3;
                outCount++;
            }

            for (s32 raceIdx = 0; raceIdx < 5; raceIdx++) {
                if (RaceManagerFunction::hasPowerStarRaceScenario(raceIdx)) {
                    pItems[outCount]._0 = 1;
                    pItems[outCount]._4 = raceIdx;
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
    : LayoutActor("全パワースターリスト", true), mArrowUpButtonCtrl(nullptr), mArrowDownButtonCtrl(nullptr), mCaptureButtonCtrl(nullptr),
      mSysInfoWindow(nullptr), mPageNo(0), mMailMessageLength(0), mMailMessage(nullptr) {
    mSeparatorArray = new Separator[4]();
}

void PowerStarList::init(const JMapInfoIter& rIter) {
    initLayoutManager("AllStarList", 1);
    MR::createAndAddPaneCtrl(this, "PicBG", 1);
    MR::createAndAddPaneCtrl(this, "List1", 1);
    MR::createAndAddPaneCtrl(this, "List2", 1);
    MR::createAndAddPaneCtrl(this, "Title", 1);
    MR::createAndAddPaneCtrl(this, "Belt", 1);

    for (u32 i = 0; i < ARRAY_SIZE(::cSeparatorPaneTable); i++) {
        const char* pPaneName = ::cSeparatorPaneTable[i];
        MR::createAndAddPaneCtrl(this, pPaneName, 1);
        MR::setFollowPos(&mSeparatorArray[i]._4, this, pPaneName);
        MR::setFollowTypeAdd(this, pPaneName);
    }

    MR::createAdaptorAndConnectToWiiMessageBoard("全パワースターリスト(伝言板用描画)",
                                                 MR::Functor_Inline(this, &PowerStarList::drawForMessageBoardCapture));

    mArrowUpButtonCtrl = createButtonController("ArrowUpButton", "BoxButton1_00");
    mArrowDownButtonCtrl = createButtonController("ArrowDownButton", "BoxButton1_01");
    mCaptureButtonCtrl = createButtonController("Capture", "BoxTButton");
    mSysInfoWindow = MR::createSysInfoWindow();
    MR::connectToSceneLayoutOnPause(mSysInfoWindow);
    MR::setTextBoxGameMessageRecursive(this, "Title", "AllStarList_Title");

    ListItem pItems[MAX_ITEMS];
    mPageNum = (getSortedList(pItems) - 1) / ITEMS_PER_PAGE + 1;

    MR::setTextBoxGameMessageRecursive(this, "PageNumber", "AllStarList_Page");
    MR::setTextBoxGameMessageRecursive(this, "Photo", "AllStarList_Page");
    MR::hidePaneRecursive(this, "Photo");
    MR::createOdhConverter();
    initNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);

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
    setNerve(&NrvPowerStarList::PowerStarListNrvAppear::sInstance);
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
    return isNerve(&NrvPowerStarList::PowerStarListNrvAppear::sInstance);
}

bool PowerStarList::isExecCapture() const {
    if (MR::isDead(this)) {
        return false;
    }

    return isNerve(&NrvPowerStarList::PowerStarListNrvCaptureStart::sInstance) ||
           isNerve(&NrvPowerStarList::PowerStarListNrvCaptureConfirm::sInstance) ||
           isNerve(&NrvPowerStarList::PowerStarListNrvCaptureWait::sInstance) || isNerve(&NrvPowerStarList::PowerStarListNrvCaptureSend::sInstance);
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
        TextBuffer(LayoutActor* pActor, const char* const* ppParam2, s32 param3) : mActor(pActor), _4(ppParam2), _8(param3), mTail(mBuffer) {
            swprintf(mBuffer, ARRAY_SIZE(mBuffer), L"");
        }

        void addNewLine(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 - v1 * _8 != 0) {
                mTail = MR::addNewLine(mTail);
            }
        }

        void update(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 - v1 * _8 == 0) {
                MR::setTextBoxMessageRecursive(mActor, _4[v1 - 1], mBuffer);
                mTail = mBuffer;
                swprintf(mBuffer, ARRAY_SIZE(mBuffer), L"");
            }
        }

        void finish(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 - v1 * _8 == 0) {
                return;
            }

            MR::setTextBoxMessageRecursive(mActor, _4[v1], mBuffer);
        }

    private:
        /* 0x000 */ LayoutActor* mActor;
        /* 0x004 */ const char* const* _4;
        /* 0x008 */ s32 _8;
        /* 0x00C */ wchar_t mBuffer[256];
        /* 0x20C */ wchar_t* mTail;
    };
};  // namespace

void PowerStarList::updateList(s32 pageNumber, bool myBool) {
    if (pageNumber >= 0) {
        mPageNo = pageNumber;
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cGalaxyNamePaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cGalaxyNamePaneTable[idx], &cTemp);
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cStarNumPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cStarNumPaneTable[idx], &cTemp);
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cCoinPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cCoinPaneTable[idx], &cTemp);
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cCrownPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cCrownPaneTable[idx], &cTemp);
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cSeparatorPaneTable); idx++) {
        MR::setTextBoxMessageRecursive(this, ::cSeparatorPaneTable[idx], &cTemp);
    }

    for (u32 idx = 0; idx < ARRAY_SIZE(::cSeparatorPaneTable); idx++) {
        MR::hidePaneRecursive(this, ::cSeparatorPaneTable[idx]);
    }

    ListItem listItems[MAX_ITEMS];

    s32 pages = (getSortedList(listItems) - 1) / ITEMS_PER_PAGE;
    mPageNum = pages + 1;

    TextBuffer galaxyBuffer(this, ::cGalaxyNamePaneTable, 4);
    TextBuffer starBuffer(this, ::cStarNumPaneTable, 8);
    TextBuffer coinBuffer(this, ::cCoinPaneTable, 8);
    TextBuffer crownBuffer(this, ::cCrownPaneTable, 8);
    TextBuffer timeBuffer(this, ::cTimePaneTable, 8);

    bool isEven = mPageNo % 2;
    s32 bestTimeNum = getDisplayRaceBestTimeNum();

    s32 prevValue = -1;
    s32 sortPriority;
    wchar_t* pNameText;

    for (s32 idx = 0; idx < mPageNo; idx++) {
        switch (listItems[idx]._0) {
        case 0:
            makeGalaxyNameText(pNameText, listItems[idx]._4Accessor, mPageNo % 4);
            makeStarNumText(pNameText, listItems[idx]._4Accessor, mPageNo % 8);
            makeCoinText(pNameText, listItems[idx]._4Accessor, mPageNo & 8);
            makeCrownText(pNameText, listItems[idx]._4Accessor, mPageNo & 8);

            timeBuffer.addNewLine(isEven);

            sortPriority = getSortPriority(listItems[idx]._4Accessor);
            s32 value;
            if (sortPriority < 2) {
                value = 0;
            } else {
                value = (3 - sortPriority) + 2;
            }

            if (value != prevValue) {
                if (prevValue >= 0) {
                    tryShowSeparator(idx, 20.0f * idx);
                }

                Separator separator = mSeparatorArray[idx];
                separator._4.x = 0.0f;
                separator._4.y = 20.0f * (isEven);
            }
            break;
        case 1:
            makeRaceNameText(pNameText, listItems[idx]._0, mPageNo % 4);
            makeRaceTimeText(pNameText, listItems[idx]._0, mPageNo % 8);

            starBuffer.addNewLine(mPageNo);
            coinBuffer.addNewLine(mPageNo);
            crownBuffer.addNewLine(mPageNo);

            if (tryShowSeparator(3, 72.0f + 20.0f * bestTimeNum)) {
                Separator separator = mSeparatorArray[idx];
                separator._4.x = 0.0f;
                separator._4.y = 20.0f * idx;
            }
            break;
        case 3:
            galaxyBuffer.addNewLine(mPageNo);
            starBuffer.addNewLine(mPageNo);
            coinBuffer.addNewLine(mPageNo);
            crownBuffer.addNewLine(mPageNo);
            timeBuffer.addNewLine(mPageNo);
            break;
        }

        mPageNo++;
        galaxyBuffer.update(mPageNo);
        starBuffer.update(mPageNo);
        coinBuffer.update(mPageNo);
        crownBuffer.update(mPageNo);
        timeBuffer.update(mPageNo);
    }

    tryShowSeparator(0, 20.0f);

    galaxyBuffer.finish(0);
    starBuffer.finish(0);
    coinBuffer.finish(0);
    crownBuffer.finish(0);
    timeBuffer.finish(0);
}

wchar_t* PowerStarList::makeGalaxyNameText(wchar_t* s, const GalaxyStatusAccessor* pAccessor, bool a1) const {
    s32 offset;
    if (a1) {
        offset = swprintf(s, 0x100, MR::getGalaxyNameShortOnCurrentLanguage(pAccessor->getName()));
    } else {
        offset = swprintf(s, 0x100, L"%ls%s%ls", s, L"\n", MR::getGalaxyNameShortOnCurrentLanguage(pAccessor->getName()));
    }
    return s + offset;
}

wchar_t* PowerStarList::makeStarNumText(wchar_t* s, const GalaxyStatusAccessor* pAccessor, bool a1) const {
    if (!a1) {
        *s = L'\n';
        s++;
    }

    s32 starId;
    const char* pName = pAccessor->getName();
    s32 idx;
    for (idx = 0; idx < pAccessor->getPowerStarNum(); idx++) {
        starId = idx + 1;
        if (!MR::isOnGameEventFlagPowerStarSuccess(pName, starId)) {
            continue;
        } else if (GameDataConst::isGrandStar(pName, starId)) {
            *s = L'K';
            s++;
        } else if (GameDataConst::isPowerStarGreen(pName, starId)) {
            *s = L'O';
            s++;
        } else if (GameDataConst::isPowerStarRed(pName, starId)) {
            *s = L'S';
            s++;
        } else if (pAccessor->isCometStar(starId)) {
            *s = L'e';
            s++;
        } else {
            *s = L'7';
            s++;
        }
    }

    *s = L'\0';
    return s;
}

wchar_t* PowerStarList::makeCoinText(wchar_t* s, const GalaxyStatusAccessor* pAccessor, bool a1) const {
    if (!a1) {
        *s = L'\n';
        s++;
    }

    s32 coinScore = MR::getCoinBestScore(pAccessor->getName());

    s = MR::addPictureFontTag(s, L'A');

    *s = coinScore / 100 + L'0';

    s32 value = (coinScore % 100) / 10;

    s[1] = value + L'0';
    s[2] = coinScore % 100 - 10 * value + L'0';
    s[3] = L'\0';

    return s + 3;
}

wchar_t* PowerStarList::makeCrownText(wchar_t* s, const GalaxyStatusAccessor* pAccessor, bool a1) const {
    if (!a1) {
        *s = L'\n';
        s++;
    }

    if (MR::isStarComplete(pAccessor->getName())) {
        *s = L'P';
        s++;
    }

    *s = L'\0';
    return s;
}

wchar_t* PowerStarList::makeRaceNameText(wchar_t* s, int raceId, bool a1) const {
    s32 offset;
    if (a1) {
        offset = swprintf(s, 0x100, MR::getRaceNameOnCurrentLanguage(raceId));
    } else {
        offset = swprintf(s, 0x100, L"%ls%s%ls", s, L"\n", MR::getRaceNameOnCurrentLanguage(raceId));
    }

    return s + offset;
}

wchar_t* PowerStarList::makeRaceTimeText(wchar_t* s, int raceId, bool a1) const {
    wchar_t raceBest[10];
    MR::makeRaceBestTimeString(raceBest, raceId);

    s32 offset;
    if (a1) {
        offset = swprintf(s, 0x100, raceBest);
    } else {
        offset = swprintf(s, 0x100, L"%ls%s%ls", s, L"\n", raceBest);
    }

    return s + offset;
}

void PowerStarList::startScrollAnimNext(bool set) {
    const char* pAnimName = "TurnOverPage2";

    if (mPageNo % 2) {
        pAnimName = "TurnOverPage1";
    }

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
    const char* pAnimName = "TurnOverPage2";

    if (mPageNo % 2) {
        pAnimName = "TurnOverPage1";
    }

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
    for (u32 idx = 0; idx < ARRAY_SIZE(::cSeparatorSizeTable); idx++) {
        pPaneName = ::cSeparatorSizeTable[idx];

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
        powerStarNum %= 100;
    }

    if (MR::getPowerStarNum() >= 10) {
        pMessage[0] = powerStarNum / 10 + L'0';
        pMessage++;
        powerStarNum %= 10;
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
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
    }
}

void PowerStarList::exeWait() {
    if (MR::isFirstStep(this)) {
        startScrollAnimNext(true);
        updateList(-1, false);
        updateButtonAppearance();
    }

    if (mPageNo < mPageNum - 1 && (mArrowDownButtonCtrl->trySelect() || MR::testCorePadButtonDown(0) || MR::testSubPadStickTriggerDown(0))) {
        setNerve(&NrvPowerStarList::PowerStarListNrvPageNext::sInstance);
    } else if (mPageNo > 0 && (mArrowUpButtonCtrl->trySelect() || MR::testCorePadButtonUp(0) || MR::testSubPadStickTriggerUp(0))) {
        setNerve(&NrvPowerStarList::PowerStarListNrvPagePrev::sInstance);
    } else if (mCaptureButtonCtrl->trySelect()) {
        setNerve(&NrvPowerStarList::PowerStarListNrvCaptureStart::sInstance);
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
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
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
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
    }
}

void PowerStarList::exeCaptureStart() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_GALAMAP_CAPTURE");
    }

    if (mCaptureButtonCtrl->isDecidedWait()) {
        setNerve(&NrvPowerStarList::PowerStarListNrvCaptureConfirm::sInstance);
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
            setNerve(&NrvPowerStarList::PowerStarListNrvCaptureWait::sInstance);
        } else {
            setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
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

    setNerve(&NrvPowerStarList::PowerStarListNrvCaptureSend::sInstance);
}

void PowerStarList::exeCaptureSend() {
    if (MR::isFirstStep(this)) {
        ReplaceTagFunction::ReplaceArgs(mMailMessage, mMailMessageLength, MR::getGameMessageDirect(::cMailMessageID),
                                        GameDataFunction::getUserName());

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
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
    }
}
