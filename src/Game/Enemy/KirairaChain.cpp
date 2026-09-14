#include "Game/Enemy/KirairaChain.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/SimpleJ3DModelDrawer.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/J3DGraphBase/J3DShapeDraw.hpp>

KirairaChain::~KirairaChain() {
}

KirairaChain::KirairaChain(const LiveActor* pHost)
    : LiveActor("キライラの鎖"), mHost(pHost), mIsCut(), mCutPos(0.0f, 0.0f, 0.0f), mRailStart(0.0f, 0.0f, 0.0f), mRailEnd(0.0f, 0.0f, 0.0f),
      mFixPointTopDir(1.0f, 0.0f, 0.0f), mFixPointBottomDir(1.0f, 0.0f, 0.0f), mPointCount(), mPoints(), _138(), mAccelPointIdx(),
      mAccelTimer(), mAccel(0.0f, 0.0f, 0.0f), mClippingCenter(0.0f, 0.0f, 0.0f), mModelDrawer(), mFixPointTop(),
      mFixPointBottom() {
    mFixPointTopMtx.identity();
    mFixPointBottomMtx.identity();

    mPosition.set(pHost->mPosition);
    mRailStart.set(mPosition);
    mRailEnd.set(mPosition);

    f32 railLength;

    if (MR::isExistRail(mHost)) {
        TVec3f startDir;
        TVec3f endDir;
        MR::calcRailStartPointPosAndDirection(&mRailStart, &startDir, mHost);
        MR::calcRailEndPointPosAndDirection(&mRailEnd, &endDir, mHost);
        mPosition.set(mRailEnd);

        railLength = MR::getRailTotalLength(mHost);

        TVec3f up(0.0f, 0.0f, 1.0f);
        MR::makeMtxUpFront(&mFixPointTopMtx, startDir, up);
        endDir.scale(-1.0f);
        MR::makeMtxUpFront(&mFixPointBottomMtx, endDir, up);
        mFixPointTopMtx.setTrans(mRailStart);
        mFixPointBottomMtx.setTrans(mRailEnd);

        mClippingCenter.set(mRailStart);
        mClippingCenter.add(mRailEnd);
        mClippingCenter.x *= 0.5f;
        mClippingCenter.y *= 0.5f;
        mClippingCenter.z *= 0.5f;

        MR::setClippingTypeSphere(this, mClippingCenter.distance(mRailStart), &mClippingCenter);
    } else {
        mPosition.y -= 300.0f;
        railLength = mHost->mPosition.distance(mPosition);
        mFixPointBottomMtx.setTrans(mPosition);
    }

    mPointCount = railLength / 40.0f;

    if (!MR::isExistRail(mHost)) {
        mPointCount -= 2;
    }

    mFixPointTopMtx.getXDir(mFixPointTopDir);
    mFixPointBottomMtx.getXDir(mFixPointBottomDir);
}

void KirairaChain::initPoints() {
    mPoints = new SwingRopePoint*[mPointCount];

    for (s32 i = 0; i < mPointCount; i++) {
        f32 t = static_cast< f32 >(i) / (mPointCount - 1);
        TVec3f point;

        if (MR::isExistRail(mHost)) {
            point = mRailStart * (1.0f - t) + mRailEnd * t;
        } else {
            point = mPosition * (1.0f - t) + mHost->mPosition * t;
        }

        mPoints[i] = new SwingRopePoint(point);
    }
}

