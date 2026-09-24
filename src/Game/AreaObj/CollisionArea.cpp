#include "Game/AreaObj/CollisionArea.hpp"
#include "Game/AreaObj/AreaForm.hpp"
#include "Game/MapObj/DynamicCollisionObj.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

void CollisionArea_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
    (void)0.5f;
    (void)3.0f;
    (void)5.0f;
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
            f32 radius = static_cast< f32 >(_5C);

            if (_5C == 0) {
                radius = MR::getPlayerHitRadius();
            }

            if (_58) {
                radius += 5.0f;
            }

            TVec3f hitPos;
            TVec3f hitNormal;

            if (!hitCheck(playerPos, radius, &hitPos, &hitNormal)) {
                if (_58) {
                    _58 = false;

                    if (mPolygon != nullptr) {
                        MR::validateCollisionParts(mPolygon->mParts);
                    }
                }
            } else if (!_58 && _50 == -1) {
                TVec3f push;

                if (_3C == 0) {
                    push = hitNormal;
                    push.setLength(radius + _40);
                } else {
                    push = playerPos - hitPos;
                    push.setLength(radius);

                    TVec3f contact(hitPos + push);
                    push = contact - playerPos;
                }

                if (push.dot(hitNormal) > 0) {
                    MR::pushPlayerFromArea(push);
                }
            }
        }
    }
}

bool CollisionArea::hitCheck(const TVec3f& rPos, f32 radius, TVec3f* pPoint, TVec3f* pNormal) {
    AreaFormCube* pCube = static_cast< AreaFormCube* >(mForm);
    s32 surface = -1;
    TPos3f matrix;
    pCube->calcWorldMtx(&matrix);
    pCube->calcWorldPos(&_44);
    TVec3f localPoint;
    TVec3f axisX;
    TVec3f axisY;
    TVec3f axisZ;
    matrix.getXDir(axisX);
    matrix.getYDir(axisY);
    matrix.getZDir(axisZ);
    f32 sizeX = axisX.length();
    MR::normalizeOrZero(&axisX);
    f32 sizeY = axisY.length();
    MR::normalizeOrZero(&axisY);
    f32 sizeZ = axisZ.length();
    MR::normalizeOrZero(&axisZ);
    sizeX *= 0.5f * (pCube->mScale.x * pCube->getBaseSize());
    sizeY *= 0.5f * (pCube->mScale.y * pCube->getBaseSize());
    sizeZ *= 0.5f * (pCube->mScale.z * pCube->getBaseSize());
    TVec3f expanded(sizeX + radius, sizeY + radius, sizeZ + radius);
    TVec3f relative(rPos);
    relative.sub(_44);
    f32 x = relative.dot(axisX);
    f32 y = relative.dot(axisY);
    f32 z = relative.dot(axisZ);
    TVec3f absolute;
    f32 absX = MR::abs(x);
    f32 absY = MR::abs(y);
    f32 absZ = MR::abs(z);
    absolute.set(absX, absY, absZ);
    if (absolute.x >= expanded.x || absolute.y >= expanded.y || absolute.z >= expanded.z) {
        return false;
    }

    u32 count = 0;
    bool outside[3] = {false, false, false};
    if (absolute.x >= sizeX) {
        outside[0] = true;
        count++;
    }

    if (absolute.y >= sizeY) {
        outside[1] = true;
        count++;
    }

    if (absolute.z >= sizeZ) {
        outside[2] = true;
        count++;
    }

    _3C = count;
    TVec3f corner;
    TVec3f depth;
    if (count == 3) {
        corner.zero();
        if (x < 0.0f) {
            corner.add(-axisX * sizeX);
        } else {
            corner.add(axisX * sizeX);
        }

        if (y < 0.0f) {
            corner.add(-axisY * sizeY);
        } else {
            corner.add(axisY * sizeY);
        }

        if (z < 0.0f) {
            corner.add(-axisZ * sizeZ);
        } else {
            corner.add(axisZ * sizeZ);
        }

        if ((corner - rPos).length() >= radius) {
            return false;
        }

        pPoint->set(corner);
        pNormal->set(axisX + axisY + axisZ);
        MR::normalizeOrZero(pNormal);
        return true;
    }

    if (count == 0) {
        depth.x = sizeX - absolute.x;
        depth.y = sizeY - absolute.y;
        depth.z = sizeZ - absolute.z;
        if (depth.x < depth.y && depth.x < depth.z) {
            _40 = depth.x;
            outside[0] = true;
        } else if (depth.y < depth.x && depth.y < depth.z) {
            _40 = depth.y;
            outside[1] = true;
        } else {
            _40 = depth.z;
            outside[2] = true;
        }

        count = 1;
    }

    if (count == 2) {
        localPoint.set(x, y, z);
        pNormal->zero();
        if (outside[0]) {
            if (x < 0.0f) {
                localPoint.x = -sizeX;
                pNormal->sub(axisX);
            } else {
                localPoint.x = sizeX;
                pNormal->add(axisX);
            }
        }

        if (outside[1]) {
            if (y < 0.0f) {
                localPoint.y = -sizeY;
                pNormal->sub(axisY);
            } else {
                localPoint.y = sizeY;
                pNormal->add(axisY);
            }
        }

        if (outside[2]) {
            if (z < 0.0f) {
                localPoint.z = -sizeZ;
                pNormal->sub(axisZ);
            } else {
                localPoint.z = sizeZ;
                pNormal->add(axisZ);
            }
        }
    }

    if (count == 1) {
        if (outside[0]) {
            if (x >= 0.0f) {
                pNormal->set(axisX);
                localPoint.set(sizeX, y, z);
                surface = 0;
            } else {
                pNormal->set(-axisX);
                localPoint.set(-sizeX, y, z);
                surface = 1;
            }
        }

        if (outside[1]) {
            if (y >= 0.0f) {
                pNormal->set(axisY);
                localPoint.set(x, sizeY, z);
                surface = 2;
            } else {
                pNormal->set(-axisY);
                localPoint.set(x, -sizeY, z);
                surface = 3;
            }
        }

        if (outside[2]) {
            if (z >= 0.0f) {
                pNormal->set(axisZ);
                localPoint.set(x, y, sizeZ);
                surface = 4;
            } else {
                pNormal->set(-axisZ);
                localPoint.set(x, y, -sizeZ);
                surface = 5;
            }
        }
    }

    pPoint->set(axisX * localPoint.x + axisY * localPoint.y + axisZ * localPoint.z + _44);
    MR::normalizeOrZero(pNormal);
    if (mPolygon != nullptr && (_60 & (1 << surface))) {
        mPolygon->setSurfaceAndSync(surface);
    }

    return true;
}

