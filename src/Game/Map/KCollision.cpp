#include "Game/Map/KCollision.hpp"
#include "Game/Camera/CameraPolygonCodeUtil.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Map/CollisionDirector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MathUtil.hpp"
#include <algorithm>

// trying to get both operator== and operator/ to emit
void DUMMY_KCollision() {
    TVec3f a, b, c;

    if (a == b) {
        c = a / b.x;
    }
}

void DUMMY_KCollision2() {
    TVec3f a, b, c;

    if (a == b) {
        c = a / b.x;
    }
}

KCollisionServer::KCollisionServer() : mFile(), mapInfo(new JMapInfo()), mMaxVertexDistance(1.0f) {
}

void KCollisionServer::init(void* pData, const void* pMapData) {
    setData(pData);

    if (pMapData != nullptr) {
        mapInfo->attach(pMapData);
    }
}

void KCollisionServer::setData(void* pData) {
    mFile = reinterpret_cast< KCLFile* >(pData);

    if (!isBinaryInitialized(pData)) {
        mFile->mPos = reinterpret_cast< TVec3f* >(reinterpret_cast< u8* >(mFile) + mFile->mPosOffset);
        mFile->mNorms = reinterpret_cast< TVec3f* >(reinterpret_cast< u8* >(mFile) + mFile->mNormOffset);
        mFile->mPrisms = reinterpret_cast< KC_PrismData* >(reinterpret_cast< u8* >(mFile) + mFile->mPrismOffset);
        mFile->mOctree = reinterpret_cast< void* >(reinterpret_cast< u8* >(mFile) + mFile->mOctreeOffset);
    }
}

bool KCollisionServer::calcFarthestVertexDistance() {
    s32 triCount = getTriangleNum();
    f32 maxDistance = 0.0f;
    KC_PrismData* prism;
    bool result = true;

    for (u32 i = 0; i < (u32)triCount; i++) {
        prism = &mFile->mPrisms[i + 1];
        JMapInfoIter iter = getAttributes(i);

        if (!iter.isValid()) {
            result = false;
        } else {
            MR::registerCameraCode(MR::getCollisionDirector()->mCode->getCameraID(iter));
        }

        if (isNearParallelNormal(prism)) {
            prism->mHeight = -MR::abs(prism->mHeight);
        } else {
            for (s32 j = 0; j < 3; j++) {
                TVec3f pos = getPos(prism, j);
                f32 distSq = pos.squared();

                if (maxDistance < distSq) {
                    maxDistance = distSq;
                }
            }
        }
    }

    mMaxVertexDistance = MR::sqrt(maxDistance);
    return result;
}

bool KCollisionServer::isBinaryInitialized(const void* pData) NO_INLINE {
    return reinterpret_cast< const s32* >(pData)[0] < 0;
}

KC_PrismData* KCollisionServer::checkPoint(Fxyz* pPoint, f32 param, f32* pDist) {
    f32 maxDist = mFile->mThickness * param;
    u32 x = (s32)(pPoint->x - mFile->mMin.x);

    if (x & mFile->mXMask) {
        return nullptr;
    }

    u32 y = (s32)(pPoint->y - mFile->mMin.y);

    if (y & mFile->mYMask) {
        return nullptr;
    }

    u32 z = (s32)(pPoint->z - mFile->mMin.z);

    if (z & mFile->mZMask) {
        return nullptr;
    }

    s32 shift;
    u16* prismList = (u16*)searchBlock(&shift, x, y, z);

    while (*++prismList != 0) {
        KC_PrismData* prism = &mFile->mPrisms[*prismList];
        f32 height = prism->mHeight;

        if (height <= 0.0f) {
            continue;
        }

        TVec3f* vtx = &mFile->mPos[prism->mPositionIndex];

        Fxyz dir;
        dir.x = pPoint->x - vtx->x;
        dir.y = pPoint->y - vtx->y;
        dir.z = pPoint->z - vtx->z;

        TVec3f& edgeNormal0 = mFile->mNorms[prism->mEdgeIndices[0]];

        if (dir.x * edgeNormal0.x + dir.y * edgeNormal0.y + dir.z * edgeNormal0.z > 0.0f) {
            continue;
        }

        TVec3f& edgeNormal1 = mFile->mNorms[prism->mEdgeIndices[1]];

        if (dir.x * edgeNormal1.x + dir.y * edgeNormal1.y + dir.z * edgeNormal1.z > 0.0f) {
            continue;
        }

        TVec3f& edgeNormal2 = mFile->mNorms[prism->mEdgeIndices[2]];

        if (dir.x * edgeNormal2.x + dir.y * edgeNormal2.y + dir.z * edgeNormal2.z > height) {
            continue;
        }

        TVec3f& faceNormal = mFile->mNorms[prism->mNormalIndex];
        f32 dist = -dir.x * faceNormal.x - dir.y * faceNormal.y - dir.z * faceNormal.z;

        if (dist < 0.0f) {
            continue;
        }

        if (maxDist < dist) {
            continue;
        }

        *pDist = dist;
        return prism;
    }

    return nullptr;
}