void KirairaChain::draw() const {
    mModelDrawer->initDraw();

    TPos3f mtx;
    mtx.identity();

    for (s32 i = 0; i < mPointCount; i++) {
        SwingRopePoint* point = mPoints[i];

        mtx.setXYZDir(point->mSide, point->mUp, point->mFront);

        mtx.setTrans(point->mPosition);

        PSMTXConcat(MR::getCameraViewMtx(), mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXLoadNrmMtxImm(mtx, GX_PNMTX0);
        GXCallDisplayList(mModelDrawer->mShapeDraw->getDisplayList(), mModelDrawer->mShapeDraw->getDisplayListSize());
    }
}

void KirairaChain::updatePoints() {
    TVec3f accel(mGravity);

    if (MR::isExistRail(mHost)) {
        accel.scale(0.05f);
    } else {
        accel.scale(0.3f);
    }

    for (s32 i = 0; i < mPointCount; i++) {
        mPoints[i]->addAccel(accel);
    }

    if (MR::isExistRail(mHost)) {
        f32 offset = 70.0f;
        if (mIsCut) {
            offset = 0.0f;
        }

        s32 lowIdx = MR::clamp(static_cast< s32 >((MR::getRailCoord(mHost) - offset) / 40.0f), 0, mPointCount - 1);
        s32 highIdx = MR::clamp(static_cast< s32 >((offset + MR::getRailCoord(mHost)) / 40.0f), 0, mPointCount - 1);

        TVec3f posA;
        MR::calcRailPosAtCoord(&posA, mHost, 40.0f * (lowIdx + 1));
        TVec3f posB;
        MR::calcRailPosAtCoord(&posB, mHost, 40.0f * highIdx);

        if (lowIdx + 1 < highIdx - 1) {
            restrictPointFromBottom(lowIdx + 1, highIdx - 1, posB, 1.0f);
            restrictPointFromTopAndCalcAxis(lowIdx + 1, highIdx - 1, posA, 0.95f);
        }

        if (!mIsCut && lowIdx < mPointCount - 1) {
            s32 idx = lowIdx + 1;
            posA.set(mPoints[idx]->mPosition);
        }

        if (lowIdx > 0) {
            restrictPointFromBottom(0, lowIdx, posA, 1.0f);
            restrictPointFromTopAndCalcAxis(0, lowIdx, mRailStart, 0.95f);
        }

        if (lowIdx > 0 && lowIdx < mPointCount - 1) {
            s32 nextIdx = lowIdx + 1;
            mPoints[lowIdx]->restrict(mPoints[nextIdx]->mPosition, 40.0f, nullptr);
            s32 prevIdx = lowIdx - 1;
            mPoints[lowIdx]->updatePosAndAxis(mPoints[prevIdx]->mSide, 0.95f);
        }

        if (!mIsCut && highIdx > 0) {
            s32 idx = highIdx - 1;
            posB.set(mPoints[idx]->mPosition);
        }

        s32 lastPointIdx = mPointCount - 1;
        if (highIdx < lastPointIdx) {
            restrictPointFromTop(highIdx, lastPointIdx, posB, 1.0f);
            restrictPointFromBottomAndCalcAxis(highIdx, lastPointIdx, mRailEnd, 0.95f);
        }

        if (mIsCut) {
            s32 lastIdx = mPointCount - 1;
            if (lowIdx == lastIdx) {
                mCutPos.set(mPoints[lastIdx]->mPosition);
            } else if (highIdx == 0) {
                mCutPos.set(mPoints[0]->mPosition);
            } else {
                s32 nextIdx = lowIdx + 1;
                mCutPos.set(mPoints[nextIdx]->mPosition);
                s32 prevIdx = highIdx - 1;
                mCutPos.add(mPoints[prevIdx]->mPosition);
                mCutPos.x *= 0.5f;
                mCutPos.y *= 0.5f;
                mCutPos.z *= 0.5f;
            }
        }
    } else {
        MtxPtr baseMtx = mHost->getBaseMtx();
        TVec3f dir(baseMtx[1][0], baseMtx[1][1], baseMtx[1][2]);
        dir.scale(-100.0f);
        dir.add(mHost->mPosition);
        restrictPointFromTopAndCalcAxis(0, mPointCount - 1, dir, 0.9f);
    }
}

void KirairaChain::control() {
    if (mAccelTimer <= 0) {
        mAccelPointIdx = MR::getRandom(0L, mPointCount - 1);
        if (MR::isExistRail(mHost)) {
            mAccelTimer = MR::getRandom(3L, 10L);
        } else {
            mAccelTimer = MR::getRandom(5L, 10L);
        }

        mAccel.set< f32 >(MR::getRandom(-0.3f, 0.3f), MR::getRandom(-0.3f, 0.3f), MR::getRandom(-0.3f, 0.3f));
    }

    mAccelTimer--;
    mPoints[mAccelPointIdx]->addAccel(mAccel);
    updatePoints();
}

void KirairaChain::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_KirairaChain);
    initPoints();

    mModelDrawer = new SimpleJ3DModelDrawer(this, "キライラの鎖描画", "KirairaChain", -1);
    mModelDrawer->initWithoutIter();

    mFixPointTop = MR::createModelObjEnemy("キライラの固定点[Top]", "KirairaFixPointTop", mFixPointTopMtx.toMtxPtr());
    mFixPointBottom = MR::createModelObjEnemy("キライラの固定点[Bottom]", "KirairaFixPointBottom", mFixPointBottomMtx.toMtxPtr());

    mFixPointTop->appear();
    mFixPointBottom->appear();
    makeActorAppeared();
}

void KirairaChain::restrictPointFromTop(s32 startIdx, s32 endIdx, const TVec3f& rPos, f32 radius) {
    mPoints[startIdx]->restrict(rPos, 40.0f, nullptr);
    mPoints[startIdx]->updatePos(radius);

    for (s32 i = startIdx + 1; i <= endIdx; i++) {
        bool isRestricted = mPoints[i]->restrict(mPoints[i - 1]->mPosition, 40.0f, nullptr);
        mPoints[i]->updatePos(radius);

        if (!isRestricted) {
            break;
        }
    }
}

void KirairaChain::restrictPointFromBottom(s32 startIdx, s32 endIdx, const TVec3f& rPos, f32 radius) {
    mPoints[endIdx]->restrict(rPos, 40.0f, nullptr);
    mPoints[endIdx]->updatePos(radius);

    for (s32 i = endIdx - 1; i >= startIdx; i--) {
        bool isRestricted = mPoints[i]->restrict(mPoints[i + 1]->mPosition, 40.0f, nullptr);
        mPoints[i]->updatePos(radius);

        if (!isRestricted) {
            break;
        }
    }
}

void KirairaChain::restrictPointFromTopAndCalcAxis(s32 startIdx, s32 endIdx, const TVec3f& rPos, f32 radius) {
    mPoints[startIdx]->restrict(rPos, 40.0f, nullptr);
    mPoints[startIdx]->updatePosAndAxis(mFixPointTopDir, radius);

    for (s32 i = startIdx + 1; i <= endIdx; i++) {
        mPoints[i]->restrict(mPoints[i - 1]->mPosition, 40.0f, nullptr);
        mPoints[i]->updatePosAndAxis(mPoints[i - 1]->mSide, radius);
    }
}

void KirairaChain::restrictPointFromBottomAndCalcAxis(s32 startIdx, s32 endIdx, const TVec3f& rPos, f32 radius) {
    mPoints[endIdx]->restrict(rPos, 40.0f, nullptr);
    mPoints[endIdx]->updatePosAndAxis(mFixPointBottomDir, radius);

    for (s32 i = endIdx - 1; i >= startIdx; i--) {
        mPoints[i]->restrict(mPoints[i + 1]->mPosition, 40.0f, nullptr);
        mPoints[i]->updatePosAndAxis(mPoints[i + 1]->mSide, radius);
    }
}