void AreaPolygon::setMtx(MtxPtr pMatrix, const TVec3f& rSize, f32 offset) {
    _128 = pMatrix;
    _12C = rSize;
    _138 = offset;
}

void AreaPolygon::setSurfaceAndSync(s32 surface) {
    setSurface(surface);

    if (surface != -1) {
        syncCollision();
    }
}

void AreaPolygon::setSurface(s32 surface) {
    TPos3f matrix;
    if (mForm != nullptr) {
        static_cast< AreaFormCube* >(mForm)->calcWorldMtx(&matrix);
        static_cast< AreaFormCube* >(mForm)->calcWorldPos(&mPosition);
    } else {
        PSMTXCopy(_128, matrix);
        MR::extractMtxTrans(_128, &mPosition);
    }

    TVec3f axisX;
    TVec3f axisY;
    TVec3f axisZ;
    matrix.getXDir(axisX);
    matrix.getYDir(axisY);
    matrix.getZDir(axisZ);
    if (_128 != nullptr) {
        mPosition.add(axisY * _138);
    }

    f32 sizeX = axisX.length();
    MR::normalizeOrZero(&axisX);
    f32 sizeY = axisY.length();
    MR::normalizeOrZero(&axisY);
    f32 sizeZ = axisZ.length();
    MR::normalizeOrZero(&axisZ);
    if (mForm != nullptr) {
        sizeX *= 0.5f * (static_cast< AreaFormCube* >(mForm)->getSizeX());
        sizeY *= 0.5f * (static_cast< AreaFormCube* >(mForm)->getSizeY());
        sizeZ *= 0.5f * (static_cast< AreaFormCube* >(mForm)->getSizeZ());
    } else {
        sizeX *= 0.5f * _12C.x;
        sizeY *= 0.5f * _12C.y;
        sizeZ *= 0.5f * _12C.z;
    }

    if (!MR::isPlayerElementModeTeresa()) {
        if (sizeX < 0.0f) {
            sizeX -= 10.0f;
        } else {
            sizeX += 10.0f;
        }

        if (sizeY < 0.0f) {
            sizeY -= 10.0f;
        } else {
            sizeY += 10.0f;
        }

        if (sizeZ < 0.0f) {
            sizeZ -= 10.0f;
        } else {
            sizeZ += 10.0f;
        }
    }

    switch (surface) {
    case 0:
        mPositions[0].set(sizeX, -sizeY, -sizeZ);
        mPositions[1].set(sizeX, sizeY, -sizeZ);
        mPositions[2].set(sizeX, sizeY, sizeZ);
        mPositions[3].set(sizeX, -sizeY, sizeZ);
        break;
    case 1:
        mPositions[0].set(-sizeX, -sizeY, sizeZ);
        mPositions[1].set(-sizeX, sizeY, sizeZ);
        mPositions[2].set(-sizeX, sizeY, -sizeZ);
        mPositions[3].set(-sizeX, -sizeY, -sizeZ);
        break;
    case 2:
        mPositions[0].set(-sizeX, sizeY, -sizeZ);
        mPositions[1].set(-sizeX, sizeY, sizeZ);
        mPositions[2].set(sizeX, sizeY, sizeZ);
        mPositions[3].set(sizeX, sizeY, -sizeZ);
        break;
    case 3:
        mPositions[0].set(-sizeX, -sizeY, sizeZ);
        mPositions[1].set(-sizeX, -sizeY, -sizeZ);
        mPositions[2].set(sizeX, -sizeY, -sizeZ);
        mPositions[3].set(sizeX, -sizeY, sizeZ);
        break;
    case 4:
        mPositions[0].set(-sizeX, -sizeY, sizeZ);
        mPositions[1].set(sizeX, -sizeY, sizeZ);
        mPositions[2].set(sizeX, sizeY, sizeZ);
        mPositions[3].set(-sizeX, sizeY, sizeZ);
        break;
    case 5:
        mPositions[0].set(-sizeX, sizeY, -sizeZ);
        mPositions[1].set(sizeX, sizeY, -sizeZ);
        mPositions[2].set(sizeX, -sizeY, -sizeZ);
        mPositions[3].set(-sizeX, -sizeY, -sizeZ);
        break;
    }

    for (u32 i = 0; i < 4; i++) {
        mPositions[i].set(axisX * mPositions[i].x + axisY * mPositions[i].y + axisZ * mPositions[i].z);
    }
}

