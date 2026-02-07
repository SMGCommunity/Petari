#pragma once

#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/LiveActor.hpp"
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
