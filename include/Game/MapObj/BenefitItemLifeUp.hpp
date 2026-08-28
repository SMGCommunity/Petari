#pragma once

#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemLifeUp : public BenefitItemObj {
public:
    BenefitItemLifeUp(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void initModelAndEfx();
    /* 0x88 */ virtual void exeCatch();
    /* 0x90 */ virtual void runEfx(const char*);
    /* 0x94 */ virtual void stopEfx(const char*);
};
