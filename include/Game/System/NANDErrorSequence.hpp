#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class SaveDataHandleSequence;
class SysInfoWindow;

class NANDErrorSequence : public ActorStateBase< SaveDataHandleSequence > {
public:
    NANDErrorSequence(SaveDataHandleSequence*, SysInfoWindow*, SysInfoWindow*);

    virtual void control();

    void startErrorToWiiMenu(const char*);
    void startErrorToFreeze(const char*);
    void startRemoveFile();
    void startCheckEnableToCreate();
    bool tryLessBlockOrInode(u32);
    bool tryNoRecoverErroSequence(s32);
    bool isPermitToReset() const;
    void prepareReset();
    bool isPreparedReset() const;
    void restoreFromReset();
    void exeIdle();
    void exeError();
    void exeErrorToFreeze();
    void exeRemoveConfirm();
    void exeRemove();
    void exeRemoveDoneKeyWait();
    void exeNoSaveConfirmBeforeRemove();
    void exeNoSaveConfirmLessBlockOrInode();
    void exeLessBlockOrInodeConfirm();
    void exeGoWiiMenu();
    void exeCheckEnableToCreateOnPreloading();
    void exeCheckEnableToCreateAfterRemove();
    bool isEnablePointer() const;
    bool isResultNoSave() const;
    bool isResultFileCorrupted() const;
    void resetParam();
    bool tryConfirm(const char*, bool*);
    bool tryProcessDoneKeyWait(const char*);
    bool tryCheckEnableToCreate();
    bool tryVerifyNANDCheckResult() NO_INLINE;

private:
    /* 0x0C */ SaveDataHandleSequence* mHost;
    /* 0x10 */ const char* mSystemMessageId;
    /* 0x14 */ bool _14;
    /* 0x15 */ bool _15;
    /* 0x18 */ u32 mAnswer;
    /* 0x1C */ u32 mResult;
    /* 0x20 */ SysInfoWindow* mSysInfoWindow;
};
