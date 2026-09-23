#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

void MercatorTransformCube_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)0.000003814697265625f;
    (void)3.141592653589793f;
    (void)1.5707963267948966f;
    (void)2.0f;
    (void)-0.000003814697265625f;
    (void)-1.5707963267948966f;
}

namespace {
    MercatorTransformCube* getMercatorCube() {
        return static_cast< MercatorTransformCube* >(MR::getAreaObjManager("MercatorCube")->getAreaObj(0));
    }

    void calcRailPosForMercator(TVec3f* pPos, const LiveActor* pActor, f32 coord) {
        TPos3f rotation;
        TVec3f railPos;
        MR::calcRailPosAtCoord(&railPos, pActor, coord);
        getMercatorCube()->convertTransAndRotate(pPos, &rotation, railPos, true);
    }

}  // namespace

void MercatorTransformCube_FORCE_EMIT() {
    TVec3f vec;
    vec *= 0.0f;
}

MercatorTransformCube::MercatorTransformCube(int formType, const char* pName) : AreaObj(formType, pName) {
    _3C.identity();
}

void MercatorTransformCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    _3C.setInline(*MR::getZonePlacementMtx(rIter));
}

f32 MercatorTransformCube::calcLocalHeight(const TVec3f& rPos) const {
    TVec3f localPos;
    MR::calcCubeLocalPos(&localPos, this, rPos);
    return localPos.y;
}

void MercatorTransformCube::convertTransAndRotate(TVec3f* pPos, TPos3f* pRotation, const TVec3f& rPos, bool applyInverse) const {
    TVec2f uv;
    calcUV(&uv, rPos);
    TVec3f point;
    f32 radius = getSphereRadius() * MR::cos(uv.y);
    point.x = radius * MR::sin(uv.x);
    point.y = getSphereRadius() * MR::sin(uv.y);
    point.z = radius * MR::cos(uv.x);
    TVec3f offset(point);
    MR::normalize(&offset);
    offset *= calcLocalHeight(rPos);
    point.add(offset);
    TVec3f up(0.0f, 1.0f, 0.0f);
    TVec3f front;
    if (MR::isSameDirection(point, up, 0.01f)) {
        front.set(0.0f, 0.0f, MR::sign(point.y));
    } else {
        TVec3f side;
        side.cross(point, up);
        front.cross(point, side);
    }

    MR::makeMtxUpFront(pRotation, point, front);
    _3C.mult(point, point);
    TMtx34f rotation(_3C);
    rotation.mMtx[0][3] = 0.0f;
    rotation.mMtx[1][3] = 0.0f;
    rotation.mMtx[2][3] = 0.0f;
    if (applyInverse) {
        TMtx34f inverse;
        inverse.invert(rotation);
        pRotation->concat(*pRotation, inverse);
    }

    pRotation->concat(rotation, *pRotation);
    pPos->set(point);
}

f32 MercatorTransformCube::getSphereRadius() const {
    if (mObjArg0 == -1) {
        return 3000.0f;
    }

    return mObjArg0;
}

void MercatorTransformCube::calcUV(TVec2f* pUV, const TVec3f& rPos) const {
    TVec3f boxSize;
    calcLocalBoxSize(&boxSize);
    boxSize *= 0.5f;

    TVec3f localPos;
    MR::calcCubeLocalPos(&localPos, this, rPos);

    pUV->x = PI * localPos.x / boxSize.x;
    pUV->y = HALF_PI * -localPos.z / boxSize.z;
}

void MercatorTransformCube::calcLocalBoxSize(TVec3f* pPos) const {
    TBox3f* box = MR::getCubeLocalBox(this);

    TVec3f max;
    TVec3f min;

    min.set(box->i);
    max.set(box->f);

    pPos->sub(max, min);
}

namespace MR {
    void convertMercatorPlaneToSphereTransAndRotate(TVec3f* pPos, TPos3f* pRotation, const TVec3f& rPos, bool applyInverse) {
        ::getMercatorCube()->convertTransAndRotate(pPos, pRotation, rPos, applyInverse);
    }

    void convertMercatorPlaneToSphereTransAndRotate(TVec3f* pPos, TVec3f* pRotation, const TVec3f& rPos, bool applyInverse) {
        TPos3f rotation;
        ::getMercatorCube()->convertTransAndRotate(pPos, &rotation, rPos, applyInverse);
        TVec3f angles;
        rotation.getEulerXYZ(angles);
        pRotation->set< f32 >(57.29577951308232f * angles.x, 57.29577951308232f * angles.y, 57.29577951308232f * angles.z);
    }

    void initDefaultPosForMercator(LiveActor* pActor, const JMapInfoIter& rIter, bool applyInverse) {
        getJMapInfoTrans(rIter, &pActor->mPosition);
        TVec3f angles;
        getJMapInfoRotate(rIter, &angles);
        TPos3f rotation;
        makeMtxRotate(rotation, angles.x, angles.y, angles.z);
        TVec3f pos;
        TPos3f sphereRotation;
        ::getMercatorCube()->convertTransAndRotate(&pos, &sphereRotation, pActor->mPosition, applyInverse);
        rotation.concat(sphereRotation, rotation);
        rotation.getEulerXYZ(angles);
        angles.set< f32 >(57.29577951308232f * angles.x, 57.29577951308232f * angles.y, 57.29577951308232f * angles.z);
        pActor->mPosition.set(pos);
        pActor->mRotation.set(angles);
        getJMapInfoScale(rIter, &pActor->mScale);
    }

