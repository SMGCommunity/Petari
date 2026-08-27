#pragma once

#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemInvincible : public BenefitItemObj {
public:
    BenefitItemInvincible(const char*);

    virtual ~BenefitItemInvincible();
    virtual void init(const JMapInfoIter&);
    virtual void appearThrowUp();
    virtual void initModelAndEfx();
    virtual void exeCatch();
    virtual void runBck(const char*);
    virtual void runEfx(const char*);
    virtual void stopEfx(const char*);
    virtual void initEscape();
    virtual void doEscape();

    /* 0x13C */ TVec3f mUpAxis;
    /* 0x148 */ TVec3f mFwdAxis;
};
