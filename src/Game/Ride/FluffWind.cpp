#include "Game/Ride/FluffWind.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"

void FluffWind_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    static const f32 sEffectEmitterInterval = 600.0f;
    static const s32 sStepBrowWindMin = 60;
    static const s32 sStepBrowWindMax = 240;
    static const s32 sStepToValidateClipping = 180;

    FluffWindHolder* getFluffWindHolder() {
        return MR::getSceneObj< FluffWindHolder >(SceneObj_FluffWindHolder);
    }
};  // namespace

namespace NrvFluffWindEffect {
    NEW_NERVE(FluffWindEffectNrvBrowWind, FluffWindEffect, BrowWind);
};  // namespace NrvFluffWindEffect

FluffWindEffect::FluffWindEffect(const char* pName) : LiveActor(pName), mEffectName(nullptr), mTimer(-1) {
    mMtx.identity();
}

void FluffWindEffect::initEffectInfo(const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rPos, const char* pEffectName, f32 radius) {
    mPosition.set(rFront);
    MR::connectToSceneMapObjMovement(this);
    MR::makeMtxFrontUpPos(&mMtx, rUp, rPos, rFront);
    mEffectName = pEffectName;
    initEffectKeeper(0, pEffectName, false);
    MR::setEffectHostMtx(this, mEffectName, mMtx);
    initNerve(GET_NERVE(FluffWindEffect, FluffWindEffectNrvBrowWind));
    if (radius > 0.0f) {
        MR::setClippingTypeSphere(this, radius);
    }
}

void FluffWindEffect::init(const JMapInfoIter&) {
    makeActorAppeared();
}

void FluffWindEffect::makeActorDead() {
    LiveActor::makeActorDead();
    MR::deleteEffect(this, mEffectName);
}

void FluffWindEffect::exeBrowWind() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, mEffectName);
        mTimer = MR::getRandom(::sStepBrowWindMin, ::sStepBrowWindMax);
    }

    if (MR::isStep(this, ::sStepToValidateClipping)) {
        MR::validateClipping(this);
    }

    if (MR::isStep(this, mTimer)) {
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(FluffWindEffect, FluffWindEffectNrvBrowWind));
    }
}

void FluffWindEffect::startClipped() {
    LiveActor::startClipped();
}

FluffWind::FluffWind(const char* pName) : LiveActor(pName), mNumEffects(), mEffects() {
}

void FluffWind::init(const JMapInfoIter& rIter) {
    initRailRider(rIter);
    initEffectKeeper(0, "FluffWind", false);
    MR::invalidateClipping(this);
    MR::createSceneObj(SceneObj_FluffWindHolder);
    ::getFluffWindHolder()->registerActor(this);

    s32 count = MR::getRailTotalLength(this) / ::sEffectEmitterInterval;
    mNumEffects = count + 1;
    f32 stepDistance = MR::getRailTotalLength(this) / (mNumEffects - 1);
    mEffects = new FluffWindEffect*[mNumEffects];
    for (s32 i = 0; i < mNumEffects; i++) {
        f32 currentRailPos = i * stepDistance;
        TVec3f pos;
        TVec3f dir;
        MR::calcRailPosAndDirectionAtCoord(&pos, &dir, this, currentRailPos);

        FluffWindEffect* effect = new FluffWindEffect("わたげエフェクト");
        effect->initEffectInfo(pos, dir, TVec3f(0.0f, 1.0f, 0.0f), "FluffWind", -1.0f);
        effect->initWithoutIter();

        mEffects[i] = effect;
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor(this, &FluffWind::makeActorDead));
    }

    makeActorAppeared();
}

void FluffWind::makeActorDead() {
    for (s32 i = 0; i < mNumEffects; i++) {
        mEffects[i]->makeActorDead();
    }
    LiveActor::makeActorDead();
}

FluffWindHolder::FluffWindHolder() : LiveActorGroup("わたげ風", 8) {
}

void FluffWindHolder::calcWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindDistance) const {
    pWindDirection->zero();
    *pWindDistance = -1.0f;

    f32 bestDistance = 1000000.0f;
    TVec3f bestDir(0.0f, 0.0f, 0.0f);

    LiveActor* pActor;
    for (s32 i = 0; i < getObjNum(); i++) {
        pActor = getActor(i);

        TVec3f railDir(0.0f, 0.0f, 0.0f);
        TVec3f nearestPos;
        MR::calcNearestRailPosAndDirection(&nearestPos, &railDir, pActor, rPosition);

        f32 distance = nearestPos.distance(rPosition);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestDir = railDir;
        }
    }

    pWindDirection->set(bestDir);
    *pWindDistance = bestDistance;
}

bool FluffFunction::calcFluffWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindDistance) {
    if (!MR::isExistSceneObj(SceneObj_FluffWindHolder)) {
        pWindDirection->zero();
        *pWindDistance = -1.0f;

        return false;
    }

    ::getFluffWindHolder()->calcWindInfo(rPosition, pWindDirection, pWindDistance);

    return true;
}
