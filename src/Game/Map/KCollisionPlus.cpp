#include "Game/Map/KCollision.hpp"

/*bool KCollisionServer::isInsideMinMaxInLocalSpace(const V3u &point) const {
    bool inside = false;
    bool bVar1 = false;

    if ((point.x & mFile->mXMask) == 0 && (point.y & mFile->mYMask) == 0) {
        bVar1 = true;
    }

    if (!bVar1) {
        return inside;
    }

    if ((point.z & mFile->mZMask) != 0) {
        return inside;
    }

    return true;
}*/

bool KCollisionServer::outCheck(const TVec3f *pPosA, const TVec3f *pPosB, V3u *pPointA, V3u *pPointB) const {
    objectSpaceToLocalSpace(pPointA, *pPosA);
    objectSpaceToLocalSpace(pPointB, *pPosB);

    if (pPointA->x < 0) {
        pPointA->x = 0;
    }

    if (pPointA->y < 0) {
        pPointA->y = 0;
    }

    if (pPointA->z < 0) {
        pPointA->z = 0;
    }

    s32 invertedXMask = ~mFile->mXMask;

    if (invertedXMask < pPointB->x) {
        pPointB->x = invertedXMask;
    }

    s32 invertedYMask = ~mFile->mYMask;

    if (invertedYMask < pPointB->y) {
        pPointB->y = invertedYMask;
    }

    s32 invertedZMask = ~mFile->mZMask;

    if (invertedZMask < pPointB->z) {
        pPointB->z = invertedZMask;
    }

    if (pPointB->x < pPointA->x || pPointB->y < pPointA->y || pPointB->z < pPointA->z) {
        return false;
    }

    return true;
}

void KCollisionServer::objectSpaceToLocalSpace(V3u *pPoint, const TVec3f &rPos) const {
    pPoint->x = static_cast<s32>(rPos.x - mFile->mMin.x);
    pPoint->y = static_cast<s32>(rPos.y - mFile->mMin.y);
    pPoint->z = static_cast<s32>(rPos.z - mFile->mMin.z);
}
