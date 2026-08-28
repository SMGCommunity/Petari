#pragma once

#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemInvincible : public BenefitItemObj {
public:
    BenefitItemInvincible(const char*);

    /* 0x08 */ virtual ~BenefitItemInvincible();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x7C */ virtual void appearThrowUp();
    /* 0x84 */ virtual void initModelAndEfx();
    /* 0x88 */ virtual void exeCatch();
    /* 0x8C */ virtual void runBck(const char*);
    /* 0x90 */ virtual void runEfx(const char*);
    /* 0x94 */ virtual void stopEfx(const char*);
    /* 0x9C */ virtual void initEscape();
    /* 0xA0 */ virtual void doEscape();

    /* 0x13C */ TVec3f mUpAxis;
    /* 0x148 */ TVec3f mFwdAxis;
};
