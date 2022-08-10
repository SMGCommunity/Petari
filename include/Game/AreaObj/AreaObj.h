#pragma once

#include "revolution.h"

#include "Game/AreaObj/AreaForm.h"
#include "Game/NameObj/NameObj.h"
#include "Game/Map/StageSwitch.h"
#include "Game/Util/Array.h"

class AreaObj : public NameObj {
public:
    AreaObj(int, const char *);
    virtual ~AreaObj() {

    }

    virtual void init(const JMapInfoIter &);

    virtual bool isInVolume(const TVec3f &) const;
    virtual const char* getManagerName() const;

    void onSwitchA();
    void offSwitchA();
    bool isOnSwitchA() const;
    bool isOnSwitchB() const;
    bool isValidSwitchA() const;
    bool isValidSwitchB() const;
    void setFollowMtx(const TPos3f *);
    TPos3f* getFollowMtx() const;

    void validate();
    void invalidate();

    bool isValid() const;

    void awake();
    void sleep();

    AreaForm* mForm;              // _C
    int mType;                    // _10
    bool mValid;                  // _14
    bool _15;
    bool mAwake;                  // _16
    s32 mObjArg0;                 // _18
    s32 mObjArg1;                 // _1C
    s32 mObjArg2;                 // _20
    s32 mObjArg3;                 // _24
    s32 mObjArg4;                 // _28
    s32 mObjArg5;                 // _2C
    s32 mObjArg6;                 // _30
    s32 mObjArg7;                 // _34
    StageSwitchCtrl* mSwitchCtrl; // _38
};

class AreaObjMgr : public NameObj {
public:
    AreaObjMgr(s32, const char *);

    virtual ~AreaObjMgr();

    void entry(AreaObj *);
    AreaObj* find_in(const TVec3f &) const;

    MR::Vector<MR::AssignableArray<AreaObj*> > mArray; // _C
    s32 _18;
};
