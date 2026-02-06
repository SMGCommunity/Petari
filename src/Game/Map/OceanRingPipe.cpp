#include "Game/Map/OceanRing.hpp"
#include "Game/Map/OceanRingPipe.hpp"
#include "Game/Map/OceanRingPipeInside.hpp"
#include "Game/Map/OceanRingPipeOutside.hpp"

#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Map/Flag.hpp"

static const char* FlagObjName = "旗";

OceanRingPipe::~OceanRingPipe() {
}


OceanRingPipe::OceanRingPipe(const OceanRing* ring, f32 a, f32 b) : LiveActor("オーシャンリングの側面"),
	mOceanRing(ring), _90(0), _94(0), _98(0), _9C(8), _A0(0), _A4(0), _A8(a), _AC(b), mPipeInside(0), mPipeOutside(0) {

}

void OceanRingPipe::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    initRailRider(rIter);
    initPoints();
    MR::invalidateClipping(this);
    mPipeInside = new OceanRingPipeInside(this);
    mPipeInside->initWithoutIter();
    mPipeOutside = new OceanRingPipeOutside(this);
    mPipeOutside->initWithoutIter();

    if (MR::isName(mOceanRing, "オーシャンリング（旗）")) {
        f32 f30 = 300.0f;
        f32 f29 = 200.0f;
        f32 f28 = 500.0f;
        TVec3f vec(1.0f, 0.0f, 0.0f);
        for (s32 i = 0; i < _98; i += 7) {
            TVec3f grav(0.0f, -1.0f, 0.0f);
            MR::calcGravityVector(this, _A0[i * _9C], &grav, nullptr, 0);
            if (grav.y > -0.89999998f)
                continue;

            if (i & 1) {
                MR::setClippingFarMax(MR::createMapFlag(FlagObjName, "FlagSurfing", &_A0[i * _9C], vec, f28, f30, f29, 2, 3, -1.0f));
            } else {
                MR::setClippingFarMax(MR::createMapFlag(FlagObjName, "FlagSurfing", &_A0[(i-1) + (i * _9C)], vec, f28, f30, f29, 2, 3, -1.0f));
            }
        }
    }

    makeActorAppeared();
}

void OceanRingPipe::movement() {
    if (!MR::isValidMovement(this)) {
        return;
    }
    mPipeOutside->movement();
    if (WaterAreaFunction::getCameraWaterInfo()->mOceanRing == mOceanRing) {
        _90 = true;
        mPipeInside->movement();
    } else {
        _90 = false;
    }
}

void OceanRingPipe::initPoints() {
    _98 = static_cast< s32 >(MR::getRailTotalLength(this) / 300.0f) + 1;
    _94 = _98 * _9C;

    f32 length = MR::getRailTotalLength(this) / (_98 - 1);
    f32 angle = getAngle();

    _A0 = new (32) TVec3f[_94];
    _A4 = new (32) TVec3s[_94];

    s32 idx = 0;

    for (s32 i = 0; i < _98; i++) {
        TVec3f grav;
        MR::calcGravityVector(this, MR::getRailPos(this), &grav, nullptr, 0);

        TVec3f side;
        side.cross(MR::getRailDirection(this), -grav);
        MR::normalize(&side);

        TPos3f mtx;
        mtx.identity();
        mtx.setRotateInline(MR::getRailDirection(this), angle);

        f32 currDist = 0.0f;
        f32 nextDist = 0.0f;
        MR::calcDistanceToCurrentAndNextRailPoint(this, &currDist, &nextDist);
        f32 sumDist = currDist + nextDist;

        f32 flt = _AC / 100.0f;
        f32 flt2 = _AC / 100.0f;
        MR::getCurrentRailPointArg1NoInit(this, &flt);
        MR::getNextRailPointArg1NoInit(this, &flt2);

        f32 f3 = 1.0f;
        TVec3f v1(side);

        if (sumDist > 0.0f) {
            // FIXME: Regswaps
            f32 f33 = flt * nextDist;
            f32 f34 = flt2 * currDist;
            f32 f35 = f33 + f34;
            f32 diva = (f35 / sumDist);
            f32 divb = (_AC / 100.0f);
            f3 = diva / divb;
            f32 fff = flt - flt2;

            if (__fabsf(fff) > 0.0001f) {
                TVec3f v2(side);
                TVec3f v3(side);
                v2.scale(flt);
                v3.scale(flt2);

                TVec3f railPos;
                MR::calcRailPosAtCoord(&railPos, this, MR::repeat(MR::getRailCoord(this) - 300.0f, 0.0f, MR::getRailTotalLength(this)));
                v2.add(railPos);

                MR::calcRailPosAtCoord(&railPos, this, MR::repeat(MR::getRailCoord(this) + 300.0f, 0.0f, MR::getRailTotalLength(this)));
                v3.add(railPos);

                v1.cross(-grav, *v2.subInline(v3));
                MR::normalize(&v1);
            }
        }

        for (s32 j = 0; j < _9C; j++) {
            TVec3f v4(side);
            v4.scale(_A8 * f3);
            v4.add(MR::getRailPos(this));

            _A0[idx].set< f32 >(v4.x, v4.y, v4.z);
            // w h a t
            // (TODO: look into how its actually supposed to convert these values into
            // their s16 components instead of writing the multiply directly)
            //JGeometry::TVec3< s64 >(v1.x * 32768.0f, v1.y * 32768.0f, v1.z * 32768.0f);
            _A4[idx].set(v1.x * 32768.0f, v1.y * 32768.0f, v1.z * 32768.0f);
            idx++;

            mtx.mult(side, side);
            mtx.mult(v1, v1);
        }
        MR::moveCoord(this, length);
    }
}


















