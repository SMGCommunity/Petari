#pragma once

#include "Game/System/NerveExecutor.hpp"

class NANDRequestInfo;
class NANDResultCode;
class SaveDataBannerCreator;
class SysConfigFile;
class UserFile;

class SaveDataHandler : public NerveExecutor {
public:
    SaveDataHandler(const SysConfigFile*, const UserFile*);

    void update();
    void requestCheckEnableToCreate();
    void requestLoadSaveData();
    bool requestVerifyAfterLoadGameDataFile();
    void initializeUserFileMemory(int, const UserFile*);
    void copyUserFileMemory(int, int);
    void restoreGameDataFile(const char*, void*, u32);
    void storeUserFile(const UserFile*);
    void storeSysConfigFile(const SysConfigFile*);
    void requestSaveSaveData();
    void requestRemoveSaveData();
    static u32 getEnoughtTempBufferSize();
    bool isDone() const;
    NANDResultCode getLastResultCode() const;
    void exeWait();
    void exeProcessing();
    void exeSaveProcessingGameData();
    void exeSaveProcessingBanner();
    void exeRemoveProcessingBanner();
    void exeRemoveProcessingGameData();
    static void resetSaveData(u8*);
    static void initializeAllFileInSaveData(u8*, const SysConfigFile*, const UserFile*);
    static bool isCorrectFileHeader(const u8*);
    static void copySaveDataEachFile(u8*, const u8*);
    void createCommunicationBuffer();
    bool tryRemoveFile(const char*, bool*);
    bool trySave(bool*, bool);

    /* 0x08 */ NANDRequestInfo* mNANDRequestInfo;
    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u8* _14;
    /* 0x18 */ u8* _18;
    /* 0x1C */ SaveDataBannerCreator* mBannerCreator;
};
