#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <revolution/nand.h>

class NANDRequestInfo;
class NANDResultCode;

class SaveDataBannerCreator : public NerveExecutor {
public:
    /// @brief Creates a new `SaveDataBannerCreator`.
    SaveDataBannerCreator();

    void execute();
    bool isDone() const;
    NANDResultCode getResultCode() const;
    void exeNoOperation();
    void exeCreateOnTemporary();
    void exeMoveToHomeDir();
    void setupBannerInfo();

public:
    /* 0x08 */ NANDRequestInfo* mNANDRequestInfo;
    /* 0x0C */ NANDBanner* mBanner;
    /* 0x10 */ char mHomeDir[NAND_MAX_PATH];
};
