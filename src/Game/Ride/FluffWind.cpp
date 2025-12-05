#include "Game/Ride/FluffWind.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/RailUtil.hpp"

namespace NrvFluffWindEffect {
    NEW_NERVE(FluffWindEffectNrvBrowWind, FluffWindEffect, BrowWind);
}

FluffWind::FluffWind(const char* pName) :
    LiveActor(pName),
    mNumEffects(0),
    mEffects(nullptr)
{ }

void FluffWind::init(const JMapInfoIter& rIter) {
    initRailRider(rIter);
    initEffectKeeper(0, "FluffWind", false);
    MR::invalidateClipping(this);
    MR::createSceneObj(SceneObj_FluffWindHolder);
    MR::getSceneObj<FluffWindHolder>(SceneObj_FluffWindHolder)->registerActor(this);
    
    s32 count = MR::getRailTotalLength(this) / 600.0f;
    mNumEffects = count + 1;
    f32 stepDistance = MR::getRailTotalLength(this) / (this->mNumEffects - 1);
    mEffects = new FluffWindEffect*[mNumEffects];
    for (s32 i = 0; i < mNumEffects; i++) {
        f32 currentRailPos = i * stepDistance;
        TVec3f pos;
        TVec3f dir;
        MR::calcRailPosAndDirectionAtCoord(&pos, &dir, this, currentRailPos);

        FluffWindEffect* effect = new FluffWindEffect("わたげエフェクト");
        effect->initEffectInfo(pos, dir, TVec3f(0.0f, 1.0f, 0.0f), "FluffWind", -1.0f);
        effect->initWithoutIter();

        this->mEffects[i] = effect;
    }
    
    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::FunctorV0M<FluffWind*, void (FluffWind::*)()> functor(this, &FluffWind::makeActorDead);
        MR::listenStageSwitchOnB(this, functor);
    }
    
    makeActorAppeared();
}

void FluffWind::makeActorDead() {
    for (s32 i = 0; i < mNumEffects; i++) {
        mEffects[i]->makeActorDead();
    }
    LiveActor::makeActorDead();
}

FluffWindEffect::FluffWindEffect(const char* pName) :
    LiveActor(pName),
    mEffectName(nullptr),
    mTimer(-1)
{
    mMtx.identity();
}

void FluffWindEffect::init(const JMapInfoIter&) {
    makeActorAppeared();
}

void FluffWindEffect::makeActorDead() {
    LiveActor::makeActorDead();
    MR::deleteEffect(this, mEffectName);
}

void FluffWindEffect::startClipped() {
    LiveActor::startClipped();
}

void FluffWindEffect::initEffectInfo(const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rPos, const char* pEffectName, f32 radius) {
    mPosition = rFront;
    MR::connectToSceneMapObjMovement(this);
    MR::makeMtxFrontUpPos(&mMtx, rUp, rPos, rFront);
    mEffectName = pEffectName;
    initEffectKeeper(0, pEffectName, false);
    MR::setEffectHostMtx(this, mEffectName, mMtx);
    initNerve(&NrvFluffWindEffect::FluffWindEffectNrvBrowWind::sInstance);
    if (radius > 0.0f) {
        MR::setClippingTypeSphere(this, radius);
    }
}

void FluffWindEffect::exeBrowWind() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, mEffectName);
        mTimer = MR::getRandom((s32)60, 240);
    }
    
    if (MR::isStep(this, 180)) {
        MR::validateClipping(this);
    }
    
    if (MR::isStep(this, mTimer)) {
        MR::invalidateClipping(this);
        setNerve(&NrvFluffWindEffect::FluffWindEffectNrvBrowWind::sInstance);
    }
}

FluffWindHolder::FluffWindHolder() :
    LiveActorGroup("わたげ風", 8)
{ }

// Non-matching: some PS/float stuffs. https://decomp.me/scratch/ZbyJa
void FluffWindHolder::calcWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindStrength) const {
    pWindDirection->zero();
    *pWindStrength = -1.0f;
    
    f32 bestDistance = 1000000.0f;
    TVec3f bestDir(0.0f, 0.0f, 0.0f);
    for (s32 i = 0; i < mObjectCount; i++) {
        LiveActor* pActor = getActor(i);
        
        TVec3f railDir(0.0f, 0.0f, 0.0f);
        TVec3f nearestPos;
        MR::calcNearestRailPosAndDirection(&nearestPos, &railDir, pActor, rPosition);
        
        f32 distance = PSVECDistance(&nearestPos, &rPosition);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestDir.set(railDir);
        }
    }
    
    *pWindDirection = bestDir;
    *pWindStrength = bestDistance;
}

bool FluffFunction::calcFluffWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindStrength) {
    if (!MR::isExistSceneObj(SceneObj_FluffWindHolder)) {
        pWindDirection->zero();
        *pWindStrength = -1.0f;
        
        return false;
    }
    
    FluffWindHolder* windHolder = MR::getSceneObj<FluffWindHolder>(SceneObj_FluffWindHolder);
    windHolder->calcWindInfo(rPosition, pWindDirection, pWindStrength);
    
    return true;
}
