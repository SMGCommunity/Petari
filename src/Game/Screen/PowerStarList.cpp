#include "Game/Screen/PowerStarList.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/FileSelectFunc.hpp"
#include "Game/NWC24/NWC24Messenger.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/Screen/OdhConverter.hpp"
#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
// #include <wprintf>

extern int swprintf(wchar_t*, u32 n, const wchar_t*, ...);

namespace {
    class ListItem;

    const char* cGalaxyNamePaneTable[] = {
        "TxtStage1", "TxtStage2", "TxtStage3", "TxtStage4", "TxtStage5", "TxtStage6", "TxtStage7", "TxtStage8",
    };
    const char* cStarNumPaneTable[] = {
        "TxtStar1",
        "TxtStar2",
        "TxtStar3",
        "TxtStar4",
    };
    const char* cCoinPaneTable[] = {
        "TxtCoinPiece1",
        "TxtCoinPiece2",
        "TxtCoinPiece3",
        "TxtCoinPiece4",
    };
    const char* cCrownPaneTable[] = {
        "TxtComp1",
        "TxtComp2",
        "TxtComp3",
        "TxtComp4",
    };
    const char* cTimePaneTable[] = {
        "TxtTime1",
        "TxtTime2",
        "TxtTime3",
        "TxtTime4",
    };
    const char* cSeparatorPaneTable[] = {
        "PicBelt1",
        "PicBelt2",
        "PicBelt3",
        "PicBelt4",
    };
    const char* cSeparatorSizeTable[] = {
        "PicBelt1",
        "PicBelt2",
        "PicBelt3",
        "WinBelt4",
    };
    const char* cMailMessageID = "WiiMessageStarList";

    s32 getSortPriority(const GalaxyStatusAccessor*);
    s32 getDisplayRaceBestTimeNum();
    s32 getSortedList(ListItem*);
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
      _2C(nullptr), _30(0), mMailMessageLength(0), mMailMessage(nullptr) {
    mSeparatorArray = new Separator[4]();
}

void PowerStarList::init(const JMapInfoIter& rIter) {
    initLayoutManager("AllStarList", 1);
    MR::createAndAddPaneCtrl(this, "PicBG", 1);
    MR::createAndAddPaneCtrl(this, "List1", 1);
    MR::createAndAddPaneCtrl(this, "List2", 1);
    MR::createAndAddPaneCtrl(this, "Title", 1);
    MR::createAndAddPaneCtrl(this, "Belt", 1);

    for (s32 i = 0; i < sizeof(cSeparatorPaneTable) / sizeof(*cSeparatorPaneTable); i++) {
        MR::createAndAddPaneCtrl(this, cSeparatorPaneTable[i], 1);
        MR::setFollowPos(&mSeparatorArray[i]._4, this, cSeparatorPaneTable[i]);
        MR::setFollowTypeAdd(this, cSeparatorPaneTable[i]);
    }

    MR::createAdaptorAndConnectToWiiMessageBoard("全パワースターリスト(伝言板用描画)", MR::Functor_Inline(this, drawForMessageBoardCapture));

    mArrowUpButtonCtrl = createButtonController("ArrowUpButton", "BoxButton1_00");
    mArrowDownButtonCtrl = createButtonController("ArrowDownButton", "BoxButton1_01");
    mCaptureButtonCtrl = createButtonController("Capture", "BoxTButton");
    _2C = MR::createSysInfoWindow();
    MR::connectToSceneLayoutOnPause(_2C);
    MR::setTextBoxGameMessageRecursive(this, "Title", "AllStarList_Title");

    // ...

    MR::setTextBoxGameMessageRecursive(this, "PageNumber", "AllStarList_Page");
    MR::setTextBoxGameMessageRecursive(this, "Photo", "AllStarList_Page");
    MR::hidePaneRecursive(this, "Photo");
    MR::createOdhConverter();
    initNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);

    mMailMessageLength = MR::getStringLengthWithMessageTag(MR::getGameMessageDirect(cMailMessageID)) + FileSelectFunc::getMiiNameBufferSize();
    mMailMessage = new wchar_t[mMailMessageLength];
}

