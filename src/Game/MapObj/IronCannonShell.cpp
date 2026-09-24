#include "Game/MapObj/IronCannonShell.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void IronCannonShell_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)30.0f;
    (void)75.0f;
    (void)100.0f;
    (void)1.2999999523162842f;
    (void)0.0f;
}

namespace {
    static const s32 sDefaultShotInterval = 300;
    static const f32 sDefaultBulletSpeed = 30.0f;
    static const f32 sGunPointOffset = 75.0f;
    static const f32 sGunPointModelOffset = 100.0f;
};  // namespace

namespace NrvIronCannonLauncherPoint {
    NEW_NERVE(IronCannonLauncherPointNrvWait, IronCannonLauncherPoint, Wait);
    NEW_NERVE(IronCannonLauncherPointNrvShot, IronCannonLauncherPoint, Shot);
};  // namespace NrvIronCannonLauncherPoint

namespace NrvIronCannonLauncher {
    NEW_NERVE(IronCannonLauncherNrvRelax, IronCannonLauncher, Relax);
    NEW_NERVE(IronCannonLauncherNrvWait, IronCannonLauncher, Wait);
    NEW_NERVE(IronCannonLauncherNrvShot, IronCannonLauncher, Shot);
};  // namespace NrvIronCannonLauncher

IronCannonShell::IronCannonShell(const char* pName) : KoopaJrShipCannonShell(pName) {
}

IronCannonLauncherPoint::IronCannonLauncherPoint(const char* pName)
    : LiveActor(pName), mShellHolder(), mShotInterval(::sDefaultShotInterval), mBulletSpeed(::sDefaultBulletSpeed) {
}

void IronCannonLauncherPoint::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelAndConnectToScene();
    initBullet();
    MR::getJMapInfoArg0NoInit(rIter, &mShotInterval);
    MR::getJMapInfoArg1NoInit(rIter, &mBulletSpeed);
    initEffectKeeper(0, "IronCannonLauncherPoint", false);

    bool isAppear = false;

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        isAppear = true;
    }

    MR::useStageSwitchReadA(this, rIter);
    MR::setGroupClipping(this, rIter, 32);
    initNerve(GET_NERVE(IronCannonLauncherPoint, IronCannonLauncherPointNrvWait));

    if (isAppear) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void IronCannonLauncherPoint::exeWait() {
    if (!MR::isStageStateScenarioOpeningCamera() && MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(IronCannonLauncherPoint, IronCannonLauncherPointNrvWait));
        return;
    }

    if (MR::isGreaterStep(this, mShotInterval)) {
        setNerve(GET_NERVE(IronCannonLauncherPoint, IronCannonLauncherPointNrvShot));
    }
}

void IronCannonLauncherPoint::exeShot() {
    if (MR::isFirstStep(this)) {
        tryShotBullet(0.0f);
        setNerve(GET_NERVE(IronCannonLauncherPoint, IronCannonLauncherPointNrvWait));
    }
}

void IronCannonLauncherPoint::initModelAndConnectToScene() {
    MR::connectToSceneMapObjMovement(this);
}

void IronCannonLauncherPoint::initBullet() {
    mShellHolder = new CannonShellHolder(3);

    for (s32 i = 0; i < 3; i++) {
        IronCannonShell* pShell = new IronCannonShell("キャノン弾砲台の弾");
        pShell->initWithoutIter();
        pShell->makeActorDead();
        mShellHolder->registerCannonShell(pShell);
    }
}

bool IronCannonLauncherPoint::tryShotBullet(f32 offset) {
    IronCannonShell* pShell = static_cast< IronCannonShell* >(mShellHolder->getValidShell());

    if (pShell == nullptr) {
        return false;
    }

    TPos3f mtx;
    MR::makeMtxTRS(mtx, mPosition, mRotation, mScale);
    TVec3f vec;
    mtx.getZDir(vec);

    TVec3f vec2;
    vec2.scaleAdd(::sGunPointOffset + offset, vec, mPosition);
    pShell->launch(vec2, vec * mBulletSpeed);

    TVec3f shadowdir;
    mtx.getYDir(shadowdir);
    shadowdir.negate();
    MR::setShadowDropDirection(pShell, nullptr, shadowdir);

    MR::emitEffect(this, "Shoot");
    return true;
}

IronCannonLauncher::IronCannonLauncher(const char* pName) : IronCannonLauncherPoint(pName), mEffectMtx(0.0f, 0.0f, 0.0f) {
}

// weird function ordering bro
IronCannonLauncherPoint::~IronCannonLauncherPoint() {
}

void IronCannonLauncher::init(const JMapInfoIter& rIter) {
    IronCannonLauncherPoint::init(rIter);

    MR::useStageSwitchReadA(this, rIter);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, ::sGunPointModelOffset);
    TVec3f front;
    MR::calcFrontVec(&front, this);
    mEffectMtx.scaleAdd(::sGunPointModelOffset, front, mPosition);
    MR::setEffectHostSRT(this, "Shoot", &mEffectMtx, nullptr, nullptr);
    setNerve(GET_NERVE(IronCannonLauncher, IronCannonLauncherNrvWait));
}

void IronCannonLauncher::exeRelax() {
    if (MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(IronCannonLauncher, IronCannonLauncherNrvWait));
    }
}

void IronCannonLauncher::exeWait() {
    if (MR::isStep(this, mShotInterval)) {
        setNerve(GET_NERVE(IronCannonLauncher, IronCannonLauncherNrvShot));
    }
}

void IronCannonLauncher::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Shot");
        tryShotBullet(::sGunPointModelOffset);
        setNerve(GET_NERVE(IronCannonLauncher, IronCannonLauncherNrvWait));
    }
}

void IronCannonLauncher::initModelAndConnectToScene() {
    initModelManagerWithAnm("IronCannonLauncher", nullptr, false);
    MR::connectToSceneMapObj(this);
}

IronCannonShell::~IronCannonShell() {
}

IronCannonLauncher::~IronCannonLauncher() {
}
