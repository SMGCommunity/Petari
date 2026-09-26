#include "Game/Map/KCollision.hpp"

bool KCollisionServer::isInsideMinMaxInLocalSpace(const V3u& rPoint) const {
    return (rPoint.x & mFile->mXMask) == 0 && (rPoint.y & mFile->mYMask) == 0 && (rPoint.z & mFile->mZMask) == 0;
}

bool KCollisionServer::outCheck(const TVec3f* pPosA, const TVec3f* pPosB, V3u* pPointA, V3u* pPointB) const {
    objectSpaceToLocalSpace(pPointA, *pPosA);
    objectSpaceToLocalSpace(pPointB, *pPosB);

    if (static_cast< s32 >(pPointA->x) < 0) {
        pPointA->x = 0;
    }

    if (static_cast< s32 >(pPointA->y) < 0) {
        pPointA->y = 0;
    }

    if (static_cast< s32 >(pPointA->z) < 0) {
        pPointA->z = 0;
    }

    s32 invertedXMask = ~mFile->mXMask;

    if (invertedXMask < static_cast< s32 >(pPointB->x)) {
        pPointB->x = invertedXMask;
    }

    s32 invertedYMask = ~mFile->mYMask;

    if (invertedYMask < static_cast< s32 >(pPointB->y)) {
        pPointB->y = invertedYMask;
    }

    s32 invertedZMask = ~mFile->mZMask;

    if (invertedZMask < static_cast< s32 >(pPointB->z)) {
        pPointB->z = invertedZMask;
    }

    if (static_cast< s32 >(pPointB->x) < static_cast< s32 >(pPointA->x) || static_cast< s32 >(pPointB->y) < static_cast< s32 >(pPointA->y) ||
        static_cast< s32 >(pPointB->z) < static_cast< s32 >(pPointA->z)) {
        return false;
    }

    return true;
}

void KCollisionServer::objectSpaceToLocalSpace(V3u* pPoint, const TVec3f& rPos) const {
    pPoint->x = static_cast< s32 >(rPos.x - mFile->mMin.x);
    pPoint->y = static_cast< s32 >(rPos.y - mFile->mMin.y);
    pPoint->z = static_cast< s32 >(rPos.z - mFile->mMin.z);
}