namespace {
    class ListItem {
    public:
        /// @brief Creates a new `ListItem`.
        ListItem() : _0(-1), _4(0) {}

    private:
        /* 0x00 */ s32 _0;
        /* 0x04 */ u32 _4;
    };
};  // namespace

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
        MR::startSystemSE("SE_SY_GALAMAP_CURSOR_ON", -1, -1);
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

namespace {
    class TextBuffer {
    public:
        TextBuffer(LayoutActor* pActor, const char* const* ppParam2, s32 param3) : mActor(pActor), _4(ppParam2), _8(param3), mTail(mBuffer) {
            swprintf(mBuffer, sizeof(mBuffer) / sizeof(*mBuffer), L"");
        }

        void addNewLine(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 != v1 * _8) {
                mTail = MR::addNewLine(mTail);
            }
        }

        void update(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 == v1 * _8) {
                MR::setTextBoxMessageRecursive(mActor, _4[v1 - 1], mBuffer);
                mTail = mBuffer;
                swprintf(mBuffer, sizeof(mBuffer) / sizeof(*mBuffer), L"");
            }
        }

        void finish(s32 param1) {
            s32 v1 = param1 / _8;

            if (param1 != v1 * _8) {
                MR::setTextBoxMessageRecursive(mActor, _4[v1], mBuffer);
            }
        }

    private:
        /* 0x000 */ LayoutActor* mActor;
        /* 0x004 */ const char* const* _4;
        /* 0x008 */ s32 _8;
        /* 0x00C */ wchar_t mBuffer[256];
        /* 0x20C */ wchar_t* mTail;
    };
};  // namespace

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

    if (_30 < _34 - 1 && (mArrowDownButtonCtrl->trySelect() || MR::testCorePadButtonDown(0) || MR::testSubPadStickTriggerDown(0))) {
        setNerve(&NrvPowerStarList::PowerStarListNrvPageNext::sInstance);
    } else if (_30 > 0 && (mArrowUpButtonCtrl->trySelect() || MR::testCorePadButtonUp(0) || MR::testSubPadStickTriggerUp(0))) {
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

        MR::startAnim(this, _30 == 0 ? "End2" : "End", 0);
    }

    MR::killAtAnimStopped(this, 0);
}

void PowerStarList::exePageNext() {
    if (MR::isFirstStep(this)) {
        startScrollAnimNext(false);
        _30++;
        MR::startSystemSE("SE_SY_GALAMAP_SCROLL", -1, -1);
    }

    if (MR::isPaneAnimStopped(this, "List1", 0) && MR::isPaneAnimStopped(this, "List2", 0)) {
        resetButtonAll();
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
    }
}

void PowerStarList::exePagePrev() {
    if (MR::isFirstStep(this)) {
        updateList(_30 - 1, false);
        startScrollAnimPrev();
        MR::startSystemSE("SE_SY_GALAMAP_SCROLL", -1, -1);
    }

    if (MR::isPaneAnimStopped(this, "List1", 0) && MR::isPaneAnimStopped(this, "List2", 0)) {
        resetButtonAll();
        setNerve(&NrvPowerStarList::PowerStarListNrvWait::sInstance);
    }
}

void PowerStarList::exeCaptureStart() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_GALAMAP_CAPTURE", -1, -1);
    }

    if (mCaptureButtonCtrl->isDecidedWait()) {
        setNerve(&NrvPowerStarList::PowerStarListNrvCaptureConfirm::sInstance);
    }
}

void PowerStarList::exeCaptureConfirm() {
    if (MR::isFirstStep(this)) {
        _2C->appear("AllStarList_ConfirmCapture", SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(_2C)) {
        resetButtonAll();

        if (_2C->isSelectedYes()) {
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
        ReplaceTagFunction::ReplaceArgs(mMailMessage, mMailMessageLength, MR::getGameMessageDirect(cMailMessageID), GameDataFunction::getUserName());

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
