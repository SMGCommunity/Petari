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
    
    u32 mMessageID;     // _C
};