#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"
#include "Game/Util.hpp"

class WipeFade : public WipeLayoutBase {
public:
    WipeFade(const char*, const Color8&);

    virtual ~WipeFade();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();
    virtual void wipe(s32);
    virtual void forceClose();
    virtual void forceOpen();
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;

    bool _20;
    s32 _24;
    s32 _28;
    Color8 _2C;
};