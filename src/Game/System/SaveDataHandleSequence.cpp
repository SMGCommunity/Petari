#include "Game/System/SaveDataHandleSequence.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/SaveIcon.hpp"
#include "Game/Screen/SysInfoWindow.hpp"
#include "Game/System/NANDErrorSequence.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/System/SaveDataHandler.hpp"
#include "Game/System/SysConfigFile.hpp"
#include "Game/System/UserFile.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <cstdio>

namespace {
    NEW_NERVE(SaveDataHandleSequenceNoOperation, SaveDataHandleSequence, NoOperation);
    NEW_NERVE(SaveDataHandleSequenceCheckEnableToCreate, SaveDataHandleSequence, CheckEnableToCreate);
    NEW_NERVE(SaveDataHandleSequenceSaveConfirm, SaveDataHandleSequence, SaveConfirm);
    NEW_NERVE(SaveDataHandleSequenceSave, SaveDataHandleSequence, Save);
    NEW_NERVE(SaveDataHandleSequenceSaveWindowDisappear, SaveDataHandleSequence, SaveWindowDisappear);
    NEW_NERVE(SaveDataHandleSequenceSaveDoneKeyWait, SaveDataHandleSequence, SaveDoneKeyWait);
    NEW_NERVE(SaveDataHandleSequenceSaveAllWithoutKeyWait, SaveDataHandleSequence, SaveAllWithoutKeyWait);
    NEW_NERVE(SaveDataHandleSequenceSaveAllWithoutKeyWaitDisappear, SaveDataHandleSequence, SaveAllWithoutKeyWaitDisappear);
    NEW_NERVE(SaveDataHandleSequenceSaveAllWithoutWindow, SaveDataHandleSequence, SaveAllWithoutWindow);
    NEW_NERVE(SaveDataHandleSequencePreLoad, SaveDataHandleSequence, PreLoad);
    NEW_NERVE(SaveDataHandleSequencePreLoadDone, SaveDataHandleSequence, PreLoadDone);
    NEW_NERVE(SaveDataHandleSequenceNoSaveConfirmRemind, SaveDataHandleSequence, NoSaveConfirmRemind);
    NEW_NERVE(SaveDataHandleSequenceErrorHandling, SaveDataHandleSequence, ErrorHandling);
};  // namespace

SaveDataHandleSequence::SaveDataHandleSequence()
    : NerveExecutor("セーブ/ロード"), mSysConfigFile(nullptr), mCurrentUserFile(nullptr), mBackupUserFile(nullptr), mSaveDataHandler(nullptr),
      mNANDErrorSequence(nullptr), mSysInfoWindowConfirm(nullptr), mSysInfoWindowSave(nullptr), _24(0), mIsConfirmRemind(false),
      mIsSaveAndQuitMsg(false), _2A(false), _2B(false), _2C(false), mWorkUserFile(nullptr), mNerveForError(nullptr), mTempBuffer(nullptr),
      mOnSaveSuccessFunc(nullptr), mJustBeforeSaveFunc(nullptr), mSaveIcon(nullptr) {
    mTempBuffer = new (32) u8[SaveDataHandler::getEnoughtTempBufferSize()];

    initNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

void SaveDataHandleSequence::initAfterResourceLoaded() {
    mSysConfigFile = new SysConfigFile();
    mCurrentUserFile = new UserFile();
    mBackupUserFile = new UserFile();
    mSaveDataHandler = new SaveDataHandler(mSysConfigFile, mCurrentUserFile);

    mSysInfoWindowConfirm = MR::createSysInfoWindowExecuteWithChildren();
    mSysInfoWindowConfirm->kill();

    mSysInfoWindowSave = MR::createSysInfoWindowMiniExecuteWithChildren();
    mSysInfoWindowSave->kill();

    mNANDErrorSequence = new NANDErrorSequence(this, mSysInfoWindowConfirm, mSysInfoWindowSave);

    mSaveIcon = new SaveIcon(mSysInfoWindowSave);
    mSaveIcon->kill();
}

void SaveDataHandleSequence::registerFunctorOnSaveSuccess(const MR::FunctorBase& rFunctor) {
    mOnSaveSuccessFunc = rFunctor.clone(nullptr);
}

void SaveDataHandleSequence::registerFunctorJustBeforeSave(const MR::FunctorBase& rFunctor) {
    mJustBeforeSaveFunc = rFunctor.clone(nullptr);
}

void SaveDataHandleSequence::update() {
    updateNerve();

    if (mSaveDataHandler != nullptr) {
        mSaveDataHandler->update();
    }

    if (mSysInfoWindowConfirm != nullptr) {
        mSysInfoWindowConfirm->movement();
        mSysInfoWindowConfirm->calcAnim();
    }

    if (mSysInfoWindowSave != nullptr) {
        mSysInfoWindowSave->movement();
        mSysInfoWindowSave->calcAnim();
        mSaveIcon->movement();
        mSaveIcon->calcAnim();
    }

    if (isEnablePointer()) {
        MR::requestStarPointerModeSaveLoad(this);
    }
}

void SaveDataHandleSequence::draw() const {
    if (mSysInfoWindowConfirm != nullptr) {
        mSysInfoWindowConfirm->draw();
    }

    if (mSysInfoWindowSave != nullptr) {
        mSysInfoWindowSave->draw();
        mSaveIcon->draw();
    }
}

void SaveDataHandleSequence::startPreLoad() {
    setNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoad));
}

