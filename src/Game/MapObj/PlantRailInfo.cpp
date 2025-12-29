#include "Game/MapObj/PlantRailInfo.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/MapObj/PlantPoint.hpp"
#include "Game/Util/MathUtil.hpp"

PlantRailInfo::PlantRailInfo(const JMapInfoIter& rIter, f32 speed)
    : mRailSpeed(speed), mNumPlantPoints(0), mPlantPoints(nullptr), mRailRider(nullptr) {
    mRailRider = new RailRider(rIter);
    mRailRider->setSpeed(mRailSpeed);

    mNumPlantPoints = static_cast< s32 >(mRailRider->getTotalLength() / mRailSpeed) + 2;

    mPlantPoints = new PlantPoint*[mNumPlantPoints];
    for (s32 idx = 0; idx < mNumPlantPoints - 1; idx++) {
        mPlantPoints[idx] = new PlantPoint(mRailRider->mCurPos, mRailRider->mCurDirection, 1.0f);
        mRailRider->move();
    }

    mRailRider->setCoord(mRailRider->getTotalLength());

    // compiler will optimize these lines if it isnt written *just right*...
    PlantPoint* point = new PlantPoint(mRailRider->mCurPos, getPlantPoint(mNumPlantPoints - 2)->mUp, 1.0f);
    s32 index = mNumPlantPoints - 1;
    mPlantPoints[index] = point;

    PlantPoint* firstPoint = getPlantPoint(0);
    PlantPoint* lastPoint = getPlantPoint(mNumPlantPoints - 2);
    firstPoint->setAxisAndPos(lastPoint->mSide, lastPoint->mUp, lastPoint->mFront, firstPoint->mPosition);
}

void PlantRailInfo::calcPosAndAxisY(TVec3f* pPos, TVec3f* pAxisY, f32 coord) const {
    // FIXME float register allocation mess
    // https://decomp.me/scratch/25d5r

    f32 normalized = (coord / mRailRider->getTotalLength()) * mNumPlantPoints;

    s32 start = normalized;
    s32 end = start + 1;
    if (start >= mNumPlantPoints) {
        start = mNumPlantPoints - 1;
    }
    if (end >= mNumPlantPoints) {
        end = mNumPlantPoints - 1;
    }

    f32 t = normalized - start;
    f32 negt = 1.0f - t;

    pPos->x = negt * mPlantPoints[start]->mPosition.x + t * mPlantPoints[end]->mPosition.x;
    pPos->y = negt * mPlantPoints[start]->mPosition.y + t * mPlantPoints[end]->mPosition.y;
    pPos->z = negt * mPlantPoints[start]->mPosition.z + t * mPlantPoints[end]->mPosition.z;

    pAxisY->x = negt * mPlantPoints[start]->mUp.x + t * mPlantPoints[end]->mUp.x;
    pAxisY->y = negt * mPlantPoints[start]->mUp.y + t * mPlantPoints[end]->mUp.y;
    pAxisY->z = negt * mPlantPoints[start]->mUp.z + t * mPlantPoints[end]->mUp.z;

    MR::normalize(pAxisY);
}

void PlantRailInfo::calcNearestPointPos(TVec3f* pPointPos, const TVec3f& rPos, s32 cutoff) const {
    s32 nearestIdx = 0;
    f32 closestMag = 10000000000.0f;
    for (s32 idx = cutoff; idx < mNumPlantPoints - cutoff; idx++) {
        f32 mag = mPlantPoints[idx]->mPosition.squared(rPos);

        if (mag < closestMag) {
            nearestIdx = idx;
            closestMag = mag;
        }
    }
    PlantPoint* nearestPoint = mPlantPoints[nearestIdx];
    pPointPos->x = nearestPoint->mPosition.x;
    pPointPos->y = nearestPoint->mPosition.y;
    pPointPos->z = nearestPoint->mPosition.z;
}
