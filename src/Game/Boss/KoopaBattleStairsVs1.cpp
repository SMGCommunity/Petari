#include "Game/Boss/KoopaBattleStairsVs1.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFireStairs.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaSwitchKeeper.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"

namespace {
    static const char* sKoopaPosName0 = "階段の戦い０（クッパ）";
    static const char* sKoopaPosName1 = "階段の戦い１（クッパ）";
    static const char* sKoopaPosName2 = "階段の戦い２（クッパ）";
    static const char* sKoopaPosNameEnd = "デモ中心";
}  // namespace

namespace NrvKoopaBattleStairsVs1 {
    NEW_NERVE(KoopaBattleStairsVs1NrvWaitDemo, KoopaBattleStairsVs1, WaitDemo);
    NEW_NERVE(KoopaBattleStairsVs1NrvDemo, KoopaBattleStairsVs1, Demo);
    NEW_NERVE(KoopaBattleStairsVs1NrvWait, KoopaBattleStairsVs1, Wait);
    NEW_NERVE(KoopaBattleStairsVs1NrvAttackFire, KoopaBattleStairsVs1, AttackFire);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosStart, KoopaBattleStairsVs1, JumpToNextPosStart);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosLoop, KoopaBattleStairsVs1, JumpToNextPosLoop);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosEnd, KoopaBattleStairsVs1, JumpToNextPosEnd);
}  // namespace NrvKoopaBattleStairsVs1

KoopaBattleStairsVs1::KoopaBattleStairsVs1(Koopa* pKoopa)
    : KoopaBattleStairsBase(pKoopa), _10(), _14(), _18(), _1C(), _20(), _24(), _28(0.0f, 0.0f, 0.0f), _34(0.0f, 0.0f, 0.0f), _40(0.0f, 0.0f, 0.0f),
      _4C(), _50(), _54(), _58(), _5C(), _60(0.0f, 0.0f, 0.0f), _6C(0.0f, 0.0f, 0.0f), _78(0.0f, 0.0f, 1.0f) {
    initNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWaitDemo::sInstance);

    calcFireStartPos(&_28, ::sKoopaPosName0);
    calcFireStartPos(&_34, ::sKoopaPosName1);
    calcFireStartPos(&_40, ::sKoopaPosName2);

    KoopaFunction::initKoopaAnimCamera(mKoopa, "DemoBattleStairsStart");
}

KoopaBattleStairsVs1::~KoopaBattleStairsVs1() {
}

void KoopaBattleStairsVs1::registerStair(KoopaBattleMapStair* pBattleMapStair) {
    if (pBattleMapStair->isTypeDemoFar()) {
        _10 = pBattleMapStair;
    }

    if (pBattleMapStair->isTypeDemoNear()) {
        _14 = pBattleMapStair;
    }

    _C->registerActor(pBattleMapStair);

    TVec3f* vec = &_28;

    if (pBattleMapStair->_94 == 1) {
        vec = &_34;
        if (pBattleMapStair->isTypeNormal()) {
            _50++;
        }
    } else if (pBattleMapStair->_94 == 2) {
        vec = &_40;
        if (pBattleMapStair->isTypeNormal()) {
            _54++;
        }
    } else {
        if (pBattleMapStair->isTypeNormal()) {
            _4C++;
        }
    }

    calcFireAttackStep(pBattleMapStair, 20.0f, 15, *vec);
}

void KoopaBattleStairsVs1::exeWaitDemo() {
    if (MR::tryStartDemo(mKoopa, "階段の戦い開始デモ")) {
        KoopaFunction::startKoopaAnimCamera(mKoopa, "DemoBattleStairsStart", 60);
        KoopaFunction::endFaceCtrl(mKoopa, -1);

        MR::startAction(mKoopa, "DemoBattleStairsStart");
        MR::stopStageBGM(60);

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvDemo::sInstance);
    }
}

void KoopaBattleStairsVs1::exeDemo() {
    if (MR::isStep(this, 264)) {
        KoopaBattleMapStair* pBattleMapStair = _10;
        Koopa* pKoopa = mKoopa;
        TVec3f jointPos = TVec3f(0.0f, 0.0f, 0.0f);
        MR::copyJointPos(pKoopa, "Tongue2", &jointPos);

        _18 = KoopaFunction::emitFireStairsToTarget(pKoopa, pBattleMapStair, jointPos, false);

        MR::startSound(mKoopa, "SE_OJ_KOOPA_BULLET_SHOOT", -1, -1);

        MR::requestMovementOn(_18);
        MR::requestMovementOn(_10);
    }

    if (MR::isStep(this, 292)) {
        KoopaBattleMapStair* pBattleMapStair = _14;
        Koopa* pKoopa = mKoopa;
        TVec3f jointPos = TVec3f(0.0f, 0.0f, 0.0f);
        MR::copyJointPos(pKoopa, "Tongue2", &jointPos);

        _1C = KoopaFunction::emitFireStairsToTarget(pKoopa, pBattleMapStair, jointPos, false);

        MR::startSound(mKoopa, "SE_OJ_KOOPA_BULLET_SHOOT", -1, -1);

        MR::requestMovementOn(_1C);
        MR::requestMovementOn(_14);
    }

    if (MR::isBckStopped(mKoopa)) {
        KoopaFunction::endKoopaAnimCamera(mKoopa, "DemoBattleStairsStart", 0);
        MR::endDemo(mKoopa, "階段の戦い開始デモ");

        MR::onSwitchB(KoopaFunction::getKoopaSwitchKeeper(mKoopa));

        KoopaFunction::startFaceCtrl(mKoopa);
        MR::startStageBGM("MBGM_GALAXY_15_HURRY", false);

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
    }
}