void SaveDataHandleSequence::startCreateUserFile(int index) {
    restoreUserFile(mCurrentUserFile, index, true);
    mCurrentUserFile->resetAllData();
    mCurrentUserFile->setCreated();
    mCurrentUserFile->updateLastModified();
    mSaveDataHandler->initializeUserFileMemory(index, mCurrentUserFile);
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait));
}

void SaveDataHandleSequence::startDeleteUserFile(int index) {
    restoreUserFile(mCurrentUserFile, index, true);
    mCurrentUserFile->resetAllData();
    mSaveDataHandler->initializeUserFileMemory(index, mCurrentUserFile);
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait));
}

void SaveDataHandleSequence::startSave(bool isConfirmRemind, bool isSaveAndQuitMsg) {
    mIsConfirmRemind = isConfirmRemind;
    mIsSaveAndQuitMsg = isSaveAndQuitMsg;
    mWorkUserFile = mCurrentUserFile;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm));
}

void SaveDataHandleSequence::startSaveBackup(bool isConfirmRemind, bool isSaveAndQuitMsg) {
    mIsConfirmRemind = isConfirmRemind;
    mIsSaveAndQuitMsg = isSaveAndQuitMsg;
    mWorkUserFile = mBackupUserFile;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm));
}

void SaveDataHandleSequence::startSaveAll() {
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait));
}

void SaveDataHandleSequence::startSaveTotalMailSize() {
    mSaveDataHandler->storeSysConfigFile(mSysConfigFile);
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutWindow));
}

void SaveDataHandleSequence::startLoad(int userFileIndex, bool isPlayerMario) {
    restoreUserFile(mCurrentUserFile, userFileIndex, isPlayerMario);

    _24 = 2;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

void SaveDataHandleSequence::storeMiiOrIconId(int userFileIndex, const void* pMiiId, const u32* pIconId) {
    restoreUserFile(mCurrentUserFile, userFileIndex);
    mCurrentUserFile->setMiiOrIconId(pMiiId, pIconId);
    mCurrentUserFile->updateLastModified();
    mSaveDataHandler->storeUserFile(mCurrentUserFile);
}

void SaveDataHandleSequence::storeCopyUserFile(int indexDst, int indexSrc) {
    mSaveDataHandler->copyUserFileMemory(indexDst, indexSrc);
}

bool SaveDataHandleSequence::tryNANDErrorSequence(s32 code) {
    if (mNANDErrorSequence->tryNoRecoverErroSequence(code)) {
        mNerveForError = GET_NERVE_ANON(SaveDataHandleSequenceNoOperation);

        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling));

        return true;
    }

    return false;
}

bool SaveDataHandleSequence::isActive() const {
    return !isNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

bool SaveDataHandleSequence::isPermitToReset() const {
    if (mNANDErrorSequence != nullptr) {
        return mNANDErrorSequence->isPermitToReset();
    }

    return true;
}

void SaveDataHandleSequence::prepareReset() {
    if (mNANDErrorSequence != nullptr) {
        mNANDErrorSequence->prepareReset();
    }
}

bool SaveDataHandleSequence::isPreparedReset() const {
    if (mNANDErrorSequence != nullptr && !mNANDErrorSequence->isPreparedReset()) {
        return false;
    }

    if (isNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveDoneKeyWait)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoSaveConfirmRemind))) {
        return true;
    }

    if (isNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoad)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoadDone)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSave)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveWindowDisappear)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWaitDisappear)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutWindow))) {
        return false;
    }

    if (isNerve(GET_NERVE_ANON(SaveDataHandleSequenceCheckEnableToCreate)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling))) {
        return mNANDErrorSequence->isPreparedReset();
    }

    return false;
}