u32 KCollisionServer::checkArea3D(Fxyz* pMin, Fxyz* pMax, KC_PrismData** pOut, u32 maxCount) {
    s32 remX;
    s32 yStep;
    s32 zStep;
    s32 bestRemY;
    u16* list;
    u16* nextSkipList = nullptr;
    u16* skipList = nullptr;
    u32 foundCount = 0;

    Fxyz box[2];
    box[0] = *pMin;
    box[1] = *pMax;

    V3u pointMin;
    V3u pointMax;
    TVec3f queryMax;
    TVec3f queryMin;
    MR::createBoundingBox((TVec3f*)box, 2, &queryMin, &queryMax);

    if (queryMin.x == queryMax.x) {
        queryMin.x -= 1.0f;
        queryMax.x += 1.0f;
    }

    if (queryMin.y == queryMax.y) {
        queryMin.y -= 1.0f;
        queryMax.y += 1.0f;
    }

    if (queryMin.z == queryMax.z) {
        queryMin.z -= 1.0f;
        queryMax.z += 1.0f;
    }

    if (!outCheck(&queryMin, &queryMax, &pointMin, &pointMax)) {
        return 0;
    }

    u32 x;
    u32 y;
    u32 z;
    KC_PrismData* prism;
    s32 shift;
    z = pointMin.z;

    do {
        y = pointMin.y;
        zStep = 1000000;

        do {
            x = pointMin.x;
            yStep = 1000000;
            bestRemY = 0;

            do {
                list = reinterpret_cast< u16* >(searchBlock(&shift, x, y, z));
                shift = 1 << shift;
                s32 blockSize = shift;
                s32 remZ;
                s32 mask = blockSize - 1;
                remX = blockSize - (x & mask);
                s32 remY = blockSize - (y & mask);
                remZ = blockSize - (z & mask);

                if (remZ < zStep) {
                    zStep = remZ;
                }

                if (remY < yStep) {
                    yStep = remY;
                }

                if (remY > bestRemY && list[1] != 0) {
                    bestRemY = remY;
                    nextSkipList = list;
                }

                if (skipList == nullptr || list != skipList) {
                    while (*++list != 0) {
                        prism = &mFile->mPrisms[*list];

                        if (prism->mHeight <= 0.0f) {
                            continue;
                        }

                        KC_PrismData** end = pOut + foundCount;

                        if (std::find(pOut, end, (KC_PrismData* const&)prism) != end) {
                            continue;
                        }

                        TVec3f verts[3];
                        verts[0] = getPos(prism, 0);
                        verts[1] = getPos(prism, 1);
                        verts[2] = getPos(prism, 2);

                        TVec3f prismMin;
                        TVec3f prismMax;
                        MR::createBoundingBox(verts, 3, &prismMin, &prismMax);

                        if (prismMax.x < queryMin.x) {
                            continue;
                        }

                        if (prismMax.y < queryMin.y) {
                            continue;
                        }

                        if (prismMax.z < queryMin.z) {
                            continue;
                        }

                        if (queryMax.x < prismMin.x) {
                            continue;
                        }

                        if (queryMax.y < prismMin.y) {
                            continue;
                        }

                        if (queryMax.z < prismMin.z) {
                            continue;
                        }

                        *end = prism;
                        foundCount++;

                        if (foundCount == maxCount) {
                            return maxCount;
                        }
                    }
                }

                x += remX;
            } while (x <= pointMax.x);

            skipList = nextSkipList;
            y += yStep;
        } while (y <= pointMax.y);

        z += zStep;
    } while (z <= pointMax.z);

    return foundCount;
}

