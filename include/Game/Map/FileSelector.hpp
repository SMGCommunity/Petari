#pragma once

#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include <RFLi_Types.h>
#include <nw4r/ut/ResFont.h>

class BackButton;
class BrosButton;
class FileSelectButton;
class FileSelectCameraController;
class FileSelectEffect;
class FileSelectInfo;
class FileSelectItem;
class FileSelectSky;
class InformationMessage;
class Manual2P;
class MiiConfirmIcon;
class MiiSelect;
class TitleSequenceProduct;
class UserFile;
class SysInfoWindow;

class FileSelector : public LiveActor {
public:
    FileSelector(const char*);

    virtual ~FileSelector();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void callbackStart();
    void callbackCopy();
    void callbackMii();
    void callbackDelete();
    void callbackManual();
    void notifyItem(FileSelectItem*, s32);
    void initUserFileArray();
    void createCameraController();
    void createSky();
    void createFileItems();
    void createOperationButton();
    void createBackButton();
    void createBrosButton();
    void createInfoMessage();
    void createSysInfoWindow();
    void createFileInfo();
    void createTitle();
    void createMiiSelect();
    void createMiiConfirmIcon();
    void createMiiFont();
    void createManual();
    void createSelectEffect();
    void appearAllItems();
    void disappaerAllLayout();
    bool isHiddenAllLayout() const;
    void updateFileInfo();
    void appearAllIndex();
    void disappearAllIndex();
    void invalidateSelectAll();
    void validateSelectAll();
    void initUserFile();
    void restoreUserFile();
    void checkAllComplete();
    void onPoint(FileSelectItem*);
    void onSelect(FileSelectItem*);
    void clearPointing();
    void setFileInfo(s32);
    bool checkSelectedBackButton();
    void goToNearPoint();
    void calcBasePos(f32);
    void initAllItems();
    void validateRotateAllItems();
    FileSelectIconID::EFellowID getUserFileFellowID(s32) const;
    bool isUserFileMiiidValid(s32) const;
    s32 getUserFileMiiIndex(s32) const;
    bool isUserFileCorrupted(s32) const;
    bool isUserFileAppearLuigi(s32) const;
    bool isUserFileLuigi(s32) const;
    void setUserFileMario(s32, bool);
    void storeSetMiiIdUserFile(s32, const FileSelectIconID&);
    void getMiiId(RFLCreateID*, const FileSelectIconID&);
    void getIconId(FileSelectIconID*, s32) const;
    s32 getMissCount(s32) const;
    void playSelectedME();
    void updateBgm();
    void exeTitle();
    void exeTitleEnd();
    void exeRFLError();
    void exeRFLWait();
    void exeRFLWaitEnd();
    void exeFileSelectStart();
    void exeFileSelect();
    void exeFileConfirmStart();
    void exeFileConfirmMiiDeleteWarningStart();
    void exeFileConfirmMiiDeleteWarning();
    void exeFileConfirmMiiDeleteSave();
    void exeFileConfirm();
    void exeDemoStartWait();
    void exeDemo();
    void exeCreateConfirmStart();
    void exeCreateConfirm();
    void exeCreate();
    void exeCopyWait();
    void exeCopySelect();
    void exeCopyConfirmStart();
    void exeCopyConfirm();
    void exeCopySave();
    void exeCopySaveMii();
    void exeCopyDemo();
    void exeCopyRejectStart();
    void exeCopyReject();
    void exeMiiWait();
    void exeMiiTip();
    void exeMiiSelectStart();
    void exeMiiSelect();
    void exeMiiCancel();
    void exeMiiConfirmWait();
    void exeMiiConfirm();
    void exeMiiCreateWait();
    void exeMiiCreateDemo();
    void exeMiiCaution();
    void exeMiiInfoStart();
    void exeMiiInfo();
    void exeDeleteConfirmStart();
    void exeDeleteConfirm();
    void exeDelete();
    void exeDeleteDemo();
    void exeFileBroken();
    void exeManualStart();
    void exeManual();

    FileSelectCameraController* mCameraController;  // 0x8C
    FileSelectSky* mSky;                            // 0x90
    DeriveActorGroup< FileSelectItem > mItems;      // 0x94
    TVec3f* _98;
    FileSelectButton* mSelectButton;    // 0x9C
    BackButton* mBackButton;            // 0xA0
    BrosButton* mBrosButton;            // 0xA4
    InformationMessage* mInfoMessage;   // 0xA8
    SysInfoWindow* mSysInfoWindow;      // 0xAC
    SysInfoWindow* mSysInfoWindowMini;  // 0xB0
    u32 _B4;
    u32 _B8;
    u32 _BC;
    u32 _C0;
    FileSelectInfo* mSelectInfo;  // 0xC4
    UserFile* mFiles;             // 0xC8
    void* _CC;
    TitleSequenceProduct* mTitleSeq;  // 0xD0
    MiiSelect* mMiiSelect;            // 0xD4
    MiiConfirmIcon* mMiiConirmIcon;   // 0xD8
    void* _DC;
    Manual2P* mManual2P;  // 0xE0
    u32 _E4;
    FileSelectEffect* mSelectEffects;  // 0xEC
    nw4r::ut::ResFont* mFont;          // 0xF0
};
