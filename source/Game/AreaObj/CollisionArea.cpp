#include "Game/AreaObj/CollisionArea.h"
#include "Game/MapObj/DynamicCollisionObj.h"
#include "Game/Util/JMapUtil.h"
#include "Game/Util/LiveActorUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Inline.h"

DynamicCollisionObj::~DynamicCollisionObj() {

}

bool AreaObj::isValid() const {
    bool valid = false;

    if (mValid && _15 && mAwake) {
        valid = true;
    }

    return valid;
}

AreaPolygon::AreaPolygon() : DynamicCollisionObj("エリアポリゴン") {
    mForm = nullptr;
    _128 = nullptr;
    _12C.zero();
}

AreaPolygon::~AreaPolygon() {

}

/*void AreaPolygon::init(const JMapInfoIter &rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
    }
    else {
        MR::extractMtxTrans(_128, &mPosition);
        mRotation.x = 0.0f;
        mRotation.y = 0.0f;
        mRotation.z = 0.0f;
    }

    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;
    _8C = nullptr;
    mNrTriangles = 4;
    _94 = 2;

    mPositions = new TVec3f[12];
    setSurface(0);
    initHitSensor(1);

    TVec3f local28 = CALL_INLINE_FUNC(TVec3f, 1.0f, 1.0f, 1.0f);
    MR::addHitSensorEye(this, "body", 8, 1.0f, local28);

    mIndices = new TriangleIndexing[_94];
    _9C = new TVec3f[_94];

    mIndices[0].mIndex[0] = 0;
    mIndices[0].mIndex[1] = 0;
    mIndices[0].mIndex[2] = 0;

    mIndices[1].mIndex[0] = 0;
    mIndices[1].mIndex[2] = 0;
    mIndices[1].mIndex[3] = 0;

    createCollision();
    makeActorAppeared();
}*/

void AreaPolygon::setMtx(MtxPtr matrix, const TVec3f &a2, float a3) {
    _128 = matrix;
    _12C = a2;
    _138 = a3;
}

void AreaPolygon::setSurfaceAndSync(long a1) {
    setSurface(a1);

    if (a1 != -1) {
        syncCollision();
    }
}

void AreaPolygon::invalidate() {
    MR::invalidateCollisionParts(mParts);
}

void AreaPolygon::validate() {
    MR::validateCollisionParts(mParts);
}

CollisionArea::CollisionArea(int type, const char *pName) : AreaObj(type, pName) {
    _3C = 0;
    _40 = 0.0f;
    _44.zero();
    _50 = 0;
    _54 = 0;
    _58 = false;
    _5C = 0;
    _60 = 0;
    mPolygon = nullptr;
    mIsValid = false;
}

CollisionArea::~CollisionArea() {

}

void CollisionArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);

    _50 = -1;
    _3C = 0;
    _40 = 0.0f;
    _54 = -1;
    _5C = 0;
    _60 = -1;

    MR::getJMapInfoArg0NoInit(rIter, &_50);
    MR::getJMapInfoArg1NoInit(rIter, &_54);
    MR::getJMapInfoArg2NoInit(rIter, &_5C);
    MR::getJMapInfoArg3NoInit(rIter, &_60);

    _58 = false;

    if (_60 == 0) {
        mPolygon = nullptr;
    }
    else {
        mPolygon = new AreaPolygon();
        mPolygon->mForm = mForm;
        mPolygon->init(rIter);
        MR::validateCollisionParts(mPolygon->mParts);

        if (!isValid()) {
            MR::invalidateCollisionParts(mPolygon->mParts);
        }
    }

    mIsValid = isValid();
}

void CollisionArea::movement() {
    if (!isValid()) {
        if (mIsValid) {
            if (mPolygon != nullptr) {
                MR::invalidateCollisionParts(mPolygon->mParts);
            }

            mIsValid = false;
        }
    }
    else {
        if (!mIsValid) {
            if (mPolygon != nullptr) {
                MR::validateCollisionParts(mPolygon->mParts);
            }

            mIsValid = true;
        }

        if (_54 == -1 && MR::isPlayerTeresaDisappear()) {
            _58 = true;

            if (mPolygon != nullptr) {
                MR::invalidateCollisionParts(mPolygon->mParts);
            }
        }
        else {
            TVec3f playerPos = *MR::getPlayerCenterPos();
            f32 dVar4 = static_cast<f32>(_5C);

            if (_5C == 0) {
                dVar4 = MR::getPlayerHitRadius();
            }

            if (_58) {
                dVar4 += 5.0f;
            }

            TVec3f tStack60;
            TVec3f tStack72;

            if (!hitCheck(playerPos, dVar4, &tStack60, &tStack72)) {
                if (_58) {
                    _58 = false;

                    if (mPolygon != nullptr) {
                        MR::validateCollisionParts(mPolygon->mParts);
                    }
                }
            }
            else if (!_58 && _50 == -1) {
                TVec3f tStack84;

                if (_3C == 0) {
                    tStack84 = tStack72;
                    tStack84.setLength(dVar4 + _40);
                }
                else {
                    TVec3f tStack96;
                    TVec3f tStack108 = playerPos;
                    tStack108.sub(tStack60);

                    tStack84 = tStack108;
                    tStack84.setLength(dVar4);

                    tStack96 = tStack60;
                    tStack96.add(tStack84);

                    TVec3f tStack120 = tStack96;
                    tStack120.sub(playerPos);
                    tStack84 = tStack120;
                }

                if (tStack84.dot(tStack72) > 0) {
                    MR::pushPlayerFromArea(tStack84);
                }
            }
        }
    }
}