#include "Game/Screen/WipeRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace {
    static const s32 sAnimFrame = 60;
    static const char* sOutAnimName = "Out";
    static const char* sInAnimName = "In";
};  // namespace

namespace {
    NEW_NERVE(WipeRingNrvClose, WipeRing, Close);
    NEW_NERVE(WipeRingNrvWipeIn, WipeRing, WipeIn);
    NEW_NERVE(WipeRingNrvOpen, WipeRing, Open);
    NEW_NERVE(WipeRingNrvWipeOut, WipeRing, WipeOut);
};  // namespace

WipeRing::WipeRing(bool isCenterOnPlayer, const char* pName)
    : WipeLayoutBase(pName), mCenterPos(0.0f, 0.0f), mAnimFrame(::sAnimFrame), mWipeOutFrame(), mWipeInFrame(), mIsCenterOnPlayer(isCenterOnPlayer) {
}

void WipeRing::init(const JMapInfoIter& rIter) {
    initLayoutManager("WipeRing", 1);
    MR::createAndAddPaneCtrl(this, "Ring", 1);
    MR::setFollowPos(&mCenterPos, this, "Ring");
    initNerve(&::WipeRingNrvOpen::sInstance);
    kill();
}

void WipeRing::exeClose() {
    updatePlayerPos();
}

void WipeRing::exeWipeIn() {
    updatePlayerPos();

    if (MR::isFirstStep(this)) {
        f32 radiusRate = calcRadius() / calcMaxRadius();

        mWipeInFrame = radiusRate * MR::getAnimCtrl(this, 0)->getEnd();
        MR::getAnimCtrl(this, 0)->setRate(0.0f);
    }

    f32 f = MR::sin(((1.0f + static_cast< f32 >(getNerveStep()) / mAnimFrame) / 2.0f) * PI);

    MR::getAnimCtrl(this, 0)->setFrame(mWipeInFrame * (1.0f - f));

    if (MR::isStep(this, mAnimFrame)) {
        setNerve(&::WipeRingNrvOpen::sInstance);
    }
}

void WipeRing::exeOpen() {
    updatePlayerPos();

    if (MR::isFirstStep(this)) {
        kill();
    }
}

void WipeRing::exeWipeOut() {
    updatePlayerPos();

    if (MR::isFirstStep(this)) {
        f32 invRadiusRate = 1.0f - calcRadius() / calcMaxRadius();

        mWipeOutFrame = invRadiusRate * MR::getAnimCtrl(this, 0)->getEnd();
        MR::getAnimCtrl(this, 0)->setRate(0.0f);
    }

    f32 f = MR::sin(((static_cast< f32 >(getNerveStep()) / mAnimFrame) / 2.0f) * PI);

    MR::getAnimCtrl(this, 0)->setFrame(mWipeOutFrame * (1.0f - f) + f * MR::getAnimCtrl(this, 0)->getEnd());

    if (MR::isStep(this, mAnimFrame)) {
        setNerve(&::WipeRingNrvClose::sInstance);
    }
}

void WipeRing::wipe(s32 frame) {
    if (frame < 1) {
        mAnimFrame = 90;
    } else {
        mAnimFrame = frame;
    }

    if (isNerve(&::WipeRingNrvClose::sInstance)) {
        setNerve(&::WipeRingNrvWipeIn::sInstance);
        startAnim(::sInAnimName);
    } else if (isNerve(&::WipeRingNrvWipeIn::sInstance)) {
        setNerve(&::WipeRingNrvWipeOut::sInstance);
        startAnim(::sOutAnimName);
    } else if (isNerve(&::WipeRingNrvOpen::sInstance)) {
        if (MR::isDead(this)) {
            appear();
        }

        setNerve(&::WipeRingNrvWipeOut::sInstance);
        startAnim(::sOutAnimName);
    } else if (isNerve(&::WipeRingNrvWipeOut::sInstance)) {
        setNerve(&::WipeRingNrvWipeIn::sInstance);
        startAnim(::sInAnimName);
    }
}

