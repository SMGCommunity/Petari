#include "Game/System/NANDErrorSequence.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/System/SaveDataHandleSequence.hpp"
#include "Game/System/SaveDataHandler.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    NEW_NERVE(NANDErrorSequenceIdle, NANDErrorSequence, Idle);
    NEW_NERVE(NANDErrorSequenceError, NANDErrorSequence, Error);
    NEW_NERVE(NANDErrorSequenceErrorToFreeze, NANDErrorSequence, ErrorToFreeze);
    NEW_NERVE(NANDErrorSequenceRemoveConfirm, NANDErrorSequence, RemoveConfirm);
    NEW_NERVE(NANDErrorSequenceRemove, NANDErrorSequence, Remove);
    NEW_NERVE(NANDErrorSequenceRemoveDoneKeyWait, NANDErrorSequence, RemoveDoneKeyWait);
    NEW_NERVE(NANDErrorSequenceNoSaveConfirmBeforeRemove, NANDErrorSequence, NoSaveConfirmBeforeRemove);
    NEW_NERVE(NANDErrorSequenceNoSaveConfirmLessBlockOrInode, NANDErrorSequence, NoSaveConfirmLessBlockOrInode);
    NEW_NERVE(NANDErrorSequenceLessBlockOrInodeConfirm, NANDErrorSequence, LessBlockOrInodeConfirm);
    NEW_NERVE(NANDErrorSequenceGoWiiMenu, NANDErrorSequence, GoWiiMenu);
    NEW_NERVE(NANDErrorSequenceCheckEnableToCreateOnPreloading, NANDErrorSequence, CheckEnableToCreateOnPreloading);
    NEW_NERVE(NANDErrorSequenceCheckEnableToCreateAfterRemove, NANDErrorSequence, CheckEnableToCreateAfterRemove);
};  // namespace

NANDErrorSequence::NANDErrorSequence(SaveDataHandleSequence* pHost, SysInfoWindow* pParam2, SysInfoWindow*)
    : ActorStateBase("NANDError", pHost), mSystemMessageId(nullptr), _14(false), _15(false), mResult(0), mSysInfoWindow(pParam2) {
    initNerve(GET_NERVE_GLOBAL(NANDErrorSequenceIdle));
    kill();
    resetParam();
}

void NANDErrorSequence::startErrorToWiiMenu(const char* pSystemMessageId) {
    appear();
    resetParam();

    mSystemMessageId = pSystemMessageId;

    setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceError));
}

void NANDErrorSequence::startErrorToFreeze(const char* pSystemMessageId) {
    appear();
    resetParam();

    mSystemMessageId = pSystemMessageId;

    setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceErrorToFreeze));
}

void NANDErrorSequence::startRemoveFile() {
    appear();
    resetParam();

    _15 = false;

    setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveConfirm));
}

void NANDErrorSequence::startCheckEnableToCreate() {
    appear();
    resetParam();
    setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceCheckEnableToCreateOnPreloading));
}

bool NANDErrorSequence::tryLessBlockOrInode(u32 answer) {
    const char* pSystemMessageId = nullptr;

    mAnswer = answer;

    if ((mAnswer & (NAND_CHECK_HOME_INSINODE | NAND_CHECK_SYS_INSINODE)) != 0) {
        pSystemMessageId = "NAND_03";
    }

    if ((mAnswer & (NAND_CHECK_HOME_INSSPACE | NAND_CHECK_SYS_INSSPACE)) != 0) {
        pSystemMessageId = "NAND_02";
    }

    if (pSystemMessageId != nullptr) {
        appear();
        resetParam();

        mSystemMessageId = pSystemMessageId;

        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceLessBlockOrInodeConfirm));

        return true;
    }

    return false;
}

bool NANDErrorSequence::tryNoRecoverErroSequence(s32 code) {
    NANDResultCode resultCode(code);

    if (resultCode.isUnknown()) {
        startErrorToFreeze("NAND_11_2");
        return true;
    }

    if (resultCode.isBusyOrAllocFailed()) {
        startErrorToWiiMenu("NAND_11");
        return true;
    }

    if (resultCode.isNANDCorrupted()) {
        startErrorToFreeze("NAND_08");
        return true;
    }

    if (resultCode.isMaxBlocks()) {
        startErrorToWiiMenu("NAND_05");
        return true;
    }

    if (resultCode.isMaxFiles()) {
        startErrorToWiiMenu("NAND_06");
        return true;
    }

    return false;
}

bool NANDErrorSequence::isPermitToReset() const {
    return !isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceErrorToFreeze));
}

void NANDErrorSequence::prepareReset() {
    if (isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceError)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceErrorToFreeze))) {
        GameSystemFunction::setResetOperationReturnToMenu();
    }
}

bool NANDErrorSequence::isPreparedReset() const {
    if (mIsDead) {
        return true;
    }

    if (isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceIdle)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceError)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveConfirm)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveDoneKeyWait)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmBeforeRemove)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmLessBlockOrInode)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceLessBlockOrInodeConfirm)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceGoWiiMenu))) {
        return true;
    }

    return false;
}

void NANDErrorSequence::restoreFromReset() {
    mResult = 0;

    setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceIdle));
    kill();
}

void NANDErrorSequence::exeIdle() {
}