void SaveDataHandleSequence::restoreFromReset() {
    mNANDErrorSequence->restoreFromReset();

    if (!MR::isDead(mSysInfoWindowConfirm)) {
        mSysInfoWindowConfirm->forceKill();
    }

    if (!MR::isDead(mSysInfoWindowSave)) {
        mSysInfoWindowSave->forceKill();
    }

    mSaveIcon->kill();
    _24 = 3;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

bool SaveDataHandleSequence::isInitializedGameDataHolder() const {
    return mCurrentUserFile != nullptr;
}

void SaveDataHandleSequence::restoreUserFile(UserFile* pParam1, int index) {
    restoreUserFileConfigData(pParam1, index);
    restoreUserFileGameData(pParam1, index, pParam1->isLastLoadedMario());
}

void SaveDataHandleSequence::restoreUserFile(UserFile* pParam1, int index, bool isPlayerMario) {
    restoreUserFileConfigData(pParam1, index);
    restoreUserFileGameData(pParam1, index, isPlayerMario);
    pParam1->setLastLoadedMario(isPlayerMario);
}

void SaveDataHandleSequence::backupCurrentUserFile() {
    mCurrentUserFile->makeGameDataBinary(mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    mBackupUserFile->loadFromGameDataBinary(mCurrentUserFile->getGameDataName(), mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    mCurrentUserFile->makeConfigDataBinary(mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    mBackupUserFile->loadFromConfigDataBinary(mCurrentUserFile->getConfigDataName(), mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
}

void SaveDataHandleSequence::exeNoOperation() {
    mIsConfirmRemind = false;
    mIsSaveAndQuitMsg = false;
    _2B = false;
}

void SaveDataHandleSequence::exeCheckEnableToCreate() {
    if (MR::isFirstStep(this)) {
        mNANDErrorSequence->startCheckEnableToCreate();
    }

    mNANDErrorSequence->update();

    if (!mNANDErrorSequence->mIsDead) {
        return;
    }

    syncNoSaveFlagsFromErrorSequence();
    _24 = 2;
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

void SaveDataHandleSequence::exeSaveConfirm() {
    bool b = _2C || _2B;

    if (b) {
        mWorkUserFile->setCreated();
        mWorkUserFile->updateLastModified();
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSave));
    } else {
        bool isSelectedYes = false;
        const char* pSystemMessageId = "System_Save00";

        if (mIsSaveAndQuitMsg) {
            pSystemMessageId = "System_Save07";
        }

        if (!tryConfirm(pSystemMessageId, &isSelectedYes)) {
            return;
        }

        if (isSelectedYes) {
            mWorkUserFile->setCreated();
            mWorkUserFile->updateLastModified();
            setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSave));
        } else if (mIsConfirmRemind) {
            setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoSaveConfirmRemind));
        } else {
            setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));

            _24 = 3;
        }
    }
}

void SaveDataHandleSequence::exeSave() {
    if (MR::isFirstStep(this)) {
        if (mJustBeforeSaveFunc != nullptr) {
            (*mJustBeforeSaveFunc)();
        }

        mSaveDataHandler->storeSysConfigFile(mSysConfigFile);
        mSaveDataHandler->storeUserFile(mWorkUserFile);
    }

    if (tryNoSave()) {
        return;
    }

    if (_2A) {
        bool isErr = true;

        executeSaveFinish(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm));
    } else if (trySave()) {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveWindowDisappear));
    }
}

void SaveDataHandleSequence::exeSaveWindowDisappear() {
    bool isErr = false;

    if (!trySaveWindowDisappear(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm))) {
        return;
    }

    if (isErr) {
        return;
    }

    if (mOnSaveSuccessFunc != nullptr) {
        (*mOnSaveSuccessFunc)();
    }

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveDoneKeyWait));
}

void SaveDataHandleSequence::exeSaveDoneKeyWait() {
    if (!tryProcessDoneKeyWait("System_Save02")) {
        return;
    }

    _24 = 2;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

void SaveDataHandleSequence::exeSaveAllWithoutKeyWait() {
    if (MR::isFirstStep(this)) {
        if (mJustBeforeSaveFunc != nullptr) {
            (*mJustBeforeSaveFunc)();
        }

        mSaveDataHandler->storeSysConfigFile(mSysConfigFile);
    }

    if (tryNoSave()) {
        return;
    }

    if (_2A) {
        bool isErr = true;

        executeSaveFinish(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait));
    } else if (trySave()) {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWaitDisappear));
    }
}