void WipeRing::forceClose() {
    if (MR::isDead(this)) {
        appear();
    }

    MR::startAnim(this, ::sInAnimName, 0);
    MR::setAnimFrameAndStop(this, 0.0f, 0);
    setNerve(&::WipeRingNrvClose::sInstance);
}

void WipeRing::forceOpen() {
    kill();
    setNerve(&::WipeRingNrvOpen::sInstance);
}

bool WipeRing::isOpen() const {
    return isNerve(&::WipeRingNrvOpen::sInstance);
}

bool WipeRing::isClose() const {
    return isNerve(&::WipeRingNrvClose::sInstance);
}

bool WipeRing::isWipeIn() const {
    return isNerve(&::WipeRingNrvWipeIn::sInstance);
}

bool WipeRing::isWipeOut() const {
    return isNerve(&::WipeRingNrvWipeOut::sInstance);
}

void WipeRing::setCenterPos(const TVec3f& rPos) {
    if (MR::calcScreenPosition(&mCenterPos, rPos)) {
        return;
    }

    mCenterPos.set(MR::getScreenWidth() * 0.5f, MR::getScreenHeight() * 0.5f);
}

void WipeRing::startAnim(const char* pAnimName) {
    MR::startAnim(this, pAnimName, 0);
    MR::getAnimCtrl(this, 0)->setRate(static_cast< f32 >(MR::getAnimCtrl(this, 0)->getEnd()) / mAnimFrame);
}

bool WipeRing::getMarioCenterPos(TVec3f* pCenterPos) {
    if (MR::isExistMario()) {
        if (!MR::isEqualStageName("IceVolcanoGalaxy") || MR::getCurrentScenarioNo() != 1) {
            TVec3f pos(15060.0f, -11800.0f, 1260.0f);

            if (MR::getPlayerPos()->distance(pos) < 600.0f) {
            }
        }

        if (MR::isStageSuddenDeathDodoryu()) {
            pCenterPos->set(*MR::getPlayerCenterPos());

            return true;
        }
    }

    pCenterPos->set(0.0f);

    return false;
}

f32 WipeRing::calcRadius() const {
    f32 radius = mCenterPos.length();

    TVec2f screenHeightPos = TVec2f(0.0f, static_cast< f32 >(MR::getScreenHeight()));
    f32 screenHeightDistance = mCenterPos.distance(screenHeightPos);

    if (radius < screenHeightDistance) {
        radius = screenHeightDistance;
    }

    TVec2f screenWidthPos = TVec2f(static_cast< f32 >(MR::getScreenWidth()), 0.0f);
    f32 screenWidthDistance = mCenterPos.distance(screenWidthPos);

    if (radius < screenWidthDistance) {
        radius = screenWidthDistance;
    }

    TVec2f screenMaxPos = TVec2f(static_cast< f32 >(MR::getScreenWidth()), static_cast< f32 >(MR::getScreenHeight()));
    f32 screenMaxDistance = mCenterPos.distance(screenMaxPos);

    if (radius < screenMaxDistance) {
        radius = screenMaxDistance;
    }

    return radius;
}

f32 WipeRing::calcMaxRadius() const {
    return JGeometry::TUtil< f32 >::inv_sqrt(900160.0f);
}

void WipeRing::updatePlayerPos() {
    if (!mIsCenterOnPlayer) {
        return;
    }

    TVec3f marioCenterPos;

    if (!getMarioCenterPos(&marioCenterPos) && !MR::isFirstStep(this)) {
        return;
    }

    if (MR::calcScreenPosition(&mCenterPos, marioCenterPos)) {
        return;
    }

    mCenterPos.set(MR::getScreenWidth() * 0.5f, MR::getScreenHeight() * 0.5f);
}
