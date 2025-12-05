#include "Game/Ride/PlantLeaf.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

PlantLeaf::PlantLeaf(f32 leafCoord, const TVec3f& pPosition, const TVec3f& pGrowDirection, f32 leafSize)
    : LiveActor("葉（伸び植物）"), mSpringVel(0.0f), mSpringAccel(0.0f), mLeafCoord(leafCoord), mLeafSize(leafSize), mFront(1.0f, 0.0f, 0.0f),
      mUp(0.0f, 1.0f, 0.0f), mSide(pGrowDirection) {
    mPosition.set(pPosition);
    MR::makeAxisFrontUp(&mFront, &mUp, mSide, mUp);
    mBaseMtx.setXYZDir(mFront, mUp, mSide);
    mBaseMtx.scale(mLeafSize * 0.0001f);
    mBaseMtx.setTrans(mPosition);
    mPosMtx.identity();
}

PlantLeafDrawInit::PlantLeafDrawInit(const char* pName) : LiveActor(pName), mMaterial(nullptr), mShape(nullptr), mShapeDraw(nullptr) {
    MR::FunctorV0M< const PlantLeafDrawInit*, void (PlantLeafDrawInit::*)() const > preDrawFunctor(this, &PlantLeafDrawInit::initDraw);
    MR::registerPreDrawFunction(preDrawFunctor, 5);
    // The above should probably be this instead, but MR::Functor_Inline does not like consts at the moment
    // MR::registerPreDrawFunction(MR::Functor_Inline(const_cast<const PlantLeafDrawInit*>(this), &PlantLeafDrawInit::initDraw), 5);

    initModelManagerWithAnm("PlantLeaf", 0, false);
    J3DModelData* modelData = MR::getJ3DModelData(this);
    mMaterial = MR::getMaterial(modelData, 0);
    mShape = mMaterial->mShape;
    mShapeDraw = *mShape->mShapeDraw;
}

void PlantLeaf::init(const JMapInfoIter&) {
    MR::invalidateClipping(this);
    makeActorAppeared();
}

void PlantLeaf::updateGrowUp(const TVec3f& rStalkPos, const TVec3f& rAxisY, f32 growthPercent, f32 offset) {
    mPosition.set(mSide);
    mPosition.mult(offset);
    mPosition.addInLine(rStalkPos);

    f32 t = growthPercent * growthPercent;
    mUp.x = rAxisY.x * (1.0f - t);
    mUp.y = rAxisY.y * (1.0f - t) + t;
    mUp.z = rAxisY.z * (1.0f - t);

    MR::makeAxisFrontUp(&mFront, &mUp, mSide, mUp);
    mBaseMtx.setXYZDir(mFront, mUp, mSide);
    mBaseMtx.scale(mLeafSize * growthPercent);
    mBaseMtx.setTrans(mPosition);
}

bool PlantLeaf::updateSpring(const TVec3f& v, f32 springPower, f32 growthPercent) {
    if (mPosition.squared(v) < 2500.0f) {
        if (mSpringVel == 0.0f && mSpringAccel == 0.0f) {
            MR::tryRumblePadWeak(this, WPAD_CHAN0);
        }
        mSpringAccel += springPower * 0.02f;
    }
    return updateSpring(growthPercent);
}

bool PlantLeaf::updateSpring(f32 growthPercent) {
    // Float register alloc issues
    // https://decomp.me/scratch/oTGS1

    mSpringAccel -= mSpringVel * 0.1f;
    mSpringAccel = MR::clamp(mSpringAccel, -0.5f, 0.5f);

    mSpringVel += mSpringAccel;
    mSpringVel = MR::clamp(mSpringVel, -0.9f, 0.9f);

    // FIXME
    mSpringAccel *= 0.9f;

    if (__fabsf(mSpringVel) < 0.01f && __fabsf(mSpringAccel) < 0.001f) {
        mSpringVel = 0.0f;
        mSpringAccel = 0.0f;
        MR::makeAxisFrontUp(&mFront, &mUp, mSide, mUp);
        mBaseMtx.setXYZDir(mFront, mUp, mSide);
        mBaseMtx.scale(mLeafSize * growthPercent);
        mBaseMtx.setTrans(mPosition);
        return true;
    } else {
        TVec3f side(mSide);
        TVec3f front;
        TVec3f up;
        side.y += mSpringVel;
        MR::normalize(&side);
        MR::makeAxisFrontUp(&front, &up, side, mUp);
        mBaseMtx.setXYZDir(front, up, side);
        mBaseMtx.scale(mLeafSize * growthPercent);
        mBaseMtx.setTrans(mPosition);
        return false;
    }
}

void PlantLeafDrawInit::init(const JMapInfoIter&) {
    MR::invalidateClipping(this);
    makeActorDead();
}

void PlantLeafDrawInit::initDraw() const {
    J3DModelData* modelData = MR::getJ3DModelData(this);
    j3dSys._10C = modelData->mVertexData._18;
    j3dSys._110 = modelData->mVertexData._1C;
    j3dSys._114 = modelData->mVertexData._24;
    mShape->sOldVcdVatCmd = 0;
    mMaterial->loadSharedDL();
    mShape->loadPreDrawSetting();
    mShape->loadVtxArray();
}
