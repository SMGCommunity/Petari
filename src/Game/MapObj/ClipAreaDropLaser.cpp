#include "Game/MapObj/ClipAreaDropLaser.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/ClipAreaDropHolder.hpp"
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

namespace NrvClipAreaDropLaser {
    NEW_NERVE(ClipAreaDropLaserNrvWait, ClipAreaDropLaser, Wait);
    NEW_NERVE(ClipAreaDropLaserNrvMove, ClipAreaDropLaser, Move);
};  // namespace NrvClipAreaDropLaser

ClipAreaDropLaser::ClipAreaDropLaser(const char* pName) : LiveActor(pName) {
    mNumPointsToDraw = 0.0f;
    mPointIndexToSkipDraw = -1;
    mDrawCount = -1;
    mSpeed = 20.0f;
    MR::createClipAreaDropHolder();
}

void ClipAreaDropLaser::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 0x22, -1, -1, 0x14);
    initRailRider(rIter);
    MR::moveCoordAndTransToRailStartPoint(this);
    mSpeed = 20.0f;
    MR::getJMapInfoArg0NoInit(rIter, &mSpeed);
    initEffectKeeper(0, "ClipAreaDropLaser", false);
    initSound(4, false);
    initNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance);
    MR::invalidateClipping(this);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvWait::sInstance);
    }
    makeActorAppeared();
}
void ClipAreaDropLaser::control() {}

void ClipAreaDropLaser::draw() const {
    TDDraw::cameraInit3D();
    TDDraw::setup(0, 2, 0);
    GXSetLineWidth(0x14, GX_TO_ZERO);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (int r29 = 1; r29 < mNumPointsToDraw; r29++) {
        int u1 = mDrawCount - (r29 - 1);
        int u2 = mDrawCount - r29;

        if (u1 < 0)
            u1 += 0x40;
        if (u2 < 0)
            u2 += 0x40;

        if (u1 != mPointIndexToSkipDraw) {
            TDDraw::drawLine(mPoints[u1], mPoints[u2], 0x40F080);
        }
    }
}

void ClipAreaDropLaser::exeWait() {
    if (mNumPointsToDraw > 0.0f) {
        mNumPointsToDraw = mNumPointsToDraw - 1.0f;
    }

    if (MR::isValidSwitchAppear(this) && MR::isOnSwitchAppear(this))
        setNerve(&NrvClipAreaDropLaser::ClipAreaDropLaserNrvMove::sInstance);
}

void ClipAreaDropLaser::exeMove() {
    if (MR::isFirstStep(this)) {
        MR::moveCoordAndTransToRailStartPoint(this);
        mNumPointsToDraw = 0.0f;
    }
    s32 railPoint = 0;
    railPoint = MR::moveCoordAndCheckPassPointNo(this, mSpeed);
    MR::moveTransToCurrentRailPos(this);
    incrementDrawCount();
    if (MR::isRailReachedGoal(this)) {
        mPointIndexToSkipDraw = mDrawCount;
        railPoint = MR::getRailPointNum(this) - 1;
        MR::moveCoordAndTransToRailStartPoint(this);
    }
    mPoints[mDrawCount].set(mPosition);
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
    mDrawCount += 1;
    if (mDrawCount >= 0x40) {
        mDrawCount += -0x40;
    }
    if (mNumPointsToDraw < 64.0f) {
        mNumPointsToDraw += 1.0f;
    }
}

ClipAreaDropLaser::~ClipAreaDropLaser() {}