void NANDErrorSequence::exeError() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear(mSystemMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        GameSystemFunction::setResetOperationReturnToMenu();
    }

    if (MR::isDead(mSysInfoWindow)) {
        GameSystemFunction::requestGoWiiMenu(false);
    }
}

void NANDErrorSequence::exeErrorToFreeze() {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear(mSystemMessageId, SysInfoWindow::Type_Blocking, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (!mSysInfoWindow->isWait()) {
        return;
    }

    for (;;) {
    }
}

void NANDErrorSequence::exeRemoveConfirm() {
    bool isSelectedYes = false;

    if (!tryConfirm("NAND_07", &isSelectedYes)) {
        return;
    }

    if (isSelectedYes) {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemove));
    } else {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmBeforeRemove));
    }
}

void NANDErrorSequence::exeRemove() {
    if (MR::isFirstStep(this)) {
        mHost->mSaveDataHandler->requestRemoveSaveData();
    }

    if (!mHost->mSaveDataHandler->isDone()) {
        return;
    }

    NANDResultCode resultCode = mHost->mSaveDataHandler->getLastResultCode();

    if (resultCode.isSuccess() || resultCode.isNoExistFile()) {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveDoneKeyWait));
    } else if (tryNoRecoverErroSequence(resultCode.getCode())) {
        return;
    }
}

void NANDErrorSequence::exeRemoveDoneKeyWait() {
    if (tryProcessDoneKeyWait("System_Save05")) {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceCheckEnableToCreateAfterRemove));
    }
}

void NANDErrorSequence::exeNoSaveConfirmBeforeRemove() {
    bool isSelectedYes = false;

    if (!tryConfirm("System_Save04", &isSelectedYes)) {
        return;
    }

    if (isSelectedYes) {
        mResult = 3;

        kill();
    } else {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveConfirm));
    }
}

void NANDErrorSequence::exeNoSaveConfirmLessBlockOrInode() {
    bool isSelectedYes = false;

    if (!tryConfirm("System_Save04", &isSelectedYes)) {
        return;
    }

    if (isSelectedYes) {
        mResult = 2;

        kill();
    } else {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceLessBlockOrInodeConfirm));
    }
}

void NANDErrorSequence::exeLessBlockOrInodeConfirm() {
    bool isSelectedYes = false;

    if (!tryConfirm(mSystemMessageId, &isSelectedYes)) {
        return;
    }

    if (isSelectedYes) {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceGoWiiMenu));
    } else {
        setNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmLessBlockOrInode));
    }
}

void NANDErrorSequence::exeGoWiiMenu() {
    if (!MR::isDead(mSysInfoWindow)) {
        return;
    }

    mResult = 1;

    GameSystemFunction::requestGoWiiMenu(false);
    kill();
}

void NANDErrorSequence::exeCheckEnableToCreateOnPreloading() {
    if (!tryCheckEnableToCreate()) {
        return;
    }

    mResult = 1;

    kill();
}

void NANDErrorSequence::exeCheckEnableToCreateAfterRemove() {
    if (!tryCheckEnableToCreate()) {
        return;
    }

    mResult = 1;

    kill();
}

bool NANDErrorSequence::isEnablePointer() const {
    if (isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceError)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceErrorToFreeze)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveConfirm)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceRemoveDoneKeyWait)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmBeforeRemove)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceNoSaveConfirmLessBlockOrInode)) ||
        isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceLessBlockOrInodeConfirm)) || isNerve(GET_NERVE_GLOBAL(NANDErrorSequenceGoWiiMenu))) {
        return true;
    }

    return false;
}

bool NANDErrorSequence::isResultNoSave() const {
    return mResult == 2;
}

bool NANDErrorSequence::isResultFileCorrupted() const {
    return mResult == 3;
}

void NANDErrorSequence::control() {
}

void NANDErrorSequence::resetParam() {
    mSystemMessageId = nullptr;
    _14 = false;
    _15 = false;
    mAnswer = 0;
    mResult = 0;
}

bool NANDErrorSequence::tryConfirm(const char* pSystemMessageId, bool* pIsSelectedYes) {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear(pSystemMessageId, SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        MR::startSystemSE("SE_SY_SAVE_CONFIRM_INFO");
    }

    if (MR::isDead(mSysInfoWindow)) {
        *pIsSelectedYes = mSysInfoWindow->isSelectedYes();

        return true;
    }

    return false;
}

bool NANDErrorSequence::tryProcessDoneKeyWait(const char* pSystemMessageId) {
    if (MR::isFirstStep(this)) {
        mSysInfoWindow->appear(pSystemMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindow)) {
        return true;
    }

    return false;
}

bool NANDErrorSequence::tryCheckEnableToCreate() {
    if (MR::isFirstStep(this)) {
        mHost->mSaveDataHandler->requestCheckEnableToCreate();
    }

    if (mHost->mSaveDataHandler->isDone()) {
        NANDResultCode resultCode = mHost->mSaveDataHandler->getLastResultCode();

        if (resultCode.isSuccess()) {
            return !tryVerifyNANDCheckResult();
        }

        tryNoRecoverErroSequence(resultCode.getCode());

        return false;
    }

    return false;
}

bool NANDErrorSequence::tryVerifyNANDCheckResult() {
    return tryLessBlockOrInode(mHost->mSaveDataHandler->_10);
}