u32 KCollisionServer::checkSphere(Fxyz* pCenter, f32 radius, f32 param, u32 maxCount, KC_PrismData** pOut, f32* pDists, u8* pFlags) {
    KC_PrismData** end;
    s32 remX;
    s32 yStep;
    s32 zStep;
    s32 bestRemY;
    u16* list;
    u16* nextSkipList = nullptr;
    u16* skipList = nullptr;
    u32 foundCount = 0;

    V3u pointMin;
    V3u pointMax;
    TVec3f boxMax;
    TVec3f boxMin;
    u32 x;
    u32 y;
    u32 z;
    KC_PrismData* prism;
    f32 dist = 0.0f;
    s32 shift;
    u8 flag;
    boxMax.x = pCenter->x + radius;
    boxMax.y = pCenter->y + radius;
    boxMax.z = pCenter->z + radius;

    boxMin.x = pCenter->x - radius;
    boxMin.y = pCenter->y - radius;
    boxMin.z = pCenter->z - radius;

    if (!outCheck(&boxMin, &boxMax, &pointMin, &pointMax)) {
        return 0;
    }

    z = pointMin.z;

    do {
        y = pointMin.y;
        zStep = 1000000;

        do {
            x = pointMin.x;
            yStep = 1000000;
            bestRemY = 0;

            do {
                list = reinterpret_cast< u16* >(searchBlock(&shift, x, y, z));
                s32 remZ;
                s32 blockSize = 1 << shift;
                s32 mask = blockSize - 1;
                remX = blockSize - (x & mask);
                s32 remY = blockSize - (y & mask);
                remZ = blockSize - (z & mask);

                if (remZ < zStep) {
                    zStep = remZ;
                }

                if (remY < yStep) {
                    yStep = remY;
                }

                if (remY > bestRemY && list[1] != 0) {
                    bestRemY = remY;
                    nextSkipList = list;
                }

                if (skipList == nullptr || list != skipList) {
                    while (*++list != 0) {
                        prism = &mFile->mPrisms[*list];

                        if (prism->mHeight <= 0.0f) {
                            continue;
                        }

                        end = pOut + foundCount;

                        if (std::find(pOut, end, (KC_PrismData* const&)prism) != end) {
                            continue;
                        }

                        if (!KCHitSphere(prism, pCenter, radius, param, &dist, &flag)) {
                            continue;
                        }

                        if (foundCount >= maxCount) {
                            continue;
                        }

                        if (std::find(pOut, end, (KC_PrismData* const&)prism) != end) {
                            continue;
                        }

                        *end = prism;
                        pDists[foundCount] = dist;
                        pFlags[foundCount] = flag;
                        foundCount++;
                    }
                }

                x += remX;
            } while (x <= pointMax.x);

            skipList = nextSkipList;
            y += yStep;
        } while (y <= pointMax.y);

        z += zStep;
    } while (z <= pointMax.z);

    return foundCount;
}

u32 KCollisionServer::checkSphereWithThickness(Fxyz* pCenter, f32 radius, f32 param, u32 maxCount, KC_PrismData** pOut, f32* pDists, u8* pFlags,
                                               f32 thickness) {
    KC_PrismData** end;
    s32 remX;
    s32 yStep;
    s32 zStep;
    s32 bestRemY;
    u16* list;
    u16* nextSkipList = nullptr;
    u16* skipList = nullptr;
    u32 foundCount = 0;

    V3u pointMin;
    V3u pointMax;
    TVec3f boxMax;
    TVec3f boxMin;
    u32 x;
    u32 y;
    u32 z;
    KC_PrismData* prism;
    f32 dist = 0.0f;
    s32 shift;
    u8 flag;
    boxMax.x = pCenter->x + radius;
    boxMax.y = pCenter->y + radius;
    boxMax.z = pCenter->z + radius;

    boxMin.x = pCenter->x - radius;
    boxMin.y = pCenter->y - radius;
    boxMin.z = pCenter->z - radius;

    if (!outCheck(&boxMin, &boxMax, &pointMin, &pointMax)) {
        return 0;
    }

    z = pointMin.z;

    do {
        y = pointMin.y;
        zStep = 1000000;

        do {
            x = pointMin.x;
            yStep = 1000000;
            bestRemY = 0;

            do {
                list = reinterpret_cast< u16* >(searchBlock(&shift, x, y, z));
                s32 remZ;
                s32 blockSize = 1 << shift;
                s32 mask = blockSize - 1;
                remX = blockSize - (x & mask);
                s32 remY = blockSize - (y & mask);
                remZ = blockSize - (z & mask);

                if (remZ < zStep) {
                    zStep = remZ;
                }

                if (remY < yStep) {
                    yStep = remY;
                }

                if (remY > bestRemY && list[1] != 0) {
                    bestRemY = remY;
                    nextSkipList = list;
                }

                if (skipList == nullptr || list != skipList) {
                    while (*++list != 0) {
                        prism = &mFile->mPrisms[*list];

                        if (prism->mHeight <= 0.0f) {
                            continue;
                        }

                        end = pOut + foundCount;

                        if (std::find(pOut, end, (KC_PrismData* const&)prism) != end) {
                            continue;
                        }

                        if (!KCHitSphereWithThickness(prism, pCenter, radius, param, &dist, &flag, thickness)) {
                            continue;
                        }

                        if (foundCount >= maxCount) {
                            continue;
                        }

                        if (std::find(pOut, end, (KC_PrismData* const&)prism) != end) {
                            continue;
                        }

                        *end = prism;
                        pDists[foundCount] = dist;
                        pFlags[foundCount] = flag;
                        foundCount++;
                    }
                }

                x += remX;
            } while (x <= pointMax.x);

            skipList = nextSkipList;
            y += yStep;
        } while (y <= pointMax.y);

        z += zStep;
    } while (z <= pointMax.z);

    return foundCount;
}

