#include "Game/Boss/DinoPackunTrackFire.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

namespace NrvDinoPackunFire {
    NEW_NERVE(DinoPackunTrackFireNrvWait, DinoPackunTrackFire, Wait);
    NEW_NERVE(DinoPackunTrackFireNrvGround, DinoPackunTrackFire, Ground);
};  // namespace NrvDinoPackunFire

DinoPackunTrackFireHolder::DinoPackunTrackFireHolder(s32 max) : DeriveActorGroup< DinoPackunTrackFire >("シッポ跡炎管理", max) {
    for (s32 i = 0; i < max; i++) {
        DinoPackunTrackFire* fire = new DinoPackunTrackFire("シッポ跡炎");
        fire->initWithoutIter();
        registerActor(fire);
    }
}

DinoPackunTrackFire::DinoPackunTrackFire(const char* pName) : LiveActor(pName) {
    _8C.identity();
}

void DinoPackunTrackFire::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemyMovement(this);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensorEnemyAttack(this, "attack", 8, 60.0f, sensorOffs);
    TVec3f spOffs;
    spOffs.x = 0.0f;
    spOffs.y = 0.0f;
    spOffs.z = 0.0f;
    MR::initStarPointerTarget(this, 60.0f, spOffs);
    initBinder(60.0f, 0.0f, 0);
    initNerve(&NrvDinoPackunFire::DinoPackunTrackFireNrvWait::sInstance);
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
    initEffectKeeper(0, "DinoPackunTrackFire", false);
    initSound(4, false);
    makeActorDead();
}

void DinoPackunTrackFire::control() {
    TVec3f v1;
    JMathInlineVEC::PSVECNegate(&mGravity, &v1);
    MR::makeMtxUpNoSupportPos(&_8C, v1, mPosition);

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        kill();
    }
}

void DinoPackunTrackFire::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::isSensorPlayer(pReceiver);
    MR::sendMsgEnemyAttackFire(pReceiver, pSender);
}

bool DinoPackunTrackFire::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void DinoPackunTrackFire::appearAndSetPos(const TVec3f& rPos) {
    mPosition.set(rPos);
    MR::zeroVelocity(this);
    MR::startSound(this, "SE_BM_D_PAKKUN_FIREBALL_S_ON", -1, -1);
    setNerve(&NrvDinoPackunFire::DinoPackunTrackFireNrvWait::sInstance);
    appear();
}

void DinoPackunTrackFire::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "TrackFire");
    }

    MR::addVelocityToGravity(this, 2.0f);
    MR::attenuateVelocity(this, 0.99199998f);
    if (MR::isBindedGround(this)) {
        setNerve(&NrvDinoPackunFire::DinoPackunTrackFireNrvGround::sInstance);
        MR::zeroVelocity(this);
    } else if (MR::isGreaterStep(this, 300)) {
        MR::deleteEffect(this, "TrackFire");
        kill();
    }
}

void DinoPackunTrackFire::exeGround() {
    MR::zeroVelocity(this);

    if (MR::isGreaterStep(this, 75)) {
        MR::deleteEffect(this, "TrackFire");
        kill();
    }
}

DinoPackunTrackFire::~DinoPackunTrackFire() {}

DinoPackunTrackFireHolder::~DinoPackunTrackFireHolder() {}

MtxPtr DinoPackunTrackFire::getBaseMtx() const {
    return (MtxPtr)&_8C;
}
