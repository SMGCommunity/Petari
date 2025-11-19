#pragma once

#include "Game/Screen/LayoutActor.hpp"

class TalkMessageCtrl;
class TalkTextFormer;
class JMapInfoIter;
class IconAButton;

class TalkBalloon : public LayoutActor {
public:
    TalkBalloon(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void kill();
    
    virtual void open(TalkMessageCtrl *);
    virtual void close();
    virtual void updateBalloon();
    virtual void updateTalking();
    virtual void pauseOff();
    virtual bool turnPage();
    virtual bool hasNextPage();
    virtual void skipMessage();
    virtual bool isTextAppearedAll();

    void create(const char*, bool, bool);

    TalkMessageCtrl *_20;
    TalkTextFormer *_24;
    bool _28;
    u8 _29;
};

class TalkBalloonShort : public TalkBalloon {
public:
    TalkBalloonShort(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void open(TalkMessageCtrl *);
    virtual void close();
    virtual void updateBalloon();

    void initInterval();
    void exeTalk();
    void exeClose();
    void exeOpen();

    u32 _2C;
};

class TalkBalloonEvent : public TalkBalloon {
public:
    TalkBalloonEvent(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void open(TalkMessageCtrl *);
    virtual void close();
    virtual void updateBalloon();
    virtual bool turnPage();
    virtual void skipMessage();
    virtual bool isTextAppearedAll();

    void initInterval();
    void exeTalk();
    void exeWait();
    void exeOpen();
    void updateBeak();
    void exeClose();

    u32 _2C;
    IconAButton *_30;
};

class TalkBalloonSign : public TalkBalloonEvent {
public:
    TalkBalloonSign(const char *);

    virtual void init(const JMapInfoIter &);
};

class TalkBalloonInfo : public TalkBalloon {
public:
    TalkBalloonInfo(const char *);
    ~TalkBalloonInfo();

    virtual void open(TalkMessageCtrl *);
    virtual void close();
    virtual void updateBalloon();
    virtual void updateTalking();
    virtual bool isTextAppearedAll();
};

class TalkBalloonIcon : public TalkBalloonShort {
public:
    TalkBalloonIcon(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void open(TalkMessageCtrl *);
};

class TalkBalloonHolder {
public:
    TalkBalloonHolder();

    void pauseOff();
    void skipMessage();
    TalkBalloon *getBalloon(const TalkMessageCtrl *);
    void balloonOff();
    void update();
    bool isActiveBalloonShort();

    TalkBalloonShort **mBalloonShortArray;

    TalkBalloonEvent *_04;
    TalkBalloonInfo *_08;
    TalkBalloonSign *_0C;
    TalkBalloonIcon *_10;
    s32 _14;
    IconAButton *_18;
};

namespace MR {
    void recoverSoundVolumeSetting(unsigned long);
    bool isCameraInterpolatingNearlyEnd();
}
