#pragma once

#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemLifeUp : public BenefitItemObj {
public:
    BenefitItemLifeUp(const char*);

    virtual ~BenefitItemLifeUp();
    virtual void init(const JMapInfoIter&);
    virtual void initModelAndEfx();
    virtual void exeCatch();
    virtual void runEfx(const char*);
    virtual void stopEfx(const char*);
};