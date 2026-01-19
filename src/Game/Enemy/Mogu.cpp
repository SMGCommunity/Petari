#include "Game/Enemy/Mogu.hpp"
#include "Game/Enemy/MoguStone.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

namespace {}

namespace NrvMogu {
    NEW_NERVE(HostTypeNrvHideWait, Mogu, HideWait);
    NEW_NERVE(HostTypeNrvHide, Mogu, Hide);
    NEW_NERVE(HostTypeNrvAppear, Mogu, Appear);
    NEW_NERVE(HostTypeNrvSearch, Mogu, Search);
    NEW_NERVE(HostTypeNrvTurn, Mogu, Search);
    NEW_NERVE(HostTypeNrvThrow, Mogu, Throw);
    NEW_NERVE(HostTypeNrvSwoonStart, Mogu, SwoonStart);
    NEW_NERVE(HostTypeNrvSwoonEnd, Mogu, SwoonEnd);
    NEW_NERVE(HostTypeNrvSwoon, Mogu, Swoon);
    NEW_NERVE(HostTypeNrvHipDropReaction, Mogu, HipDropReaction);
    NEW_NERVE(HostTypeNrvStampDeath, Mogu, StampDeath);
    NEW_NERVE(HostTypeNrvHitBlow, Mogu, HitBlow);
}  // namespace NrvMogu

Mogu::Mogu(const char* pName)
    : LiveActor(pName), mNerveExecutor(nullptr), _90(0), mStone(nullptr), _98(nullptr), _9C(0, 0, 1), _A8(0, 1, 0), _B4(true), mIsCannonFleet(false) {
}

void Mogu::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
        MR::useStageSwitchReadA(this, rIter);
        MR::useStageSwitchSleep(this, rIter);

        s32 value = -1;
        MR::getJMapInfoArg0NoInit(rIter, &value);

        if (value != -1) {
            _B4 = false;
        } else {
            _B4 = true;
        }
    }
    initModelManagerWithAnm("Mogu", nullptr, false);
    MtxPtr mtx = getBaseMtx();

    _A8.set< f32 >(mtx[0][1], mtx[0][2], mtx[0][3]);
    MR::connectToSceneEnemy(this);
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);

    f32 f1 = mScale.y * 160.0f;
    initBinder(f1, f1, 0);
    initHitSensor(2);
    TVec3f v1(-55.0f, 0.0f, 13.0f);
    TVec3f v2(v1);
    v2 *= mScale.y;

    MR::addHitSensorAtJointEnemy(this, "body", "Head", 32, 150.0f * mScale.y, v2);
    initEffectKeeper(false, nullptr, false);
    //
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 50.0f, 0.0f));

    _98->initWithoutIter();
    mIsCannonFleet = MR::isEqualStageName("CannonFleetGalaxy");
}

void Mogu::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
}

void Mogu::control() {
    mNerveExecutor->updateNerve();
}

void Mogu::endClipped() {
    LiveActor::endClipped();
    if (isNerve(&NrvMogu::HostTypeNrvTurn::sInstance)) {
        setNerve(&NrvMogu::HostTypeNrvTurn::sInstance);
    }
}

void Mogu::exeHideWait() {
    TVec3f normalToPlayer(*MR::getPlayerPos());
    normalToPlayer -= mPosition;
    MR::normalizeOrZero(&normalToPlayer);
    f32 dot = mGravity.dot(normalToPlayer);
    if (!(dot < -0.75f)) {
        f32 distanceToPlayer = MR::calcDistanceToPlayer(this);
        if (MR::isGreaterStep(this, 0x78) && 400.0f < distanceToPlayer && distanceToPlayer < 2000.0f) {
            setNerve(&NrvMogu::HostTypeNrvAppear::sInstance);
        }
    }
}

bool Mogu::isPlayerExistUp() {
    TVec3f vecToPlayer(*MR::getPlayerCenterPos());
    vecToPlayer -= mPosition;
    f32 dot = _A8.dot(vecToPlayer);
    if (dot < 0.0f) {
        return false;
    }

    TVec3f* playerGravity = MR::getPlayerGravity();
    MR::vecKillElement(vecToPlayer, *playerGravity, &vecToPlayer);
    return vecToPlayer.length() < 400.0f;
}

void Mogu::tearDownThrow() {
    if (mStone->isTaken()) {
        mStone->kill();
    }
}