KC_PrismData* KCollisionServer::checkArrow(const TVec3f& rOrigin, const TVec3f& rDir, f32* pDists, u8* pFlags, u32* pCount, KC_PrismData** pOut,
                                           u32 maxCount) const {
    if (rDir.x == 0.0f && rDir.y == 0.0f && rDir.z == 0.0f) {
        return nullptr;
    }

    f32 length;
    TVec3f dir(rDir);
    MR::separateScalarAndDirection(&length, &dir, dir);

    if (MR::isNearZero(dir)) {
        return nullptr;
    }

    TVec3f start(rOrigin);
    start.x -= mFile->mMin.x;
    start.y -= mFile->mMin.y;
    start.z -= mFile->mMin.z;

    V3u cell;
    cell.setUsingCast(start);

    TVec3f hitPoint;
    f32 startT = 0.0f;

    if (isInsideMinMaxInLocalSpace(cell)) {
        hitPoint.set(start);
    } else {
        TVec3f boxMax;
        boxMax.x = (f32)(u32)~mFile->mXMask;
        boxMax.y = (f32)(u32)~mFile->mYMask;
        boxMax.z = (f32)(u32)~mFile->mZMask;

        if (dir.x != 0.0f) {
            f32 edge = dir.x <= 0.0f ? boxMax.x : 0.0f;
            startT = (edge - start.x) / dir.x;

            if (startT >= 0.0f && startT <= length) {
                TVec3f step(dir);
                step.scale(startT);
                hitPoint.set(step);
                hitPoint += start;
                cell.setUsingCast(hitPoint);
                if (isInsideMinMaxInLocalSpace(cell)) {
                    goto searchStart;
                }
            }
        }

        if (dir.y != 0.0f) {
            f32 edge = dir.y <= 0.0f ? boxMax.y : 0.0f;
            startT = (edge - start.y) / dir.y;

            if (startT >= 0.0f && startT <= length) {
                TVec3f step(dir);
                step.scale(startT);
                hitPoint.set(step);
                hitPoint += start;
                cell.setUsingCast(hitPoint);
                if (isInsideMinMaxInLocalSpace(cell)) {
                    goto searchStart;
                }
            }
        }

        if (dir.z != 0.0f) {
            f32 edge = dir.z <= 0.0f ? boxMax.z : 0.0f;
            startT = (edge - start.z) / dir.z;

            if (startT >= 0.0f && startT <= length) {
                TVec3f step(dir);
                step.scale(startT);
                hitPoint.set(step);
                hitPoint += start;
                cell.setUsingCast(hitPoint);
                if (isInsideMinMaxInLocalSpace(cell)) {
                    goto searchStart;
                }
            }
        }

        return nullptr;
    }

searchStart:
    u32 foundCount = 0;
    KC_PrismData* bestPrism = nullptr;

    s32 stepX = dir.x < 0.0f ? -1 : 1;
    s32 stepY = dir.y < 0.0f ? -1 : 1;
    s32 stepZ = dir.z < 0.0f ? -1 : 1;

    f32 accumT = startT;
    f32 bestFraction = 1.0f;

    s32 shift;

    do {
        s32* list = searchBlock(&shift, cell.x, cell.y, cell.z);
        u32 blockSize = 1 << shift;
        u32 mask = blockSize - 1;

        s32 deltaPosX = blockSize - (cell.x & mask);
        s32 deltaPosY = blockSize - (cell.y & mask);
        s32 deltaPosZ = blockSize - (cell.z & mask);
        s32 deltaNegX = -(s32)(cell.x & mask);
        s32 deltaNegY = -(s32)(cell.y & mask);
        s32 deltaNegZ = -(s32)(cell.z & mask);

        s32 deltaX = stepX < 0 ? deltaNegX : deltaPosX;
        s32 deltaY = stepY < 0 ? deltaNegY : deltaPosY;
        s32 deltaZ = stepZ < 0 ? deltaNegZ : deltaPosZ;

        if (deltaX == 0) {
            deltaX = stepX;
        }

        if (deltaY == 0) {
            deltaY = stepY;
        }

        if (deltaZ == 0) {
            deltaZ = stepZ;
        }

        u16* prismList = (u16*)list;

        while (*++prismList != 0) {
            KC_PrismData* prism = &mFile->mPrisms[*prismList];

            if (prism->mHeight <= 0.0f) {
                continue;
            }

            f32 dist;
            u8 flag = 0;

            if (!KCHitArrow(prism, rOrigin, rDir, &dist, &flag)) {
                continue;
            }

            if (pOut != nullptr) {
                foundCount++;
                pDists[foundCount - 1] = dist;
                pOut[foundCount - 1] = prism;

                if (dist < bestFraction) {
                    bestFraction = dist;
                    bestPrism = prism;
                }

                if (foundCount == maxCount) {
                    if (pCount != nullptr) {
                        *pCount = foundCount;
                    }

                    return bestPrism;
                }
            } else {
                if (dist >= bestFraction) {
                    continue;
                }

                *pDists = dist;
                bestFraction = dist;
                bestPrism = prism;
                *pFlags = flag;
            }
        }

        if (pOut == nullptr && bestPrism != nullptr) {
            break;
        }

        f32 tX = MR::isNearZero(dir.x) ? 1.0e9f : (f32)deltaX / dir.x;
        f32 tY = MR::isNearZero(dir.y) ? 1.0e9f : (f32)deltaY / dir.y;
        f32 tZ = MR::isNearZero(dir.z) ? 1.0e9f : (f32)deltaZ / dir.z;

        f32 tMin = tX;

        if (tY < tMin) {
            tMin = tY;
        }

        if (tZ < tMin) {
            tMin = tZ;
        }

        if (length - accumT <= tMin) {
            break;
        }

        TVec3f step(dir);
        step.scale(tMin);
        hitPoint += step;
        accumT += tMin;

        cell.setUsingCast(hitPoint);

        if (!isInsideMinMaxInLocalSpace(cell)) {
            break;
        }
    } while (accumT < length);

    if (pCount != nullptr) {
        *pCount = foundCount;
    }

    return bestPrism;
}

