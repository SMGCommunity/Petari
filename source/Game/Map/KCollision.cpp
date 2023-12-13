#include "Game/Map/KCollision.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MathUtil.hpp"

Fxyz &Fxyz::operator=(const Fxyz &rOther) {
    x = rOther.x;
    y = rOther.y;
    z = rOther.z;

    return *this;
}

void KCollisionServer::V3u::setUsingCast(const TVec3f &rPos) {
    x = static_cast<s32>(rPos.x);
    y = static_cast<s32>(rPos.y);
    z = static_cast<s32>(rPos.z);
}

KCollisionServer::KCollisionServer() {
    mFile = nullptr;
    mapInfo = new JMapInfo();
    mMaxVertexDistance = 1.0f;
}

void KCollisionServer::init(void *pData, const void *pMapData) {
    setData(pData);

    if (pMapData != nullptr) {
        mapInfo->attach(pMapData);
    }
}

void KCollisionServer::setData(void *pData) {
    mFile = reinterpret_cast<KCLFile *>(pData);

    if (isBinaryInitialized(pData)) {
        mFile->mPos = reinterpret_cast<TVec3f *>(reinterpret_cast<u8 *>(mFile) + mFile->mPosOffset);
        mFile->mNorms = reinterpret_cast<TVec3f *>(reinterpret_cast<u8 *>(mFile) + mFile->mNormOffset);
        mFile->mPrisms = reinterpret_cast<KC_PrismData *>(reinterpret_cast<u8 *>(mFile) + mFile->mPrismOffset);
        mFile->mOctree = reinterpret_cast<void *>(reinterpret_cast<u8 *>(mFile) + mFile->mOctreeOffset);
    }
}

/*void KCollisionServer::calcFarthestVertexDistance() {
    s32 triCount = getTriangleNum();
    f32 maxDistance = 0.0f;

    for (s32 i = 0; i < triCount; i++) {

    }
}*/

bool KCollisionServer::isBinaryInitialized(const void *pData) {
    return reinterpret_cast<const s32 *>(pData)[0] < 0;
}

bool KCollisionServer::isNearParallelNormal(const KC_PrismData *pPrism) const {
    TVec3f edge0 = mFile->mNorms[pPrism->mEdgeIndices[0]];
    TVec3f edge1 = mFile->mNorms[pPrism->mEdgeIndices[1]];
    TVec3f edge2 = mFile->mNorms[pPrism->mEdgeIndices[2]];

    bool isNear = false;

    if (MR::isSameDirection(edge0, edge1, 0.01f) || MR::isSameDirection(edge0, edge2, 0.01f) || MR::isSameDirection(edge1, edge2, 0.01f)) {
        isNear = true;
    }

    return isNear;
}

s32 KCollisionServer::toIndex(const KC_PrismData *pPrism) const {
    return pPrism - (mFile->mPrisms + 1);
}

TVec3f *KCollisionServer::getFaceNormal(const KC_PrismData *pPrism) const {
    return &mFile->mNorms[pPrism->mNormalIndex];
}

TVec3f *KCollisionServer::getEdgeNormal1(const KC_PrismData *pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[0]];
}

TVec3f *KCollisionServer::getEdgeNormal2(const KC_PrismData *pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[1]];
}

TVec3f *KCollisionServer::getEdgeNormal3(const KC_PrismData *pPrism) const {
    return &mFile->mNorms[pPrism->mEdgeIndices[2]];
}

TVec3f *KCollisionServer::getNormal(unsigned long index) const {
    return &mFile->mNorms[index];
}

void KCollisionServer::calXvec(const Fxyz *pVecA, const Fxyz *pVecB, Fxyz *pDst) const {
    pDst->x = pVecA->z * pVecB->y - pVecA->y * pVecB->z;
    pDst->y = pVecA->x * pVecB->z - pVecA->z * pVecB->x;
    pDst->z = pVecA->y * pVecB->x - pVecA->x * pVecB->y;
}

TVec3f KCollisionServer::getPos(const KC_PrismData *pPrism, int vertexIndex) const {
    switch (vertexIndex) {
        case 0: {
                TVec3f *pos = &mFile->mPos[pPrism->mPositionIndex];

                return TVec3f(pos->x, pos->y, pos->z);
            }
        case 1: {
                Fxyz *pos = reinterpret_cast<Fxyz *>(&mFile->mPos[pPrism->mPositionIndex]);
                Fxyz *edge2 = reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mEdgeIndices[2]]);

                Fxyz finalPos;

                calXvec(
                    reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mEdgeIndices[1]]),
                    reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mNormalIndex]),
                    &finalPos
                    );

                f32 sideLength = pPrism->mHeight / (finalPos.x * edge2->x + finalPos.y * edge2->y + finalPos.z * edge2->z);

                finalPos.x = pos->x + sideLength * finalPos.x;
                finalPos.y = pos->y + sideLength * finalPos.y;
                finalPos.z = pos->z + sideLength * finalPos.z;

                return TVec3f(finalPos.x, finalPos.y, finalPos.z);
            }
        case 2: {
                Fxyz *pos = reinterpret_cast<Fxyz *>(&mFile->mPos[pPrism->mPositionIndex]);
                Fxyz *edge2 = reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mEdgeIndices[2]]);

                Fxyz finalPos;

                calXvec(
                    reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mNormalIndex]),
                    reinterpret_cast<Fxyz *>(&mFile->mNorms[pPrism->mEdgeIndices[0]]),
                    &finalPos
                );

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

KC_PrismData *KCollisionServer::getPrismData(unsigned long index) const {
    return &mFile->mPrisms[1 + index];
}

s32 KCollisionServer::getTriangleNum() const {
    return (reinterpret_cast<u8 *>(mFile->mOctree) - reinterpret_cast<u8 *>(mFile->mPrisms + 1)) / sizeof(KC_PrismData);
}

JMapInfoIter KCollisionServer::getAttributes(unsigned long index) const {
    KC_PrismData *prism = &mFile->mPrisms[1 + index];

    JMapInfoIter iter;
    iter.mInfo = mapInfo;
    iter._4 = prism->mAttribute;

    return iter;
}

#ifdef NON_MATCHING
// Register mismatch
s32 *KCollisionServer::searchBlock(long *a1, const unsigned long &rX, const unsigned long &rY, const unsigned long &rZ) const {
    KCLFile *file = mFile;
    s32 blockWidthShift = file->mBlockWidthShift;
    u8 *octree = reinterpret_cast<u8 *>(file->mOctree);
    *a1 = blockWidthShift;

    s32 offset = ((rX >> blockWidthShift) | ((rZ >> blockWidthShift) << file->mBlockXYShift) | ((rY >> blockWidthShift) << file->mBlockXShift)) * 4;

    if (file->mBlockXYShift == -1 && file->mBlockXShift == -1) {
        offset = 0;
    }

    while ((offset = *reinterpret_cast<s32 *>(octree + offset)) >= 0) {
        octree += offset;
        s32 uVar7 = --(*a1);

        offset = ((((rZ >> uVar7) & 1) << 2) | (((rY >> uVar7) & 1) << 1) | ((rX >> uVar7) & 1)) * 4;
    }

    return reinterpret_cast<s32 *>(octree + (offset & 0x7FFFFFFF));
}
#endif
