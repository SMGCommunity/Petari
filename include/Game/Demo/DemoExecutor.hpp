#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Util/Array.hpp"

class DemoActionKeeper;
class DemoCameraKeeper;
class DemoExecutor;
class DemoPlayerKeeper;
class DemoSoundKeeper;
class DemoSubPartInfo;
class DemoSubPartKeeper;
class DemoTalkAnimCtrl;
class DemoTimeKeeper;
class DemoWipeKeeper;
class StageSwitchCtrl;
class TalkMessageCtrl;

class DemoSheetKeeperBase {
public:
    virtual const char* getName() = 0;
    virtual const char* getTypeString() = 0;
    virtual void initCast(LiveActor*, const JMapInfoIter&) {};
    virtual void start() {};
    virtual void end() {};
    virtual void update() {};

    /* 0x04 */ DemoExecutor* mExecutor;
};

template < class T >
class DemoSheetKeeperInfoHolder {
public:
    virtual void executeType(T*);

    /* 0x00 */ MR::Vector< MR::AssignableArray< T > > mInfo;
};

struct DemoTalkMessageCtrl {
    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ TalkMessageCtrl* mTalkMessageCtrl;
};

class DemoExecutor : public DemoCastGroup {
public:
    /// @brief Creates a new `DemoExecutor`
    DemoExecutor(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    virtual void registerDemoActor(LiveActor*, const JMapInfoIter&);

    void start(NameObj*, const char*, s32);
    void startPart(NameObj*, const char*, const char*, s32);
    void startProperDemoSystem();
    void startDemoSystemPart(const char*, s32);
    bool tryStartProperDemoSystem();
    bool tryStartDemoSystemPart(const char*, s32);
    bool tryStartProperDemoSystemPart(const char*);
    void pause();
    void resume();
    void addTalkAnimCtrl(DemoTalkAnimCtrl*);
    void addTalkMessageCtrl(LiveActor*, TalkMessageCtrl*);
    TalkMessageCtrl* findTalkMessageCtrl(const LiveActor*) const;
    void setTalkMessageCtrl(const LiveActor*, TalkMessageCtrl*);
    void end();

    /* 0x014 */ const char* mSheetName;
    /* 0x018 */ DemoTimeKeeper* mTimeKeeper;
    /* 0x01C */ DemoSubPartKeeper* mSubPartKeeper;
    /* 0x020 */ DemoPlayerKeeper* mPlayerKeeper;
    /* 0x024 */ DemoCameraKeeper* mCameraKeeper;
    /* 0x028 */ DemoActionKeeper* mActionKeeper;
    /* 0x02C */ DemoWipeKeeper* mWipeKeeper;
    /* 0x030 */ DemoSoundKeeper* mSoundKeeper;
    /* 0x034 */ MR::Vector< MR::FixedArray< DemoSheetKeeperBase*, 2 > > mSheetKeeper;
    /* 0x040 */ StageSwitchCtrl* _40;
    /* 0x044 */ NameObj* _44;
    /* 0x048 */ const char* _48;
    /* 0x04C */ s32 _4C;
    /* 0x050 */ bool _50;
    /* 0x054 */ MR::Vector< MR::FixedArray< LiveActor*, 192 > > mActor;
    /* 0x358 */ MR::Vector< MR::FixedArray< DemoTalkAnimCtrl*, 8 > > mTalkAnimCtrl;
    /* 0x37C */ MR::Vector< MR::FixedArray< DemoTalkMessageCtrl, 8 > > mTalkMessageCtrl;
};