bool KCollisionServer::KCHitSphere(KC_PrismData* pPrism, Fxyz* pCenter, f32 radius, f32 param, f32* pDist, u8* pFlag) {
    f32 radiusSq = radius * radius;
    f32 threshold = mFile->mThickness * param;
    *pFlag = 0;

    TVec3f* v0 = &mFile->mPos[pPrism->mPositionIndex];

    Fxyz dir;
    f32 distances[4];
    dir.x = pCenter->x - v0->x;
    dir.y = pCenter->y - v0->y;
    dir.z = pCenter->z - v0->z;

    TVec3f* n0 = &mFile->mNorms[pPrism->mEdgeIndices[0]];
    distances[1] = dir.x * n0->x + dir.y * n0->y + dir.z * n0->z;

    if (distances[1] >= radius) {
        return false;
    }

    TVec3f* n1 = &mFile->mNorms[pPrism->mEdgeIndices[1]];
    distances[2] = dir.x * n1->x + dir.y * n1->y + dir.z * n1->z;

    if (distances[2] >= radius) {
        return false;
    }

    TVec3f* n2 = &mFile->mNorms[pPrism->mEdgeIndices[2]];
    distances[3] = dir.x * n2->x + dir.y * n2->y + dir.z * n2->z - pPrism->mHeight;

    if (distances[3] >= radius) {
        return false;
    }

    TVec3f* faceNormal = &mFile->mNorms[pPrism->mNormalIndex];
    distances[0] = dir.x * faceNormal->x + dir.y * faceNormal->y + dir.z * faceNormal->z;
    *pDist = radius - distances[0];

    if (*pDist < 0.0f) {
        return false;
    }

    f32 nn;

    if (distances[1] > distances[2]) {
        if (!(distances[1] > distances[3])) {
            goto side2;
        }
        goto side0;
    } else {
        if (!(distances[2] > distances[3])) {
            goto side2;
        }
        goto side1;
    }

side0:
    if (distances[1] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[2] > distances[3]) {
        nn = n0->x * n1->x + n0->y * n1->y + n0->z * n1->z;

        if (nn * distances[1] > distances[2]) {
            goto vertex2;
        } else {
            goto region5;
        }
    }

    nn = n0->x * n2->x + n0->y * n2->y + n0->z * n2->z;

    if (nn * distances[1] > distances[3]) {
        goto vertex2;
    } else {
        goto region7;
    }

side1:
    if (distances[2] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[3] > distances[1]) {
        nn = n1->x * n2->x + n1->y * n2->y + n1->z * n2->z;

        if (nn * distances[2] > distances[3]) {
            goto vertex3;
        } else {
            goto region6;
        }
    }

    nn = n1->x * n0->x + n1->y * n0->y + n1->z * n0->z;

    if (nn * distances[2] > distances[1]) {
        goto vertex3;
    } else {
        goto region5;
    }

side2:

    if (distances[3] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[1] > distances[2]) {
        nn = n2->x * n0->x + n2->y * n0->y + n2->z * n0->z;

        if (nn * distances[3] > distances[1]) {
            goto vertex4;
        } else {
            goto region7;
        }
    }

    nn = n2->x * n1->x + n2->y * n1->y + n2->z * n1->z;

    if (nn * distances[3] > distances[2]) {
        goto vertex4;
    } else {
        goto region6;
    }

vertex2:
    if (distances[1] > distances[0]) {
        return false;
    }

    *pDist = radiusSq - distances[1] * distances[1];
    *pFlag = 2;
    goto finish;

vertex3:
    if (distances[2] > distances[0]) {
        return false;
    }

    *pDist = radiusSq - distances[2] * distances[2];
    *pFlag = 3;
    goto finish;

vertex4:
    if (distances[3] > distances[0]) {
        return false;
    }

    *pDist = radiusSq - distances[3] * distances[3];
    *pFlag = 4;
    goto finish;

region5 : {
    f32 t = (nn * distances[2] - distances[1]) / (nn * nn - 1.0f);
    f32 s = distances[2] - t * nn;
    dir.x = t * n0->x + s * n1->x;
    dir.y = t * n0->y + s * n1->y;
    dir.z = t * n0->z + s * n1->z;
    *pFlag = 5;
    goto edgeFinish;
}

region6 : {
    f32 t = (nn * distances[3] - distances[2]) / (nn * nn - 1.0f);
    f32 s = distances[3] - t * nn;
    dir.x = t * n1->x + s * n2->x;
    dir.y = t * n1->y + s * n2->y;
    dir.z = t * n1->z + s * n2->z;
    *pFlag = 6;
    goto edgeFinish;
}

region7 : {
    f32 t = (nn * distances[1] - distances[3]) / (nn * nn - 1.0f);
    f32 s = distances[1] - t * nn;
    dir.x = t * n2->x + s * n0->x;
    dir.y = t * n2->y + s * n0->y;
    dir.z = t * n2->z + s * n0->z;
    *pFlag = 7;
}

edgeFinish : {
    f32 closestSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    f32 dist = MR::sqrt(closestSq);

    if (dist > distances[0] || dist >= radius) {
        *pFlag = 0;
        return false;
    }

    *pDist = radiusSq - closestSq;
}

finish:
    *pDist = MR::sqrt(*pDist) - distances[0];

    if (*pDist < 0.0f || threshold < *pDist) {
        *pFlag = 0;
        return false;
    }

success:
    return true;
}