void KoopaBattleStairsVs1::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "Wait");
    }

    if (tryAttackRequest()) {
        MR::startAction(mKoopa, "AttackFire");
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
    }
}

void KoopaBattleStairsVs1::exeAttackFire() {
    if (tryRequestedToMoveNextPos()) {
        _20 = nullptr;
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosStart::sInstance);
        return;
    }

    if (MR::isStep(this, 15)) {
        KoopaBattleMapStair* pBattleMapStair = _20;
        Koopa* pKoopa = mKoopa;
        TVec3f jointPos = TVec3f(0.0f, 0.0f, 0.0f);
        MR::copyJointPos(pKoopa, "Tongue2", &jointPos);
        KoopaFunction::emitFireStairsToTarget(pKoopa, pBattleMapStair, jointPos, false);
        _20 = nullptr;
    }

    KoopaBattleMapStair* pBattleMapStair = _20;
    if (MR::isLessStep(this, 15) && tryAttackRequest()) {
        Koopa* pKoopa = mKoopa;
        TVec3f jointPos = TVec3f(0.0f, 0.0f, 0.0f);
        MR::copyJointPos(pKoopa, "Tongue2", &jointPos);
        KoopaFunction::emitFireStairsToTarget(pKoopa, pBattleMapStair, jointPos, false);

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);

        return;
    }

    if (MR::isGreaterEqualStep(this, 15) && tryAttackRequest()) {
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, 33)) {
        if (_24 || (_5C == 0 && _58 >= _4C) || (_5C == 1 && _58 >= _4C + _50) || (_5C == 2 && _58 >= _4C + _50 + _54)) {
            _20 = 0;
            setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosStart::sInstance);
            return;
        }

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
    }
}

void KoopaBattleStairsVs1::exeJumpToNextPosStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "JumpStartFast");
        MR::startSound(mKoopa, "SE_BM_KOOPA_JUMP", -1, -1);
        _24 = false;
    }

    if (MR::isBckStopped(mKoopa)) {
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosLoop::sInstance);
    }
}

void KoopaBattleStairsVs1::exeJumpToNextPosLoop() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "JumpFast");

        _60.set(mKoopa->mPosition);
        _5C++;
        TVec3f namePos = TVec3f(0.0f, 0.0f, 0.0f);

        if (_5C == 1) {
            MR::findNamePos(sKoopaPosName1, &_6C, &namePos);
        } else if (_5C == 2) {
            MR::findNamePos(sKoopaPosName2, &_6C, &namePos);
        } else {
            MR::findNamePos(sKoopaPosNameEnd, &_6C, &namePos);
        }

        MR::getRotatedAxisZ(&_78, namePos);
    }

    f32 step = getNerveStep() / 60.0f;
    f32 val = MR::getEaseInOutValue(step, 0.0f, 1.0f, 1.0f);
    mKoopa->mPosition.set(_60 * (1.0f - val) + _6C * val);

    mKoopa->mPosition.y += 1000.0f * JMASinDegree(180.0f * step);
    MR::turnDirectionDegree(mKoopa, &mKoopa->mFront, _78, 2.0f);

    if (MR::isStep(this, 60)) {
        MR::tryRumblePadStrong(mKoopa, 0);
        MR::shakeCameraNormal();
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosEnd::sInstance);
    }
}

void KoopaBattleStairsVs1::exeJumpToNextPosEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "LandFast");
        MR::startSound(mKoopa, "SE_BM_KOOPA_LAND", -1, -1);
    }

    if (tryAttackRequest()) {
        if (_5C == 1) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName1);
        } else {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName2);
        }
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
        return;
    }

    if (MR::isBckStopped(mKoopa)) {
        if (_5C == 1) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName1);
        } else if (_5C == 2) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName2);
        } else {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosNameEnd);
        }
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
    }
}

bool KoopaBattleStairsVs1::tryAttackRequest() {
    bool out = false;
    for (int idx = 0; idx < _C->mObjectCount; idx++) {
        KoopaBattleMapStair* pBattleMapStair = static_cast< KoopaBattleMapStair* >(_C->getActor(idx));

        if (pBattleMapStair->isRequestAttackVs1()) {
            out = true;
            _20 = pBattleMapStair;
            _58++;
        }
    }

    return out;
}

bool KoopaBattleStairsVs1::tryRequestedToMoveNextPos() {
    for (int idx = 0; idx < _C->mObjectCount; idx++) {
        KoopaBattleMapStair* pBattleMapStair = static_cast< KoopaBattleMapStair* >(_C->getActor(idx));

        if (pBattleMapStair->isRequestAttackVs1()) {
            if (pBattleMapStair->_94 > _5C) {
                return true;
            }

            if (pBattleMapStair->_9C) {
                _24 = true;
            }
        }
    }

    return false;
}
