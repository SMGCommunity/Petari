#include "Game/Screen/WipeRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

void WipeRing_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)PI;
}

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
    initNerve(GET_NERVE_ANON(WipeRingNrvOpen));
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

    const f32 angle = (1.0f + static_cast< f32 >(getNerveStep()) / mAnimFrame) * JGeometry::TUtil< f32 >::PI();
    f32 f = MR::sin(0.5f * angle);

    MR::getAnimCtrl(this, 0)->setFrame(mWipeInFrame * (1.0f - f));

    if (MR::isStep(this, mAnimFrame)) {
        setNerve(GET_NERVE_ANON(WipeRingNrvOpen));
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

    const f32 angle = (static_cast< f32 >(getNerveStep()) / mAnimFrame) * JGeometry::TUtil< f32 >::PI();
    f32 f = MR::sin(0.5f * angle);

    MR::getAnimCtrl(this, 0)->setFrame(mWipeOutFrame * (1.0f - f) + f * MR::getAnimCtrl(this, 0)->getEnd());

    if (MR::isStep(this, mAnimFrame)) {
        setNerve(GET_NERVE_ANON(WipeRingNrvClose));
    }
}

void WipeRing::wipe(s32 frame) {
    if (frame < 1) {
        mAnimFrame = 90;
    } else {
        mAnimFrame = frame;
    }

    if (isNerve(GET_NERVE_ANON(WipeRingNrvClose))) {
        setNerve(GET_NERVE_ANON(WipeRingNrvWipeIn));
        startAnim(::sInAnimName);
    } else if (isNerve(GET_NERVE_ANON(WipeRingNrvWipeIn))) {
        setNerve(GET_NERVE_ANON(WipeRingNrvWipeOut));
        startAnim(::sOutAnimName);
    } else if (isNerve(GET_NERVE_ANON(WipeRingNrvOpen))) {
        if (MR::isDead(this)) {
            appear();
        }

        setNerve(GET_NERVE_ANON(WipeRingNrvWipeOut));
        startAnim(::sOutAnimName);
    } else if (isNerve(GET_NERVE_ANON(WipeRingNrvWipeOut))) {
        setNerve(GET_NERVE_ANON(WipeRingNrvWipeIn));
        startAnim(::sInAnimName);
    }
}

void WipeRing::forceClose() {
    if (MR::isDead(this)) {
        appear();
    }

    MR::startAnim(this, ::sInAnimName, 0);
    MR::setAnimFrameAndStop(this, 0.0f, 0);
    setNerve(GET_NERVE_ANON(WipeRingNrvClose));
}

void WipeRing::forceOpen() {
    kill();
    setNerve(GET_NERVE_ANON(WipeRingNrvOpen));
}

bool WipeRing::isOpen() const {
    return isNerve(GET_NERVE_ANON(WipeRingNrvOpen));
}

bool WipeRing::isClose() const {
    return isNerve(GET_NERVE_ANON(WipeRingNrvClose));
}

bool WipeRing::isWipeIn() const {
    return isNerve(GET_NERVE_ANON(WipeRingNrvWipeIn));
}

bool WipeRing::isWipeOut() const {
    return isNerve(GET_NERVE_ANON(WipeRingNrvWipeOut));
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

namespace {
    bool canCenterOnMario() {
        if (MR::isEqualStageName("IceVolcanoGalaxy") && MR::getCurrentScenarioNo() == 1) {
            TVec3f position(15060.0f, -11800.0f, 1260.0f);
            if (MR::getPlayerPos()->distance(position) < 600.0f) {
                return false;
            }
        }

        return !MR::isStageSuddenDeathDodoryu();
    }
}  // namespace

bool WipeRing::getMarioCenterPos(TVec3f* pCenterPos) {
    if (MR::isExistMario() && ::canCenterOnMario()) {
        pCenterPos->set(static_cast< const Vec& >(*MR::getPlayerCenterPos()));

        return true;
    }

    pCenterPos->zero();
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
    return MR::fastSqrtf(900160.0f);
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