bool KCollisionServer::KCHitSphereWithThickness(KC_PrismData* pPrism, Fxyz* pCenter, f32 radius, f32 param, f32* pDist, u8* pFlag, f32 thickness) {
    f32 radiusSq = radius * radius;
    f32 threshold = thickness * param;
    *pFlag = 0;

    TVec3f* v0 = &mFile->mPos[pPrism->mPositionIndex];

    Fxyz dir;
    f32 distances[4];
    dir.x = pCenter->x - v0->x;
    dir.y = pCenter->y - v0->y;
    dir.z = pCenter->z - v0->z;

    TVec3f* n0 = &mFile->mNorms[pPrism->mEdgeIndices[0]];
    distances[1] = dir.x * n0->x + dir.y * n0->y + dir.z * n0->z;

    if (distances[1] >= radius) {
        return false;
    }

    TVec3f* n1 = &mFile->mNorms[pPrism->mEdgeIndices[1]];
    distances[2] = dir.x * n1->x + dir.y * n1->y + dir.z * n1->z;

    if (distances[2] >= radius) {
        return false;
    }

    TVec3f* n2 = &mFile->mNorms[pPrism->mEdgeIndices[2]];
    distances[3] = dir.x * n2->x + dir.y * n2->y + dir.z * n2->z - pPrism->mHeight;

    if (distances[3] >= radius) {
        return false;
    }

    TVec3f* faceNormal = &mFile->mNorms[pPrism->mNormalIndex];
    distances[0] = dir.x * faceNormal->x + dir.y * faceNormal->y + dir.z * faceNormal->z;
    *pDist = radius - distances[0];

    if (*pDist < 0.0f) {
        return false;
    }

    f32 nn;

    if (distances[1] > distances[2]) {
        if (!(distances[1] > distances[3])) {
            goto side2;
        }
        goto side0;
    } else {
        if (!(distances[2] > distances[3])) {
            goto side2;
        }
        goto side1;
    }

side0:
    if (distances[1] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[2] > distances[3]) {
        nn = n0->x * n1->x + n0->y * n1->y + n0->z * n1->z;

        if (!(nn * distances[1] > distances[2])) {
            goto region5;
        }

        goto vertex2;
    }

    nn = n0->x * n2->x + n0->y * n2->y + n0->z * n2->z;

    if (!(nn * distances[1] > distances[3])) {
        goto region7;
    }

    goto vertex2;

side1:
    if (distances[2] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[3] > distances[1]) {
        nn = n1->x * n2->x + n1->y * n2->y + n1->z * n2->z;

        if (!(nn * distances[2] > distances[3])) {
            goto region6;
        }

        goto vertex3;
    }

    nn = n1->x * n0->x + n1->y * n0->y + n1->z * n0->z;

    if (!(nn * distances[2] > distances[1])) {
        goto region5;
    }

    goto vertex3;

side2:

    if (distances[3] <= 0.0f) {
        if (threshold < *pDist) {
            return false;
        }

        *pFlag = 1;
        goto success;
    }

    if (distances[1] > distances[2]) {
        nn = n2->x * n0->x + n2->y * n0->y + n2->z * n0->z;

        if (!(nn * distances[3] > distances[1])) {
            goto region7;
        }

        goto vertex4;
    }

    nn = n2->x * n1->x + n2->y * n1->y + n2->z * n1->z;

    if (!(nn * distances[3] > distances[2])) {
        goto region6;
    }

    goto vertex4;

vertex2:
    *pDist = radiusSq - distances[1] * distances[1];
    *pFlag = 2;
    goto finish;

vertex3:
    *pDist = radiusSq - distances[2] * distances[2];
    *pFlag = 3;
    goto finish;

vertex4:
    *pDist = radiusSq - distances[3] * distances[3];
    *pFlag = 4;
    goto finish;

region5 : {
    f32 t = (nn * distances[2] - distances[1]) / (nn * nn - 1.0f);
    f32 s = distances[2] - t * nn;
    dir.x = t * n0->x + s * n1->x;
    dir.y = t * n0->y + s * n1->y;
    dir.z = t * n0->z + s * n1->z;
    *pFlag = 5;
    goto edgeFinish;
}

region6 : {
    f32 t = (nn * distances[3] - distances[2]) / (nn * nn - 1.0f);
    f32 s = distances[3] - t * nn;
    dir.x = t * n1->x + s * n2->x;
    dir.y = t * n1->y + s * n2->y;
    dir.z = t * n1->z + s * n2->z;
    *pFlag = 6;
    goto edgeFinish;
}

region7 : {
    f32 t = (nn * distances[1] - distances[3]) / (nn * nn - 1.0f);
    f32 s = distances[1] - t * nn;
    dir.x = t * n2->x + s * n0->x;
    dir.y = t * n2->y + s * n0->y;
    dir.z = t * n2->z + s * n0->z;
    *pFlag = 7;
}

edgeFinish : {
    f32 closestSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    f32 dist = MR::sqrt(closestSq);

    if (dist >= radius) {
        *pFlag = 0;
        return false;
    }

    *pDist = radiusSq - closestSq;
}

finish:
    f32 distance = MR::sqrt(*pDist);

    if (distances[0] + distance < 0.0f) {
        *pFlag = 0;
        return false;
    }

    *pDist = distance - distances[0];

    if (*pDist < 0.0f || threshold < *pDist) {
        *pFlag = 0;
        return false;
    }

success:
    return true;
}

