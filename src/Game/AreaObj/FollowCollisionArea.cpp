#include "Game/AreaObj/FollowCollisionArea.hpp"
#include "Game/AreaObj/CollisionArea.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void FollowCollisionArea_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)0.5f;
    (void)5.0f;
}

FollowCollisionArea::~FollowCollisionArea() {
}

FollowCollisionArea::FollowCollisionArea(const TVec3f& rSize, MtxPtr pMatrix, f32 offset) : NameObj("追従コリジョンエリア") {
    _38 = rSize;
    _34 = pMatrix;
    _44 = offset;
    MR::connectToSceneAreaObj(this);
    _C = 0;
    _10 = 0.0f;
    _20 = -1;
    _24 = 0;
    _2C = 0;
    _28 = false;
    _30 = new AreaPolygon();
    _30->setMtx(_34, _38, _44);
    _30->initWithoutIter();
    _30->validate();
}

void FollowCollisionArea::movement() {
    if (_24 == -1 && MR::isPlayerTeresaDisappear()) {
        _28 = true;
        _30->invalidate();
    } else {
        TVec3f playerPos(*MR::getPlayerCenterPos());
        f32 radius = static_cast< f32 >(_2C);
        if (_2C == 0) {
            radius = MR::getPlayerHitRadius();
        }

        if (_28) {
            radius += 5.0f;
        }

        TVec3f hitPos;
        TVec3f hitNormal;
        if (!hitCheck(playerPos, radius, &hitPos, &hitNormal)) {
            if (_28) {
                _28 = false;
                _30->validate();
            }
        } else if (!_28 && _20 == -1) {
            TVec3f push;
            if (_C == 0) {
                push = hitNormal;
                push.setLength(radius + _10);
            } else {
                push = playerPos - hitPos;
                push.setLength(radius);
                TVec3f contact(hitPos + push);
                push = contact - playerPos;
            }

            if (push.dot(hitNormal) > 0.0f) {
                MR::pushPlayerFromArea(push);
            }
        }
    }
}

bool FollowCollisionArea::hitCheck(const TVec3f& rPos, f32 radius, TVec3f* pPoint, TVec3f* pNormal) {
    s32 surface = -1;
    TPos3f matrix;
    PSMTXCopy(_34, matrix);
    MR::extractMtxTrans(_34, &_14);
    TVec3f localPoint;
    TVec3f axisX;
    TVec3f axisY;
    TVec3f axisZ;
    matrix.getXDir(axisX);
    matrix.getYDir(axisY);
    matrix.getZDir(axisZ);
    _14.add(axisY * _44);

    f32 sizeX = axisX.length();
    MR::normalizeOrZero(&axisX);
    f32 sizeY = axisY.length();
    MR::normalizeOrZero(&axisY);
    f32 sizeZ = axisZ.length();
    MR::normalizeOrZero(&axisZ);
    sizeX *= 0.5f * _38.x;
    sizeY *= 0.5f * _38.y;
    sizeZ *= 0.5f * _38.z;
    TVec3f expanded(sizeX + radius, sizeY + radius, sizeZ + radius);
    TVec3f relative(rPos);
    relative.sub(_14);
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

    _C = count;
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
            _10 = depth.x;
            outside[0] = true;
        } else if (depth.y < depth.x && depth.y < depth.z) {
            _10 = depth.y;
            outside[1] = true;
        } else {
            _10 = depth.z;
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

    pPoint->set(axisX * localPoint.x + axisY * localPoint.y + axisZ * localPoint.z + _14);
    MR::normalizeOrZero(pNormal);
    _30->setSurfaceAndSync(surface);

    return true;
}
