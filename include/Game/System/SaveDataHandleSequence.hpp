#pragma once

#include "Game/System/NerveExecutor.hpp"

class GameDataHolder;
class NANDErrorSequence;
class SaveDataHandler;
class SaveIcon;
class SysConfigFile;
class SysInfoWindow;
class UserFile;

namespace MR {
    class FunctorBase;
};

class SaveDataHandleSequence : public NerveExecutor {
public:
    /// @brief Creates a new `SaveDataHandleSequence`.
    SaveDataHandleSequence();

    void initAfterResourceLoaded();
    void registerFunctorOnSaveSuccess(const MR::FunctorBase&);
    void registerFunctorJustBeforeSave(const MR::FunctorBase&);
    void update();
    void draw() const;
    void startPreLoad();
    void startCreateUserFile(int);
    void startDeleteUserFile(int);
    void startSave(bool, bool);
    void startSaveBackup(bool, bool);
    void startSaveAll();
    void startSaveTotalMailSize();
    void startLoad(int, bool);
    void storeMiiOrIconId(int, const void*, const u32*);
    void storeCopyUserFile(int, int);
    bool tryNANDErrorSequence(s32);
    bool isActive() const;
    bool isPermitToReset() const;
    void prepareReset();
    bool isPreparedReset() const;
    void restoreFromReset();
    bool isInitializedGameDataHolder() const;
    void restoreUserFile(UserFile*, int);
    void restoreUserFile(UserFile*, int, bool);
    void backupCurrentUserFile();
    void exeNoOperation();
    void exeCheckEnableToCreate();
    void exeSaveConfirm();
    void exeSave();
    void exeSaveWindowDisappear();
    void exeSaveDoneKeyWait();
    void exeSaveAllWithoutKeyWait();
    void exeSaveAllWithoutKeyWaitDisappear();
    void exeSaveAllWithoutWindow();
    void exePreLoad();
    void exePreLoadDone();
    void exeNoSaveConfirmRemind();
    void exeErrorHandling();
    GameDataHolder* getHolder();
    SysConfigFile* getSysConfigFile();
    UserFile* getCurrentUserFile();
    UserFile* getBackupUserFile();
    void restoreUserFileConfigData(UserFile*, int);
    void restoreUserFileGameData(UserFile*, int, bool);
    void restoreSysConfigFile(SysConfigFile*);
    bool trySave();
    bool trySaveWindowDisappear(bool*, const Nerve*);
    bool trySaveWithoutWindow(bool*, const Nerve*);
    bool tryConfirm(const char*, bool*);
    bool tryProcessDoneKeyWait(const char*);
    bool tryNoSave();
    bool isEnablePointer() const;
    bool executeSaveFinish(bool*, const Nerve*);
    void syncNoSaveFlagsFromErrorSequence();

    /* 0x08 */ SysConfigFile* mSysConfigFile;
    /* 0x0C */ UserFile* mCurrentUserFile;
    /* 0x10 */ UserFile* mBackupUserFile;
    /* 0x14 */ SaveDataHandler* mSaveDataHandler;
    /* 0x18 */ NANDErrorSequence* mNANDErrorSequence;
    /* 0x1C */ SysInfoWindow* mSysInfoWindowConfirm;
    /* 0x20 */ SysInfoWindow* mSysInfoWindowSave;
    /* 0x24 */ s32 _24;
    /* 0x28 */ bool mIsConfirmRemind;
    /* 0x29 */ bool mIsSaveAndQuitMsg;
    /* 0x2A */ bool _2A;
    /* 0x2B */ bool _2B;
    /* 0x2C */ bool _2C;
    /* 0x30 */ UserFile* mWorkUserFile;
    /* 0x34 */ const Nerve* mNerveForError;
    /* 0x38 */ u8* mTempBuffer;
    /* 0x3C */ MR::FunctorBase* mOnSaveSuccessFunc;
    /* 0x40 */ MR::FunctorBase* mJustBeforeSaveFunc;
    /* 0x44 */ SaveIcon* mSaveIcon;
};