bool KCollisionServer::isNearParallelNormal(const KC_PrismData* pPrism) const {
    TVec3f edge0 = mFile->mNorms[pPrism->mEdgeIndices[0]];
    TVec3f edge1 = mFile->mNorms[pPrism->mEdgeIndices[1]];
    TVec3f edge2 = mFile->mNorms[pPrism->mEdgeIndices[2]];

    bool isNear = false;

    if (MR::isSameDirection(edge0, edge1) || MR::isSameDirection(edge0, edge2) || MR::isSameDirection(edge1, edge2)) {
        isNear = true;
    }

    return isNear;
}

bool KCollisionServer::KCHitArrow(KC_PrismData* pPrism, const TVec3f& rOrigin, const TVec3f& rDir, f32* pDist, u8* pFlag) const {
    TVec3f* v0 = &mFile->mPos[pPrism->mPositionIndex];
    TVec3f* faceNormal = &mFile->mNorms[pPrism->mNormalIndex];

    TVec3f rel;
    PSVECSubtract(rOrigin, v0, rel);

    f32 t = PSVECDotProduct(rel, faceNormal);

    if (t <= 0.0f) {
        *pFlag = 0;
        return false;
    }

    f32 dirDotFace = PSVECDotProduct(faceNormal, rDir);

    if (0.0f < t + dirDotFace) {
        *pFlag = 0;
        return false;
    }

    t = t / -dirDotFace;

    TVec3f hit = rDir * t;
    hit += rel;

    bool onEdge0 = false;
    bool onEdge1 = false;
    bool onEdge2 = false;

    f32 e0 = PSVECDotProduct(hit, mFile->mNorms[pPrism->mEdgeIndices[0]]);

    if (0.01f < e0) {
        *pFlag = 0;
        return false;
    }

    if (0.0f <= e0 && e0 <= 0.01f) {
        onEdge0 = true;
    }

    f32 e1 = PSVECDotProduct(hit, mFile->mNorms[pPrism->mEdgeIndices[1]]);

    if (0.01f < e1) {
        *pFlag = 0;
        return false;
    }

    if (0.0f <= e1 && e1 <= 0.01f) {
        onEdge1 = true;
    }

    f32 e2 = PSVECDotProduct(hit, mFile->mNorms[pPrism->mEdgeIndices[2]]);

    if (0.01f + pPrism->mHeight < e2) {
        *pFlag = 0;
        return false;
    }

    if (0.0f <= e2 && e2 <= 0.01f) {
        onEdge2 = true;
    }

    *pDist = t;

    if (onEdge0) {
        if (onEdge1) {
            if (onEdge2) {
                *pFlag = 1;
            } else {
                *pFlag = 5;
            }
        } else {
            if (onEdge2) {
                *pFlag = 7;
            } else {
                *pFlag = 2;
            }
        }
    } else {
        if (onEdge1) {
            if (onEdge2) {
                *pFlag = 6;
            } else {
                *pFlag = 3;
            }
        } else {
            if (onEdge2) {
                *pFlag = 4;
            } else {
                *pFlag = 1;
            }
        }
    }

    return true;
}

s32 KCollisionServer::toIndex(const KC_PrismData* pPrism) const {
    return pPrism - (mFile->mPrisms + 1);
}

TVec3f* KCollisionServer::getFaceNormal(const KC_PrismData* pPrism) const {
    return &mFile->mNorms[pPrism->mNormalIndex];
}