void SaveDataHandleSequence::exeSaveAllWithoutKeyWaitDisappear() {
    bool isErr = false;

    if (!trySaveWindowDisappear(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait))) {
        return;
    }

    if (isErr) {
        return;
    }

    if (mOnSaveSuccessFunc != nullptr) {
        (*mOnSaveSuccessFunc)();
    }

    _24 = 2;

    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
}

void SaveDataHandleSequence::exeSaveAllWithoutWindow() {
    if (MR::isFirstStep(this)) {
        if (mJustBeforeSaveFunc != nullptr) {
            (*mJustBeforeSaveFunc)();
        }

        mSaveDataHandler->storeSysConfigFile(mSysConfigFile);
    }

    if (tryNoSave()) {
        return;
    }

    if (_2A) {
        bool isErr = true;

        executeSaveFinish(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutKeyWait));
    } else {
        bool isErr = false;

        if (!trySaveWithoutWindow(&isErr, GET_NERVE_ANON(SaveDataHandleSequenceSaveAllWithoutWindow))) {
            return;
        }

        if (isErr) {
            return;
        }

        if (mOnSaveSuccessFunc != nullptr) {
            (*mOnSaveSuccessFunc)();
        }

        _24 = 2;

        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
    }
}

void SaveDataHandleSequence::exePreLoad() {
    if (MR::isFirstStep(this)) {
        mSaveDataHandler->requestLoadSaveData();
    }

    if (!mSaveDataHandler->isDone()) {
        return;
    }

    NANDResultCode resultCode = mSaveDataHandler->getLastResultCode();

    if (resultCode.isSuccess()) {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoadDone));
    } else if (resultCode.isNoExistFile()) {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceCheckEnableToCreate));
    } else if (resultCode.isSaveDataCorrupted()) {
        mNerveForError = GET_NERVE_ANON(SaveDataHandleSequenceNoOperation);

        mNANDErrorSequence->startRemoveFile();
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling));
    } else if (tryNANDErrorSequence(resultCode.getCode())) {
        // FIXME: cmpwi instruction should not be optimized out.
    }
}

void SaveDataHandleSequence::exePreLoadDone() {
    if (!mSaveDataHandler->requestVerifyAfterLoadGameDataFile()) {
        mNerveForError = GET_NERVE_ANON(SaveDataHandleSequenceNoOperation);
        mNANDErrorSequence->startRemoveFile();

        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling));
    } else {
        restoreSysConfigFile(mSysConfigFile);
        _24 = 2;

        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
    }
}

void SaveDataHandleSequence::exeNoSaveConfirmRemind() {
    bool isSelectedYes = false;

    if (!tryConfirm("ConfirmEndGame", &isSelectedYes)) {
        return;
    }

    if (isSelectedYes) {
        _24 = 2;

        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));
    } else {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceSaveConfirm));
    }
}

void SaveDataHandleSequence::exeErrorHandling() {
    mNANDErrorSequence->update();

    if (!mNANDErrorSequence->mIsDead) {
        return;
    }

    syncNoSaveFlagsFromErrorSequence();
    setNerve(mNerveForError);

    mNerveForError = nullptr;
}

GameDataHolder* SaveDataHandleSequence::getHolder() {
    return mCurrentUserFile->mGameDataHolder;
}

SysConfigFile* SaveDataHandleSequence::getSysConfigFile() {
    return mSysConfigFile;
}

UserFile* SaveDataHandleSequence::getCurrentUserFile() {
    return mCurrentUserFile;
}

UserFile* SaveDataHandleSequence::getBackupUserFile() {
    return mBackupUserFile;
}

