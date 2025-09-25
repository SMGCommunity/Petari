#include "Game/MapObj/ClipAreaDropLaser.hpp"
#include "Game/MapObj/ClipAreaDropHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include "revolution/gx/GXPixel.h"
#include "Game/MapObj/ClipHolder.h"

ClipAreaDropLaser::ClipAreaDropLaser(const char* pName) : LiveActor(pName) {
    _38C = 0.0f;
    _390 = -1;
    _394 = -1;
    _398 = 20.0f;
    MR::createClipAreaDropHolder();
}

void ClipAreaDropLaser::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 0x22, -1, -1, 0x14);
    initRailRider(rIter);
    MR::moveCoordAndTransToRailStartPoint(this);
    _398 = 20.0f;
    MR::getJMapInfoArg0NoInit(rIter, &_398);
    initEffectKeeper(0, "ClipAreaDropLaser", false);
    initSound(4, false);
    initNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance);
    MR::invalidateClipping(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
    }
    makeActorAppeared();
}
void ClipAreaDropLaser::control() {

}

void ClipAreaDropLaser::draw() const {
    TDDraw::cameraInit3D();
    TDDraw::setup(0, 2, 0);
    GXSetLineWidth(0x14, GX_TO_ZERO);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (int r29 = 1; r29 < _38C; r29++) {
        int u1 = _394-(r29-1);
        int u2 = _394-r29;
        
        if (u1 < 0)
            u1 += 0x40;
        if (u2 < 0)
            u2 += 0x40;

        if (u1 != _390) {
            TDDraw::drawLine(_8C[u1], _8C[u2], 0x40F080);
        }
    }
}

void ClipAreaDropLaser::exeWait() {
    if (_38C > 0.0f) {
        _38C = _38C - 1.0f;
    }

    if (MR::isValidSwitchAppear(this) && MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance);
}

void ClipAreaDropLaser::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::moveCoordAndTransToRailStartPoint(this);
        _38C = 0.0f;
    }
    s32 railPoint = 0;
    railPoint = MR::moveCoordAndCheckPassPointNo(this, _398);
    MR::moveTransToCurrentRailPos(this);
    incrementDrawCount();
    if (MR::isRailReachedGoal(this)) {
        _390 = _394;
        railPoint = MR::getRailPointNum(this)-1;
        MR::moveCoordAndTransToRailStartPoint(this);
    }
    _8C[_394].set(mPosition);
    if (railPoint != -1) {
        TVec3f pos;
        MR::calcRailPointPos(&pos, this, railPoint);
        f32 arg = -1.0;
        MR::getRailPointArg0NoInit(this, railPoint, &arg);
        if (arg > 0.0f) {
            MR::emitEffectHit(this, pos, "Splash");
            MR::startSound(this, "SE_OJ_DROP_LASER_SPLASH", -1, -1);
            MR::appearClipAreaDrop(pos, arg);
        }
    }

    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
}

void ClipAreaDropLaser::incrementDrawCount() {
    _394 += 1;
    if (_394 >= 0x40) {
        _394 += -0x40;
    }
    if (_38C < 64.0f) {
        _38C += 1.0f;
    }
}

ClipAreaDropLaser::~ClipAreaDropLaser() {

}

namespace NrvClipAreaDropLaser {
    ClipAreaDropLaserNrvWait(ClipAreaDropLaserNrvWait::sInstance);
    ClipAreaDropLaserNrvMove(ClipAreaDropLaserNrvMove::sInstance);
}