TVec3f* KCollisionServer::getEdgeNormal1(const KC_PrismData* pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[0]];
}

TVec3f* KCollisionServer::getEdgeNormal2(const KC_PrismData* pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[1]];
}

TVec3f* KCollisionServer::getEdgeNormal3(const KC_PrismData* pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[2]];
}

TVec3f* KCollisionServer::getNormal(u32 index) const {
    return &mFile->mNorms[index];
}

void KCollisionServer::calXvec(const Fxyz* pVecA, const Fxyz* pVecB, Fxyz* pDst) const {
    pDst->x = pVecA->z * pVecB->y - pVecA->y * pVecB->z;
    pDst->y = pVecA->x * pVecB->z - pVecA->z * pVecB->x;
    pDst->z = pVecA->y * pVecB->x - pVecA->x * pVecB->y;
}

TVec3f KCollisionServer::getPos(const KC_PrismData* pPrism, int vertexIndex) const {
    switch (vertexIndex) {
    case 0: {
        TVec3f* pos = &mFile->mPos[pPrism->mPositionIndex];

        return TVec3f(pos->x, pos->y, pos->z);
    }
    case 1: {
        Fxyz* pos = reinterpret_cast< Fxyz* >(&mFile->mPos[pPrism->mPositionIndex]);
        Fxyz* edge2 = reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mEdgeIndices[2]]);

        Fxyz finalPos;

        calXvec(reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mEdgeIndices[1]]), reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mNormalIndex]),
                &finalPos);

        f32 sideLength = pPrism->mHeight / (finalPos.x * edge2->x + finalPos.y * edge2->y + finalPos.z * edge2->z);

        finalPos.x = pos->x + sideLength * finalPos.x;
        finalPos.y = pos->y + sideLength * finalPos.y;
        finalPos.z = pos->z + sideLength * finalPos.z;

        return TVec3f(finalPos.x, finalPos.y, finalPos.z);
    }
    case 2: {
        Fxyz* pos = reinterpret_cast< Fxyz* >(&mFile->mPos[pPrism->mPositionIndex]);
        Fxyz* edge2 = reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mEdgeIndices[2]]);

        Fxyz finalPos;

        calXvec(reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mNormalIndex]), reinterpret_cast< Fxyz* >(&mFile->mNorms[pPrism->mEdgeIndices[0]]),
                &finalPos);

        f32 sideLength = pPrism->mHeight / (finalPos.x * edge2->x + finalPos.y * edge2->y + finalPos.z * edge2->z);

        finalPos.x = pos->x + sideLength * finalPos.x;
        finalPos.y = pos->y + sideLength * finalPos.y;
        finalPos.z = pos->z + sideLength * finalPos.z;

        return TVec3f(finalPos.x, finalPos.y, finalPos.z);
    }
    default:
        return TVec3f(0.0f, 0.0f, 0.0f);
    }
}

KC_PrismData* KCollisionServer::getPrismData(u32 index) const {
    return &mFile->mPrisms[1 + index];
}

s32 KCollisionServer::getTriangleNum() const {
    return (reinterpret_cast< u8* >(mFile->mOctree) - reinterpret_cast< u8* >(mFile->mPrisms + 1)) / sizeof(KC_PrismData);
}

JMapInfoIter KCollisionServer::getAttributes(u32 index) const {
    KC_PrismData* prism = &mFile->mPrisms[1 + index];

    return JMapInfoIter(mapInfo, prism->mAttribute);
}

s32* KCollisionServer::searchBlock(s32* pShift, const u32& rX, const u32& rY, const u32& rZ) const {
    KCLFile* file = mFile;
    s32 blockWidthShift = file->mBlockWidthShift;
    u8* octree = reinterpret_cast< u8* >(file->mOctree);
    *pShift = blockWidthShift;

    s32 offset = (((rZ >> blockWidthShift) << file->mBlockXYShift) | ((rY >> blockWidthShift) << file->mBlockXShift) | (rX >> blockWidthShift)) * 4;

    if (file->mBlockXYShift == -1 && file->mBlockXShift == -1) {
        offset = 0;
    }

    while ((offset = *reinterpret_cast< s32* >(octree + offset)) >= 0) {
        octree += offset;
        (*pShift)--;
        s32 shift = *pShift;

        offset = ((((rZ >> shift) & 1) << 2) | (((rY >> shift) & 1) << 1) | ((rX >> shift) & 1)) * 4;
    }

    return reinterpret_cast< s32* >(octree + (offset & 0x7FFFFFFF));
}

void KCollisionServer::V3u::setUsingCast(const TVec3f& rPos) {
    x = static_cast< s32 >(rPos.x);
    y = static_cast< s32 >(rPos.y);
    z = static_cast< s32 >(rPos.z);
}

Fxyz& Fxyz::operator=(const Fxyz& rOther) {
    x = rOther.x;
    y = rOther.y;
    z = rOther.z;

    return *this;
}
