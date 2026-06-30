#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class AreaForm;
class StageSwitchCtrl;

class AreaObj : public NameObj {
public:
    AreaObj(int, const char*);

    virtual void init(const JMapInfoIter&);

    virtual bool isInVolume(const TVec3f&) const;

    virtual const char* getManagerName() const {
        return mName;
    }

    void onSwitchA();
    void offSwitchA();
    bool isOnSwitchA() const;
    bool isOnSwitchB() const;
    bool isValidSwitchA() const;
    bool isValidSwitchB() const;
    void setFollowMtx(const TPos3f*);
    TPos3f* getFollowMtx() const;

    void validate() {
        mIsValid = true;
    }

    void invalidate() {
        mIsValid = false;
    }

    bool isValid() const {
        return mIsValid && _15 && mIsAwake;
    }

    void awake() {
        mIsAwake = true;
    }

    void sleep() {
        mIsAwake = false;
    }

    /* 0x0C */ AreaForm* mForm;
    /* 0x10 */ int mFormType;
    /* 0x14 */ bool mIsValid;
    /* 0x15 */ bool _15;
    /* 0x16 */ bool mIsAwake;
    /* 0x18 */ s32 mObjArg0;
    /* 0x1C */ s32 mObjArg1;
    /* 0x20 */ s32 mObjArg2;
    /* 0x24 */ s32 mObjArg3;
    /* 0x28 */ s32 mObjArg4;
    /* 0x2C */ s32 mObjArg5;
    /* 0x30 */ s32 mObjArg6;
    /* 0x34 */ s32 mObjArg7;
    /* 0x38 */ StageSwitchCtrl* mSwitchCtrl;
};

class AreaObjMgr : public NameObj {
public:
    AreaObjMgr(s32, const char*);

    void entry(AreaObj*);
    AreaObj* find_in(const TVec3f&) const;

    AreaObj* getAreaObj(int index) {
        return mArray[index];
    }

    /* 0x0C */ MR::Vector< MR::AssignableArray< AreaObj* > > mArray;
    /* 0x18 */ s32 _18;
};
