#include "Game/MapObj/TransparentWall.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/InvisiblePolygonObj.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

TransparentWall::TransparentWall(const char* pName) : InvisiblePolygonObj(pName) {}

TransparentWall::~TransparentWall() {}

void TransparentWall::init(const JMapInfoIter& rIfter) {
    InvisiblePolygonObj::init(rIfter);
    MR::getJMapInfoArg4WithInit(rIfter, &_BC);
    makeActorAppeared();
    _C0 = false;
    MR::invalidateCollisionParts(this);
    HitSensor* sensor = getSensor("body");
    sensor->setType(87);
}

void TransparentWall::control() {
    if (!_C0) {
        for (u32 i = 0; i < 10; i++) {
            if (_BC & (1 << i) && MR::isPlayerElementMode(i)) {
                _C0 = true;
                break;
            }
        }

        if ((_BC & 1024) && MR::isPlayerSkating()) {
            _C0 = true;
        }

        if ((_BC & 2048) && !MR::isPlayerTeresaDisappear()) {
            _C0 = true;
        }

        if (_C0) {
            MR::validateCollisionParts(this);
        }
    } else {
        bool r29 = true;
        for (u32 i = 0; i < 10; i++) {
            if (_BC & (1 << i) && MR::isPlayerElementMode(i)) {
                r29 = false;
                break;
            }
        }

        if ((_BC & 1024) && MR::isPlayerSkating()) {
            r29 = false;
        }

        if ((_BC & 2048)) {
            r29 = MR::isPlayerTeresaDisappear();
        }

        if (r29) {
            MR::invalidateCollisionParts(this);
            _C0 = false;
        }
    }
}
