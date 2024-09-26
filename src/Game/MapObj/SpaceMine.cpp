#include "Game/MapObj/SpaceMine.hpp"

SpaceMine::SpaceMine(const char *pName) : MapObjActor(pName), mClippingRange(gZeroVec) {
    _C4 = 1.0f;
    _D4 = -1;
    _D8 = 0;
}

void SpaceMine::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(2);
    info.setupNerve(&NrvSpaceMine::HostTypeWait::sInstance);
    info.setupRailMover();
    info.setupHitSensor();
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    info.setupHitSensorParam(8, 100.0f, sensorOffs);
    info.setupGroupClipping(0x10);
    MR::getJMapInfoArg0NoInit(rIter, &_D4);

    bool isValid = isShadowValid(_D4);

    if (isValid) {
        info.setupShadow(nullptr);
    }

    s32 arg1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);

    if (arg1 != -1) {
        info.setupBinder(100.0f, 0.0f);
    }

    initialize(rIter, info);
    if (isCalcShadowAlways()) {
        MR::onCalcShadow(this, nullptr);
        MR::onCalcShadowDropGravity(this, nullptr);
    }
}

/*
bool SpaceMine::isCalcShadowAlways() const {

    return isShadowValid()

    s32 v1 = _D4;
    bool v2 = true;

    if (v1 != 1) {
        v2 = isShadowValid(v1) && hasRail() == false ? false : true;
    }

    return v2;
}
*/

void SpaceMine::kill() {
    MR::emitEffect(this, "Explosion");
    MR::startSound(this, "SE_OJ_SPACEMINE_EXPLOSION", -1, -1);
    MR::tryRumblePad(this, "中", 0);
    MapObjActor::kill();
}

void SpaceMine::initAfterPlacement() {
    bool isShadow = isShadowValid(_D4);

    if (isShadow) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
    }
}

void SpaceMine::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (MR::sendMsgEnemyAttackExplosion(a2, a1)) {
        kill();
    }
    else {
        MR::sendMsgPush(a2, a1);
    }
}

bool SpaceMine::receiveMsgPlayerAttack(u32 msg, HitSensor *, HitSensor *) {
    if (MR::isMsgInvincibleAttack(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        kill();
        return true;
    }

    return false;
}

void SpaceMine::exeWait() {
    if (isCalcShadowAlways()) {
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
    }

    f32 playerDist = MR::calcDistanceToPlayer(this);
    if (_D8) {
        if (1450.0f < playerDist) {
            _D8 = 0;
            _C4 = 1.0f;
        }
    }
    else if (playerDist < 1500.0f) {
        _D8 = 1;
        _C4 = 1.0f;
    }

    if (MR::isBinded(this)) {
        if (MR::isBindedGround(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getGroundSensor(this));
        }

        if (MR::isBindedWall(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getWallSensor(this));
        }

        if (MR::isBindedRoof(this)) {
            MR::sendMsgEnemyAttackExplosionToBindedSensor(this, MR::getWallSensor(this));
        }

        MR::offBind(this);
        kill();
    }
    else {
        f32 v6 = MR::subtractFromSum(_C4, mRotation.y, 0.0f);
        mRotation.y += _C4;
        mRotation.y = MR::modAndAdd(0.0f, v6);
    }
}

void SpaceMine::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Appear");
        MR::setClippingRangeIncludeShadow(this, &mClippingRange, 100.0f);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPACEMINE_APPEAR", -1, -1, -1);

    if (MR::isStep(this, 120)) {
        MR::startBck(this, "Appear", nullptr);
        MR::showModel(this);
        MR::startSound(this, "SE_OJ_SPACEMINE_APPEAR", -1, -1);
    }

    if (MR::isGreaterStep(this, 120)) {
        if (MR::isBckStopped(this)) {
            setNerve(&NrvSpaceMine::HostTypeWait::sInstance);
        }
    }
}

SpaceMine::~SpaceMine() {

}

namespace NrvSpaceMine {
    void HostTypeAppear::execute(Spine *pSpine) const {
        SpaceMine* mine = reinterpret_cast<SpaceMine*>(pSpine->mExecutor);
        mine->exeAppear();
    }

    void HostTypeWait::execute(Spine *pSpine) const {
        SpaceMine* mine = reinterpret_cast<SpaceMine*>(pSpine->mExecutor);
        mine->exeWait();
    }

    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeAppear);
};