void SaveDataHandleSequence::restoreUserFileConfigData(UserFile* pParam1, int index) {
    char dataName[16];
    snprintf(dataName, sizeof(dataName), "config%1d", index);

    mSaveDataHandler->restoreGameDataFile(dataName, mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    pParam1->loadFromConfigDataBinary(dataName, mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
}

void SaveDataHandleSequence::restoreUserFileGameData(UserFile* pParam1, int index, bool isPlayerMario) {
    char dataName[16];
    snprintf(dataName, sizeof(dataName), "%s%1d", isPlayerMario ? "mario" : "luigi", index);

    mSaveDataHandler->restoreGameDataFile(dataName, mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    pParam1->loadFromGameDataBinary(dataName, mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());

    pParam1->mIsPlayerMario = isPlayerMario;
}

void SaveDataHandleSequence::restoreSysConfigFile(SysConfigFile* pSysConfigFile) {
    mSaveDataHandler->restoreGameDataFile("sysconf", mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
    pSysConfigFile->loadFromDataBinary(mTempBuffer, SaveDataHandler::getEnoughtTempBufferSize());
}

bool SaveDataHandleSequence::trySave() {
    if (MR::isFirstStep(this)) {
        mSaveDataHandler->requestSaveSaveData();
        mSysInfoWindowSave->appear("System_Save01", SysInfoWindow::Type_Blocking, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSaveIcon) && mSysInfoWindowSave->isWait()) {
        mSaveIcon->appear();
    }

    if (MR::isGreaterStep(this, 20)) {
        MR::startSystemLevelSE("SE_SY_LV_SAVING");
    }

    if (MR::isGreaterStep(this, 120) && mSaveDataHandler->isDone()) {
        return true;
    }

    return false;
}

bool SaveDataHandleSequence::trySaveWindowDisappear(bool* pIsErr, const Nerve* pNerveForError) {
    if (MR::isFirstStep(this)) {
        mSaveIcon->kill();
        mSysInfoWindowSave->disappear();
    }

    if (MR::isDead(mSysInfoWindowSave)) {
        return executeSaveFinish(pIsErr, pNerveForError);
    }

    return false;
}

bool SaveDataHandleSequence::trySaveWithoutWindow(bool* pIsErr, const Nerve* pNerveForError) {
    if (MR::isFirstStep(this)) {
        mSaveDataHandler->requestSaveSaveData();
    }

    if (mSaveDataHandler->isDone()) {
        return executeSaveFinish(pIsErr, pNerveForError);
    }

    return false;
}

bool SaveDataHandleSequence::tryConfirm(const char* pSystemMessageId, bool* pIsSelectedYes) {
    if (MR::isFirstStep(this)) {
        _24 = 1;

        mSysInfoWindowConfirm->appear(pSystemMessageId, SysInfoWindow::Type_YesNo, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
        MR::startSystemSE("SE_SY_SAVE_CONFIRM_INFO");
    }

    if (MR::isDead(mSysInfoWindowConfirm)) {
        *pIsSelectedYes = mSysInfoWindowConfirm->isSelectedYes();

        return true;
    }

    return false;
}

bool SaveDataHandleSequence::tryProcessDoneKeyWait(const char* pSystemMessageId) {
    if (MR::isFirstStep(this)) {
        mSysInfoWindowSave->appear(pSystemMessageId, SysInfoWindow::Type_Key, SysInfoWindow::TextPos_Center, SysInfoWindow::MessageType_System);
    }

    if (MR::isDead(mSysInfoWindowSave)) {
        return true;
    }

    return false;
}

bool SaveDataHandleSequence::tryNoSave() {
    bool b = _2C || _2B;

    if (b) {
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation));

        _24 = 2;

        return true;
    }

    return false;
}

bool SaveDataHandleSequence::isEnablePointer() const {
    if (isNerve(GET_NERVE_ANON(SaveDataHandleSequenceNoOperation)) || isNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoad)) ||
        isNerve(GET_NERVE_ANON(SaveDataHandleSequencePreLoadDone))) {
        return false;
    }

    if (isNerve(GET_NERVE_ANON(SaveDataHandleSequenceCheckEnableToCreate)) && !mNANDErrorSequence->isEnablePointer()) {
        return false;
    }

    return true;
}

bool SaveDataHandleSequence::executeSaveFinish(bool* pIsErr, const Nerve* pNerveForError) {
    NANDResultCode resultCode = mSaveDataHandler->getLastResultCode();

    if (_2A) {
        resultCode = NANDResultCode(NAND_RESULT_AUTHENTICATION);
    }

    if (resultCode.isSuccess()) {
        MR::startSystemSE("SE_SY_SAVE_SUCCESS");

        *pIsErr = false;
        _2A = false;

        return true;
    }

    *pIsErr = true;

    MR::startSystemSE("SE_SY_SAVE_FAILURE");

    if (resultCode.isSaveDataCorrupted()) {
        mNerveForError = pNerveForError;

        mNANDErrorSequence->startRemoveFile();
        setNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling));

        return true;
    }

    if (tryNANDErrorSequence(resultCode.getCode())) {
        return true;
    }

    mNerveForError = GET_NERVE_ANON(SaveDataHandleSequenceNoOperation);

    mNANDErrorSequence->startErrorToWiiMenu("NAND_11_2");
    setNerve(GET_NERVE_ANON(SaveDataHandleSequenceErrorHandling));

    return true;
}

void SaveDataHandleSequence::syncNoSaveFlagsFromErrorSequence() {
    _2C = mNANDErrorSequence->isResultNoSave();

    bool isResultFileCorrupted = mNANDErrorSequence->isResultFileCorrupted();

    _2A = isResultFileCorrupted;
    _2B = isResultFileCorrupted;
}
