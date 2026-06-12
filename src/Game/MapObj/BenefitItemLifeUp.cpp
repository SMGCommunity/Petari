#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include <cstring>

BenefitItemLifeUp::BenefitItemLifeUp(const char* pName) : BenefitItemObj(pName, "KinokoLifeUp") {
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
    MR::addHitSensorMapObj(this, "body", 4, 50.0f, TVec3f(0.0f, 50.0f, 0.0f));
}

void BenefitItemLifeUp::runEfx(const char* pName) {
    char str[0x40];
    strcpy(str, pName);
    strcat(str, "LifeUp");
    MR::emitEffect(this, str);
}

void BenefitItemLifeUp::init(const JMapInfoIter& rIter) {
    BenefitItemObj::init(rIter);
}

void BenefitItemLifeUp::stopEfx(const char* pName) {
    char str[0x40];
    strcpy(str, pName);
    strcat(str, "LifeUp");
    MR::emitEffect(this, str);
}

BenefitItemLifeUp::~BenefitItemLifeUp() {
}
