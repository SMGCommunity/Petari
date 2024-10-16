#include "Game/MapObj/BenefitItemInvincible.hpp"
#include <cstring>

namespace {
    const f32 cAppearThrowFwdSpd = 5.0f;
    const f32 cAppearThrowUpSpd = 10.0f;
    const f32 cBoundUpSpd = 20.0f;
};

BenefitItemInvincible::BenefitItemInvincible(const char *pName) : BenefitItemObj(pName, "PowerupInvincible") {
    if (MR::isGalaxyDarkCometAppearInCurrentStage()) {
        _D8 = 1;   
    }
}

void BenefitItemInvincible::exeCatch() {
    MR::startSystemSE("SE_SY_POWER_UP_2", -1, -1);
    MR::hideModel(this);
    MR::changePlayerItemStatus(7);
    kill();
}

void BenefitItemInvincible::init(const JMapInfoIter &rIter) {
    BenefitItemObj::init(rIter);

    if (!_D8) {
        if (_DC) {
            MR::setShadowVolumeCylinderRadius(this, nullptr, 60.0f);
        }

        _13A = 0;
        TVec3f axis_z;
        MR::getRotatedAxisZ(&axis_z, mRotation);
        _148.set(axis_z);
        TVec3f axis_y;
        MR::getRotatedAxisY(&axis_y, mRotation);
        _13C.set(axis_y);
    }
}

void BenefitItemInvincible::initModelAndEfx() {
    initModelManagerWithAnm(_120, nullptr, false);
    initEffectKeeper(5, "BenefitItemObj", false);
    initBinder(60.0f, 60.0f, 0);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 60.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 4, 60.0f, offs);
    MR::startBrk(this, "PowerupInvincible");
}

/*
void BenefitItemInvincible::appearThrowUp() {
    if (!_DA) {
        BenefitItemObj::appearThrowUp();
    }
    else {
        MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
        setNerve(&NrvBenefitItemObj::HostTypeNrvShoot::sInstance);
        f32 appear = cAppearThrowFwdSpd;
        mVelocity = MR::addTwoScalarVecs(_148, _13C, appear, cAppearThrowUpSpd);
        MR::invalidateClipping(this);
    }
}
*/

// BenefitItemInvincible::initEscape
// BenefitItemInvincible::doEscape

void BenefitItemInvincible::runEfx(const char *pEffect) {
    char str[0x40];
    if (!strcmp(pEffect, "Glow")) {
        strcpy(str, pEffect);
        strcat(str, "PowerUpInvincible");
        MR::emitEffect(this, str);
    }
}

void BenefitItemInvincible::stopEfx(const char *pEffect) {
    char str[0x40];
    if (!strcmp(pEffect, "Glow")) {
        strcpy(str, pEffect);
        strcat(str, "PowerUpInvincible");
        MR::emitEffect(this, str);
    }
}

BenefitItemInvincible::~BenefitItemInvincible() {

}

void BenefitItemInvincible::runBck(const char *) {

}
