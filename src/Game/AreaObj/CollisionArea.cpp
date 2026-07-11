#include "Game/AreaObj/CollisionArea.hpp"
#include "Game/MapObj/DynamicCollisionObj.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

DynamicCollisionObj::~DynamicCollisionObj() {
}

AreaPolygon::AreaPolygon() : DynamicCollisionObj("エリアポリゴン") {
    mForm = nullptr;
    _128 = nullptr;
    _12C.zero();
}

AreaPolygon::~AreaPolygon() {
}

//this is needed to get setLength to match
void FORCE_SCALE() {
    TVec3f vec(0.0f);
    vec.scale(5.0f);
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
    mKCLFile = nullptr;
    mPositionNum = 4;
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

void AreaPolygon::setMtx(MtxPtr matrix, const TVec3f& a2, f32 a3) {
    _128 = matrix;
    _12C = a2;
    _138 = a3;
}

void AreaPolygon::setSurfaceAndSync(s32 a1) {
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

CollisionArea::CollisionArea(int formType, const char* pName) : AreaObj(formType, pName) {
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

void CollisionArea::init(const JMapInfoIter& rIter) {
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
    } else {
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
    } else {
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
        } else {
            TVec3f playerPos = *MR::getPlayerCenterPos();
            f32 dVar4 = static_cast< f32 >(_5C);

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
            } else if (!_58 && _50 == -1) {
                TVec3f tStack84;

                if (_3C == 0) {
                    tStack84 = tStack72;
                    tStack84.setLength(dVar4 + _40);
                } else {
                    tStack84 = playerPos - tStack60;
                    tStack84.setLength(dVar4);

                    tStack84 = tStack60 + tStack84 - playerPos;
                }

                if (tStack84.dot(tStack72) > 0) {
                    MR::pushPlayerFromArea(tStack84);
                }
            }
        }
    }
}