    void calcNearestRailPosForMercator(TVec3f* pPos, const LiveActor* pActor, f32 step) {
        pPos->zero();
        f32 nearest = 3.4028234663852886e38f;
        for (f32 coord = 0.0f; coord <= getRailTotalLength(pActor); coord += step) {
            TVec3f pos;
            ::calcRailPosForMercator(&pos, pActor, coord);
            f32 distance = pos.distance(pActor->mPosition);
            if (nearest > distance) {
                *pPos = pos;
                nearest = distance;
            }
        }
    }

    void calcRailClippingInfoForMercator(TVec3f* pCenter, f32* pRadius, LiveActor* pActor, f32 step, f32 padding) {
        f32 length = getRailTotalLength(pActor);
        s32 count = length / step;
        TVec3f start;
        ::calcRailPosForMercator(&start, pActor, 0.0f);
        TVec3f end;
        ::calcRailPosForMercator(&end, pActor, length);
        TBox3f box;
        box.set(TVec3f(min(start.x, end.x), min(start.y, end.y), min(start.z, end.z)),
                TVec3f(max(start.x, end.x), max(start.y, end.y), max(start.z, end.z)));
        for (s32 i = 1; i < count; i++) {
            TVec3f pos;
            ::calcRailPosForMercator(&pos, pActor, i * step);
            box.extend(pos);
        }

        TVec3f pad(padding);
        box.i.sub(pad);
        box.f.add(pad);
        box.getCenter(pCenter);
        TVec3f size;
        size.sub(box.f, box.i);
        *pRadius = 0.5f * size.length();
    }

    void initAndSetRailClippingForMercator(TVec3f* pCenter, LiveActor* pActor, f32 step, f32 padding) {
        f32 radius = 0.0f;
        calcRailClippingInfoForMercator(pCenter, &radius, pActor, step, padding);
        setClippingTypeSphere(pActor, radius, pCenter);
    }

    f32 calcRailTotalLengthForMercator(const LiveActor* pActor, f32 step) {
        f32 length = getRailTotalLength(pActor);
        s32 count = length / step;
        TVec3f points[2];
        ::calcRailPosForMercator(&points[0], pActor, 0.0f);
        f32 total = 0.0f;
        for (s32 i = 1; i <= count; i++) {
            ::calcRailPosForMercator(&points[i % 2], pActor, length * i / count);
            total += points[0].distance(points[1]);
        }

        return total;
    }

    void getDivideMercatorRailPosition(DivideMercatorRailPosInfo* pInfo, const LiveActor* pActor, u32 count, f32 tolerance, u32 maxIterations) {
        if (count <= 1) {
            TVec3f pos;
            ::calcRailPosForMercator(&pos, pActor, 0.0f);
            pInfo->setPosition(0, pos);
            return;
        }

        f32 total = calcRailTotalLengthForMercator(pActor, 100.0f);
        f32 railSpacing;
        f32 spacing;
        if (isLoopRail(pActor)) {
            spacing = total / count;
        } else {
            spacing = total / (count - 1);
        }

        f32 railLength = getRailTotalLength(pActor);
        if (isLoopRail(pActor)) {
            railSpacing = railLength / count;
        } else {
            railSpacing = railLength / (count - 1);
        }

        f32 distance = 0.0f;
        f32 targetDistance = spacing;
        f32 lower = 0.0f;
        f32 coord = railSpacing;
        f32 upper = 2.0f * railSpacing;
        TVec3f previous;
        ::calcRailPosForMercator(&previous, pActor, 0.0f);
        pInfo->setPosition(0, previous);
        for (s32 i = 1; i < count; i++) {
            bool bracketed = false;
            u32 iteration = 0;
            while (iteration < maxIterations) {
                TVec3f pos;
                ::calcRailPosForMercator(&pos, pActor, coord);
                f32 segment = previous.distance(pos);
                f32 remaining = targetDistance - (distance + segment);
                if (MR::abs(remaining) < tolerance || iteration == maxIterations) {
                    pInfo->setPosition(i, pos);
                    previous = pos;
                    lower = coord;
                    coord += railSpacing;
                    distance += segment;
                    upper = coord + railSpacing;
                    targetDistance = spacing * (i + 1);
                    if (targetDistance < distance) {
                        targetDistance = distance;
                    }

                    break;
                }

                if (remaining > 0.0f) {
                    lower = coord;
                    if (!bracketed) {
                        upper += 2.0f * railSpacing;
                    }

                    coord = 0.5f * (coord + upper);
                    distance += segment;
                    previous = pos;
                } else {
                    if (!bracketed) {
                        bracketed = true;
                    }

                    upper = coord;
                    coord = 0.5f * (lower + coord);
                }

                if (bracketed) {
                    iteration++;
                }
            }
        }
    }

    MapPartsRailMover* createMapPartsRailMoverForMercator(LiveActor* pActor, const JMapInfoIter& rIter, bool setRailPos) {
        TVec3f savedPos(pActor->mPosition);
        TVec3f placementPos;
        getJMapInfoTrans(rIter, &placementPos);
        pActor->mPosition.set(placementPos);
        MapPartsRailMover* pMover = new MapPartsRailMover(pActor);
        pMover->init(rIter);
        if (setRailPos) {
            ::calcRailPosForMercator(&pActor->mPosition, pActor, getRailCoord(pActor));
        } else {
            pActor->mPosition.set(savedPos);
        }

        return pMover;
    }
}  // namespace MR

MercatorTransformCube::~MercatorTransformCube() {
}
