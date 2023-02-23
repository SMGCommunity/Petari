#include "Game/MapObj/BenefitItemLifeUp.h"
#include <cstring>

BenefitItemLifeUp::BenefitItemLifeUp(const char *pName) : BenefitItemObj(pName, "KinokoLifeUp") {
    if (MR::isGalaxyDarkCometAppearInCurrentStage()) {
        _D8 = 1;
    }
}

void BenefitItemLifeUp::exeCatch() {
    MR::hideModel(this);
    MR::changePlayerItemStatus(0xA);
    runEfx("Get");
    kill();
}

void BenefitItemLifeUp::initModelAndEfx() {
    initModelManagerWithAnm(_120, nullptr, false);
    initEffectKeeper(5, "BenefitItemObj", false);
    initBinder(50.0f, 50.0f, 0);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 50.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 4, 50.0f, offs);
}

void BenefitItemLifeUp::runEfx(const char *pName) {
    char str[0x40];
    strcpy(str, pName);
    strcat(str, "LifeUp");
    MR::emitEffect(this, str);
}

namespace NrvBenefitItemObj {
    INIT_NERVE(HostTypeNrvWait);
    INIT_NERVE(HostTypeNrvShoot);
    INIT_NERVE(HostTypeNrvCatch);
    INIT_NERVE(HostTypeNrvAppearGround);
    INIT_NERVE(HostTypeNrvPreEscape);
    INIT_NERVE(HostTypeNrvEscape);
};

void BenefitItemLifeUp::init(const JMapInfoIter &rIter) {
    BenefitItemObj::init(rIter);
}

void BenefitItemLifeUp::stopEfx(const char *pName) {
    char str[0x40];
    strcpy(str, pName);
    strcat(str, "LifeUp");
    MR::emitEffect(this, str);
}

BenefitItemLifeUp::~BenefitItemLifeUp() {
    
}