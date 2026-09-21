#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/FileSelectIconID.hpp"

struct RFLCreateID;

namespace nw4r {
    namespace ut {
        class ResFont;
    };  // namespace ut
};  // namespace nw4r

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

template < typename T >
class DeriveActorGroup;

class FileSelector : public LiveActor {
public:
    FileSelector(const char* pName);

    virtual ~FileSelector();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void callbackStart();
    void callbackCopy();
    void callbackMii();
    void callbackDelete();
    void callbackManual();
    void notifyItem(FileSelectItem* pItem, s32 msg);
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
    void disappearAllLayout();
    bool isHiddenAllLayout() const;
    void updateFileInfo();
    void appearAllIndex();
    void disappearAllIndex();
    void invalidateSelectAll();
    void validateSelectAll();
    void initUserFile();
    void restoreUserFile();
    void checkAllComplete();
    void onPoint(FileSelectItem* pItem);
    void onSelect(FileSelectItem* pItem);
    void clearPointing();
    void setFileInfo(s32 id);
    bool checkSelectedBackButton();
    void goToNearPoint();
    void calcBasePos(f32 offset);
    void initAllItems();
    void validateRotateAllItems();
    FileSelectIconID::EFellowID getUserFileFellowID(s32 id) const;
    bool isUserFileMiiIdValid(s32 id) const;
    u16 getUserFileMiiIndex(s32 id) const;
    bool isUserFileCorrupted(s32 id) const;
    bool isUserFileAppearLuigi(s32 id) const;
    bool isUserFileLuigi(s32 id) const;
    void setUserFileMario(s32 id, bool isMario) NO_INLINE;
    void storeSetMiiIdUserFile(s32 id, const FileSelectIconID& rIconId);
    void getMiiId(RFLCreateID* pCreateId, const FileSelectIconID& rIconId) const;
    void getIconId(FileSelectIconID* pIconId, s32 id) const;
    s32 getMissCount(s32 id) const;
    void playSelectedME();
    void updateBgm();
    void exeWaitBind();
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

    /* 0x8C */ FileSelectCameraController* mCameraController;
    /* 0x90 */ FileSelectSky* mSky;
    /* 0x94 */ DeriveActorGroup< FileSelectItem >* mItems;
    /* 0x98 */ TVec3f* _98;
    /* 0x9C */ FileSelectButton* mOperationButton;
    /* 0xA0 */ BackButton* mBackButton;
    /* 0xA4 */ BrosButton* mBrosButton;
    /* 0xA8 */ InformationMessage* mInfoMessage;
    /* 0xAC */ SysInfoWindow* mSysInfoWindow;
    /* 0xB0 */ SysInfoWindow* mSysInfoWindowMini;
    /* 0xB4 */ FileSelectItem* _B4;
    /* 0xB8 */ FileSelectItem* _B8;
    /* 0xBC */ FileSelectItem* _BC;
    /* 0xC0 */ FileSelectItem* _C0;
    /* 0xC4 */ FileSelectInfo* mFileInfo;
    /* 0xC8 */ UserFile* mUserFile;
    /* 0xCC */ bool* _CC;
    /* 0xD0 */ TitleSequenceProduct* mTitle;
    /* 0xD4 */ MiiSelect* mMiiSelect;
    /* 0xD8 */ MiiConfirmIcon* mMiiConfirmIcon;
    /* 0xDC */ RFLCreateID* mMiiId;
    /* 0xE0 */ Manual2P* mManual;
    /* 0xE4 */ bool mIsMiiSelectStartFirst;
    /* 0xE8 */ s32 mBgmState;
    /* 0xEC */ FileSelectEffect* mSelectEffect;
    /* 0xF0 */ nw4r::ut::ResFont* mFont;
};
