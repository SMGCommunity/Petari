#pragma once

#include <revolution.h>

class TalkMessageCtrl;
class TalkBalloon;
class LiveActor;
class TalkSupportPlayerWatcher;
class IconAButton;
class LayoutActor;
class TalkBalloonShort;
class TalkBalloonEvent;
class TalkBalloonInfo;
class TalkBalloonSign;
class TalkBalloonCompose;

class TalkState {
public:
    TalkState();

    virtual void init(TalkMessageCtrl*, TalkBalloon*);
    virtual bool prep(const TalkMessageCtrl*) { return true; }

    virtual bool test() { return true; }

    virtual void open() {}

    virtual bool talk(const TalkMessageCtrl*) { return true; }

    virtual void clos() {}

    virtual bool term(const TalkMessageCtrl*) { return true; }

    virtual u32 getPageCount() const;

    void balloonOff() const;
    bool isLostMessage(const TalkMessageCtrl*) const;
    bool isSelfInterrupt(const TalkMessageCtrl*) const;

    TalkMessageCtrl* _04;
    /* 0x08 */ TalkBalloon* mBalloon;
    /* 0x0C */ u32 mMessageID;
};

class TalkStateShort : public TalkState {
public:
    TalkStateShort();

    virtual bool prep(const TalkMessageCtrl*);
    virtual void open();
    virtual bool talk(const TalkMessageCtrl*);
    virtual void clos();
};

class TalkStateEvent : public TalkState {
public:
    TalkStateEvent();

    virtual bool prep(const TalkMessageCtrl*);
    virtual bool test() NO_INLINE;
    virtual void open();
    virtual bool talk(const TalkMessageCtrl*);
    virtual void clos();
    virtual bool term(const TalkMessageCtrl*);
    virtual u32 getPageCount() const;

    s32 _10;
    s32 _14;
    TalkSupportPlayerWatcher* _18;
    u8 _1C;
    bool _1D;
    /* 0x20 */ u32 mPageCount;
};

class TalkStateNormal : public TalkStateEvent {
public:
    TalkStateNormal();

    virtual bool prep(const TalkMessageCtrl*);
    virtual bool test();
    virtual void clos();
    virtual bool term(const TalkMessageCtrl*);
    void updateButton();

    /* 0x24 */ IconAButton* mAButton;
};

class TalkStateCompose : public TalkStateNormal {
public:
    TalkStateCompose();

    virtual void init(TalkMessageCtrl*, TalkBalloon*);
    virtual bool prep(const TalkMessageCtrl*);
    virtual bool test();
    virtual void open();

    /* 0x28 */ TalkBalloon* mSecondBalloon;
};

class TalkStateHolder {
public:
    TalkStateHolder();

    void update();
    void pauseOff();
    TalkState* getState(const TalkMessageCtrl*);

    TalkSupportPlayerWatcher* _00;
    /* 0x04 */ IconAButton* mAButton;
    /* 0x08 */ TalkStateShort* mTalkShort;
    /* 0x0C */ TalkStateNormal* mTalkNormal;
    /* 0x10 */ TalkStateEvent* mTalkEvent;
    /* 0x14 */ TalkStateCompose* mTalkCompose;
    /* 0x18 */ TalkState* mTalk;
    /* 0x1C */ TalkBalloonShort* mBalloonShort;
};

namespace MR {
    void startTalkSound(unsigned char, const LiveActor*);
};
