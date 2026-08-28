#pragma once

#include "Game/Screen/LayoutActor.hpp"

class TalkMessageCtrl;
class TalkTextFormer;
class JMapInfoIter;
class IconAButton;

class TalkBalloon : public LayoutActor {
public:
    TalkBalloon(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void kill();

    /* 0x30 */ virtual void open(TalkMessageCtrl*);
    /* 0x34 */ virtual void close();
    /* 0x38 */ virtual void updateBalloon();
    /* 0x3C */ virtual void updateTalking();
    /* 0x40 */ virtual void pauseOff();
    /* 0x44 */ virtual bool turnPage();
    /* 0x48 */ virtual bool hasNextPage();
    /* 0x4C */ virtual void skipMessage();
    /* 0x50 */ virtual bool isTextAppearedAll();

    void create(const char*, bool, bool);

    /* 0x20 */ TalkMessageCtrl* mMessageCtrl;
    /* 0x24 */ TalkTextFormer* mTextFormer;
    bool _28;
    bool _29;
};

class TalkBalloonShort : public TalkBalloon {
public:
    TalkBalloonShort(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void open(TalkMessageCtrl*);
    /* 0x34 */ virtual void close();
    /* 0x38 */ virtual void updateBalloon();

    void initInterval();
    void exeTalk();
    void exeClose();
    void exeOpen();

    u32 _2C;
};

class TalkBalloonEvent : public TalkBalloon {
public:
    TalkBalloonEvent(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void open(TalkMessageCtrl*);
    /* 0x34 */ virtual void close();
    /* 0x44 */ virtual bool turnPage();
    /* 0x4C */ virtual void skipMessage();
    /* 0x50 */ virtual bool isTextAppearedAll();

    void initInterval();
    void exeTalk();
    void exeWait();
    void exeOpen();
    void updateBeak();
    void exeClose();

    u32 _2C;
    /* 0x30 */ IconAButton* mAButton;
};

class TalkBalloonSign : public TalkBalloonEvent {
public:
    TalkBalloonSign(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class TalkBalloonInfo : public TalkBalloon {
public:
    TalkBalloonInfo(const char*);

    /* 0x30 */ virtual void open(TalkMessageCtrl*);
    /* 0x34 */ virtual void close();
    /* 0x38 */ virtual void updateBalloon();
    /* 0x3C */ virtual void updateTalking();
    /* 0x50 */ virtual bool isTextAppearedAll();
};

class TalkBalloonIcon : public TalkBalloonShort {
public:
    TalkBalloonIcon(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void open(TalkMessageCtrl*);
};

class TalkBalloonHolder {
public:
    TalkBalloonHolder();

    void pauseOff();
    void skipMessage();
    TalkBalloon* getBalloon(const TalkMessageCtrl*);
    void balloonOff();
    void update();
    bool isActiveBalloonShort() const;

    /* 0x00 */ TalkBalloonShort** mBalloonShortArray;
    /* 0x04 */ TalkBalloonEvent* mBalloonEvent;
    /* 0x08 */ TalkBalloonInfo* mBalloonInfo;
    /* 0x0C */ TalkBalloonSign* mBalloonSign;
    /* 0x10 */ TalkBalloonIcon* mBalloonIcon;
    s32 _14;
    /* 0x18 */ IconAButton* mAButton;
};

namespace MR {
    void recoverSoundVolumeSetting(u32);
    bool isCameraInterpolatingNearlyEnd();
};  // namespace MR