void AreaPolygon::invalidate() {
    MR::invalidateCollisionParts(mParts);
}

void AreaPolygon::validate() {
    MR::validateCollisionParts(mParts);
}

AreaPolygon::~AreaPolygon() {
}

CollisionArea::~CollisionArea() {
}

AreaPolygon::AreaPolygon() : DynamicCollisionObj("エリアポリゴン") {
    mForm = nullptr;
    _128 = nullptr;
    _12C.zero();
}

void AreaPolygon::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
    } else {
        MR::extractMtxTrans(_128, &mPosition);
        mRotation.set(0.0f, 0.0f, 0.0f);
    }

    mScale.set(1.0f, 1.0f, 1.0f);
    mKCLFile = nullptr;
    mPositionNum = 4;
    _94 = 2;
    mPositions = new TVec3f[mPositionNum];
    setSurface(0);

    initHitSensor(1);
    MR::addHitSensorEye(this, "body", 8, 1.0f, TVec3f(0.0f));

    mIndices = new TriangleIndexing[_94];
    _9C = new TVec3f[_94];

    TriangleIndexing& rFirst = mIndices[0];
    rFirst.mIndex[0] = 0;
    rFirst.mIndex[1] = 1;
    rFirst.mIndex[2] = 2;

    TriangleIndexing& rSecond = mIndices[1];
    rSecond.mIndex[0] = 0;
    rSecond.mIndex[1] = 2;
    rSecond.mIndex[2] = 3;

    createCollision();
    makeActorAppeared();
}
