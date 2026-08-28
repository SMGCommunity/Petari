#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class WipeLayoutBase;

class WipeHolderBase : public NameObj {
public:
    WipeHolderBase(u32, const char*);

    /* 0x24 */ virtual void setCurrent(const char*);
    /* 0x28 */ virtual void wipe(const char*, s32);
    /* 0x2C */ virtual void forceClose(const char*);
    /* 0x30 */ virtual void forceOpen(const char*);
    /* 0x34 */ virtual bool isOpen() const;
    /* 0x38 */ virtual bool isClose() const;
    /* 0x3C */ virtual bool isWipeIn() const;
    /* 0x40 */ virtual bool isWipeOut() const;
    /* 0x44 */ virtual bool isCurrent(const char*) const;
    /* 0x48 */ virtual void addWipeLayout(WipeLayoutBase*);
    /* 0x4C */ virtual WipeLayoutBase* getCurrent() const;
    /* 0x50 */ virtual void updateWipe(const char*);

    WipeLayoutBase* findWipe(const char*) const;

private:
    /* 0x0C */ WipeLayoutBase* mCurrentWipeLayout;
    /* 0x10 */ MR::Vector< MR::AssignableArray< WipeLayoutBase* > > mWipeLayoutArray;
};
