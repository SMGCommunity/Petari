#include "Game/MapObj/LavaGalaxyParts.hpp"

LavaGalaxyParts::~LavaGalaxyParts() {

}

LavaGalaxyParts::LavaGalaxyParts(const char *pName) : LiveActor(pName) {
    mLodCtrl = nullptr;
    mMtxSetter = nullptr;
}

void LavaGalaxyParts::initAfterPlacement() {
    if (mMtxSetter != nullptr) {
        TVec3f upper;
        MR::calcMapGroundUpper(&upper, this);
        f32 dist = PSVECDistance(mPosition.toCVec(), upper.toVec());
        TVec3f neg = mPosition - upper;
        if (neg.dot(mGravity) < 0.0f) {
            dist *= -1.0f;
        }

        TVec3f localOffs;
        localOffs.z = 0.0f;
        localOffs.x = 0.0f;
        localOffs.y = dist;
        
        TVec3f bruh;
        bruh.x = localOffs.x;
        bruh.y = localOffs.y;
        bruh.z = localOffs.z;
        mMtxSetter->updateMtxUseBaseMtxWithLocalOffset(bruh);
        MR::offCalcGravity(this);
    }
}

void LavaGalaxyParts::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (mLodCtrl != nullptr) {
        mLodCtrl->validate();
    }
}

void LavaGalaxyParts::makeActorDead() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->invalidate();
    }

    LiveActor::makeActorDead();
}

void LavaGalaxyParts::control() {
    if (mLodCtrl != nullptr) {
        mLodCtrl->update();
    }
}