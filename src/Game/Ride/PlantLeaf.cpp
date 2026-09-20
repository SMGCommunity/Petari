#include "Game/Ride/PlantLeaf.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

void PlantLeaf_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const f32 sScaleMin = 0.0001f;
    static const f32 sDistancePush = 50.0f;
    static const f32 sPlayerPushRate = 0.02f;
    static const f32 sSpringAccelRate = 0.1f;
    static const f32 sSpringFrictionRate = 0.9f;
    static const f32 sSpringSpeedMax = 0.5f;
    static const f32 sSpringCoordMax = 0.9f;
    static const f32 sSpringCoordToStop = 0.01f;
    static const f32 sSpringSpeedToStop = 0.001f;
};  // namespace

PlantLeaf::PlantLeaf(f32 leafCoord, const TVec3f& rPosition, const TVec3f& rGrowDirection, f32 leafSize)
    : LiveActor("葉（伸び植物）"), mSpringCoord(), mSpringSpeed(), mLeafCoord(leafCoord), mLeafSize(leafSize), mSide(1.0f, 0.0f, 0.0f),
      mUp(0.0f, 1.0f, 0.0f), mFront(rGrowDirection) {
    mPosition.set(rPosition);
    MR::makeAxisFrontUp(&mSide, &mUp, mFront, mUp);
    mBaseMtx.setXYZDir(mSide, mUp, mFront);
    mBaseMtx.scale(mLeafSize * ::sScaleMin);
    mBaseMtx.setTrans(mPosition);
    mPosMtx.identity();
}

PlantLeafDrawInit::PlantLeafDrawInit(const char* pName) : LiveActor(pName), mMaterial(), mShape(), mShapeDraw() {
    MR::registerPreDrawFunction(MR::Functor(this, &PlantLeafDrawInit::initDraw), MR::DrawType_Plant);

    initModelManagerWithAnm("PlantLeaf", 0, false);
    J3DModelData* pModelData = MR::getJ3DModelData(this);
    mMaterial = MR::getMaterial(pModelData, 0);
    mShape = mMaterial->mShape;
    mShapeDraw = *mShape->mShapeDraw;
}

void PlantLeaf::init(const JMapInfoIter&) {
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void PlantLeaf::updateGrowUp(const TVec3f& rStalkPos, const TVec3f& rAxisY, f32 growthPercent, f32 offset) {
    mPosition.set(mFront);
    mPosition *= offset;
    mPosition.add(rStalkPos);

    f32 t = growthPercent * growthPercent;
    mUp.x = rAxisY.x * (1.0f - t);
    mUp.y = rAxisY.y * (1.0f - t) + t;
    mUp.z = rAxisY.z * (1.0f - t);

    MR::makeAxisFrontUp(&mSide, &mUp, mFront, mUp);
    mBaseMtx.setXYZDir(mSide, mUp, mFront);
    mBaseMtx.scale(mLeafSize * growthPercent);
    mBaseMtx.setTrans(mPosition);
}

bool PlantLeaf::updateSpring(const TVec3f& rPos, f32 springPower, f32 growthPercent) {
    if (mPosition.squared(rPos) < ::sDistancePush * ::sDistancePush) {
        if (mSpringCoord == 0.0f && mSpringSpeed == 0.0f) {
            MR::tryRumblePadWeak(this, WPAD_CHAN0);
        }

        mSpringSpeed += springPower * ::sPlayerPushRate;
    }

    return updateSpring(growthPercent);
}

bool PlantLeaf::updateSpring(f32 growthPercent) {
    mSpringSpeed -= mSpringCoord * ::sSpringAccelRate;
    mSpringSpeed = MR::clamp(mSpringSpeed, -::sSpringSpeedMax, ::sSpringSpeedMax);

    mSpringCoord += mSpringSpeed;
    mSpringCoord = MR::clamp(mSpringCoord, -::sSpringCoordMax, ::sSpringCoordMax);

    mSpringSpeed *= ::sSpringFrictionRate;

    if (MR::abs(mSpringCoord) < ::sSpringCoordToStop && MR::abs(mSpringSpeed) < ::sSpringSpeedToStop) {
        mSpringCoord = 0.0f;
        mSpringSpeed = 0.0f;
        MR::makeAxisFrontUp(&mSide, &mUp, mFront, mUp);
        mBaseMtx.setXYZDir(mSide, mUp, mFront);
        mBaseMtx.scale(mLeafSize * growthPercent);
        mBaseMtx.setTrans(mPosition);
        return true;
    } else {
        TVec3f side = mFront;
        TVec3f front;
        TVec3f up;
        side.y += mSpringCoord;
        MR::normalize(&side);
        MR::makeAxisFrontUp(&front, &up, side, mUp);
        mBaseMtx.setXYZDir(front, up, side);
        mBaseMtx.scale(mLeafSize * growthPercent);
        mBaseMtx.setTrans(mPosition);
        return false;
    }
}

void PlantLeafDrawInit::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    makeActorDead();
}

void PlantLeafDrawInit::initDraw() const {
    J3DModelData* pModelData = MR::getJ3DModelData(this);
    j3dSys.mVtxPos = pModelData->mVertexData.mVtxPosArray;
    j3dSys.mVtxNrm = pModelData->mVertexData.mVtxNrmArray;
    j3dSys.mVtxCol = pModelData->mVertexData.mVtxColorArray[0];
    mShape->sOldVcdVatCmd = 0;
    mMaterial->loadSharedDL();
    mShape->loadPreDrawSetting();
    mShape->loadVtxArray();
}
