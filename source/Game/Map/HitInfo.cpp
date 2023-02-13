#include "Game/Map/CollisionParts.h"
#include "Game/Map/HitInfo.h"
#include "Game/Map/KCollision.h"
#include "Game/Util/MathUtil.h"

Triangle::Triangle() {
    mParts = nullptr;
    mIdx = 0xFFFFFFFF;
    mSensor = nullptr;
    mNormals[0].zero();
    mNormals[1].zero();
    mNormals[2].zero();
    mNormals[3].zero();
    mPos[0].zero();
    mPos[1].zero();
    mPos[2].zero();
}

void Triangle::fillData(CollisionParts *pParts, unsigned long index, HitSensor *pSensor) {
    mParts = pParts;
    mIdx = index;
    mSensor = pSensor;

    KCollisionServer *server = pParts->mServer;
    MtxPtr matrix;
    KC_PrismData *prism = server->getPrismData(index);

    mNormals[0].set(*server->getFaceNormal(prism));
    mNormals[1].set(*server->getEdgeNormal1(prism));
    mNormals[2].set(*server->getEdgeNormal2(prism));
    mNormals[3].set(*server->getEdgeNormal3(prism));

    matrix = reinterpret_cast<MtxPtr>(&mParts->mBaseMatrix);

    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[0]), reinterpret_cast<Vec *>(&mNormals[0]));
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[1]), reinterpret_cast<Vec *>(&mNormals[1]));
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[2]), reinterpret_cast<Vec *>(&mNormals[2]));
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[3]), reinterpret_cast<Vec *>(&mNormals[3]));

    MR::normalize(&mNormals[0]);
    MR::normalize(&mNormals[1]);
    MR::normalize(&mNormals[2]);
    MR::normalize(&mNormals[3]);

    mPos[0].set(server->getPos(prism, 0));
    mPos[1].set(server->getPos(prism, 1));
    mPos[2].set(server->getPos(prism, 2));

    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mPos[0]), reinterpret_cast<Vec *>(&mPos[0]));
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mPos[1]), reinterpret_cast<Vec *>(&mPos[1]));
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mPos[2]), reinterpret_cast<Vec *>(&mPos[2]));
}

const char *Triangle::getHostName() const {
    return mParts->getHostName();
}

s32 Triangle::getHostPlacementZoneID() const {
    return mParts->getPlacementZoneID();
}

bool Triangle::isHostMoved() const {
    return mParts->_D4 == 0;
}

bool Triangle::isValid() const {
    return mIdx != 0xFFFFFFFF;
}

const TVec3f *Triangle::getNormal(int index) const {
    return &mNormals[index];
}

const TVec3f *Triangle::getFaceNormal() const {
    return &mNormals[0];
}

const TVec3f *Triangle::getEdgeNormal(int index) const {
    return &mNormals[index + 1];
}

const TVec3f *Triangle::getPos(int index) const {
    return &mPos[index];
}

const TVec3f *Triangle::calcAndGetNormal(int index) {
    KCollisionServer *server = mParts->mServer;
    KC_PrismData *prism = server->getPrismData(mIdx);

    MtxPtr matrix = reinterpret_cast<MtxPtr>(&mParts->mBaseMatrix);

    switch (index) {
        case 0: {
                mNormals[0].set(*server->getFaceNormal(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[0]), reinterpret_cast<Vec *>(&mNormals[0]));
                MR::normalize(&mNormals[0]);

                return &mNormals[0];
            }
            break;
        case 1: {
                mNormals[1].set(*server->getEdgeNormal1(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[1]), reinterpret_cast<Vec *>(&mNormals[1]));
                MR::normalize(&mNormals[1]);

                return &mNormals[1];
            }
            break;
        case 2: {
                mNormals[2].set(*server->getEdgeNormal2(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[2]), reinterpret_cast<Vec *>(&mNormals[2]));
                MR::normalize(&mNormals[2]);
                
                return &mNormals[2];
            }
            break;
        case 3: {
                mNormals[3].set(*server->getEdgeNormal3(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[3]), reinterpret_cast<Vec *>(&mNormals[3]));
                MR::normalize(&mNormals[3]);

                return &mNormals[3];
            }
            break;
    }

    return &mNormals[index];
}

const TVec3f *Triangle::calcAndGetEdgeNormal(int index) {
    KCollisionServer *server = mParts->mServer;
    KC_PrismData *prism = server->getPrismData(mIdx);

    MtxPtr matrix = reinterpret_cast<MtxPtr>(&mParts->mBaseMatrix);

    switch (index) {
        case 0: {
                mNormals[1].set(*server->getEdgeNormal1(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[1]), reinterpret_cast<Vec *>(&mNormals[1]));
                MR::normalize(&mNormals[1]);

                return &mNormals[1];
            }
            break;
        case 1: {
                mNormals[2].set(*server->getEdgeNormal2(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[2]), reinterpret_cast<Vec *>(&mNormals[2]));
                MR::normalize(&mNormals[2]);

                return &mNormals[2];
            }
            break;
        case 2: {
                mNormals[3].set(*server->getEdgeNormal3(prism));
                PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&mNormals[3]), reinterpret_cast<Vec *>(&mNormals[3]));
                MR::normalize(&mNormals[3]);

                return &mNormals[3];
            }
            break;
    }

    return &mNormals[index + 1];
}

const TVec3f *Triangle::calcAndGetPos(int index) {
    KCollisionServer *server = mParts->mServer;
    KC_PrismData *prism = server->getPrismData(mIdx);

    TVec3f *pos = &mPos[index];

    pos->set(server->getPos(prism, index));

    mParts->mBaseMatrix.mult(*pos, *pos);

    return pos;
}

void Triangle::calcForceMovePower(TVec3f *a1, const TVec3f &a2) const {
    mParts->calcForceMovePower(a1, a2);
}

JMapInfoIter Triangle::getAttributes() const {
    if (mIdx == 0xFFFFFFFF) {
        JMapInfoIter iter;
        iter.mInfo = nullptr;
        iter._4 = -1;

        return iter;
    }

    return mParts->mServer->getAttributes(mIdx);
}

TPos3f *Triangle::getBaseMtx() const {
    return &mParts->mBaseMatrix;
}

TPos3f *Triangle::getBaseInvMtx() const {
    return &mParts->mInvBaseMatrix;
}

TPos3f *Triangle::getPrevBaseMtx() const {
    return &mParts->mPrevBaseMatrix;
}

HitInfo::HitInfo() : mParentTriangle(), _60(0.0f),
    _64(0, 0, 0), _70(0, 0, 0), _7C(0, 0, 0) {
    _88 = 0;
}

bool HitInfo::isCollisionAtFace() const {
    return _88 == 1;
}

bool HitInfo::isCollisionAtEdge() const {
    return _88 == 2 || _88 == 3 || _88 == 4;
}

bool HitInfo::isCollisionAtCorner() const {
    return _88 == 5 || _88 == 6 || _88 == 7;
}
