#pragma once

#include "Game/NameObj/NameObj.hpp"

class WipeLayoutBase;

class WipeHolderBase : public NameObj {
public:
    WipeHolderBase(u32, const char*);

    virtual void setCurrent(const char*);
    virtual void wipe(const char*, s32);
    virtual void forceClose(const char*);
    virtual void forceOpen(const char*);
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;
    virtual bool isCurrent(const char*) const;
    virtual void addWipeLayout(WipeLayoutBase*);
    virtual WipeLayoutBase* getCurrent() const {
        return mCurrentWipeLayout;
    };
    virtual void updateWipe(const char*);

    WipeLayoutBase* findWipe(const char*) const;

private:
    /* 0x0C */ WipeLayoutBase* mCurrentWipeLayout;
    /* 0x10 */ WipeLayoutBase** mWipeLayoutArray;
    /* 0x14 */ u32 mWipeLayoutCapacity;
    /* 0x18 */ u32 mWipeLayoutCount;
};
