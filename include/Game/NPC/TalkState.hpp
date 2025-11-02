#pragma once

#include <revolution.h>

class TalkMessageCtrl;
class TalkBalloon;

class TalkState {
public:
    TalkState();

    virtual void init(TalkMessageCtrl *, TalkBalloon *);
    virtual bool prep(const TalkMessageCtrl *) {
        return true;
    }

    virtual bool test() {
        return true;
    }

    virtual void open() {

    }

    virtual bool talk(const TalkMessageCtrl *) {
        return true;
    }

    virtual void clos() {
        
    }

    virtual bool term(const TalkMessageCtrl *) {
        return true;
    }

    virtual u32 getPageCount() const;
    
    void balloonOff() const;
    bool isLostMessage(const TalkMessageCtrl*) const;
    bool isSelfInterrupt(const TalkMessageCtrl*) const;
    
    TalkMessageCtrl *_04;
    TalkBalloon *_08;
    u32 mMessageID;     // 0xC
};

class TalkStateShort : public TalkState {
public:
    TalkStateShort();

    virtual bool prep(const TalkMessageCtrl *);
    virtual void open();
    virtual bool talk(const TalkMessageCtrl *);
    virtual void clos();
};

class TalkStateEvent : public TalkState {
public:
    TalkStateEvent();

    virtual bool prep(const TalkMessageCtrl *);
    virtual bool test();
    virtual void open();
    virtual bool talk(const TalkMessageCtrl *);
    virtual void clos();
    virtual bool term(const TalkMessageCtrl *);
    virtual u32 getPageCount() const;

    s32 _10;
    s32 _14;
    u32 _18;
    u8 _1C;
    bool _1D;
    u32 mPageCount; // 0x20
};
