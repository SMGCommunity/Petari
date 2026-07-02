#include "Game/MapObj/IronCannonShell.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

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
    : LiveActor(pName), mShellHolder(nullptr), mShotInterval(::sDefaultShotInterval), mBulletSpeed(::sDefaultBulletSpeed) {
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
    initNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance);

    if (isAppear)
        makeActorDead();
    else
        makeActorAppeared();
}

void IronCannonLauncherPoint::exeWait() {
    if (!MR::isStageStateScenarioOpeningCamera() && MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, mShotInterval))
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvShot::sInstance);
}

void IronCannonLauncherPoint::exeShot() {
    if (MR::isFirstStep(this)) {
        tryShotBullet(0.0f);
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance);
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
    if (pShell == nullptr)
        return false;

    TPos3f mtx;
    MR::makeMtxTRS(mtx, mPosition, mRotation, mScale);
    TVec3f vec;
    mtx.getZDirInline(vec);

    TVec3f vec2;
    vec2.scaleAdd(vec, mPosition, ::sGunPointOffset + offset);
    pShell->launch(vec2, vec.multInLine(mBulletSpeed));

    TVec3f shadowdir;
    mtx.getYDirInline(shadowdir);
    shadowdir.invertInternal();
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
    mEffectMtx.scaleAdd(front, mPosition, ::sGunPointModelOffset);
    MR::setEffectHostSRT(this, "Shoot", &mEffectMtx, nullptr, nullptr);
    setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
}

void IronCannonLauncher::exeRelax() {
    if (MR::isOnSwitchA(this))
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
}

void IronCannonLauncher::exeWait() {
    if (MR::isStep(this, mShotInterval))
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvShot::sInstance);
}

void IronCannonLauncher::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Shot");
        tryShotBullet(::sGunPointModelOffset);
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
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

s32 IronCannonShell::getLifeTime() const {
    return ::sDefaultShotInterval;
}

f32 IronCannonShell::getBaseScale() const {
    return 1.3f;
}