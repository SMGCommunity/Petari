#include "Game/Demo/ReturnDemoRailMove.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

void ReturnDemoRailMove_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    void setResultFlyStartFrame(LiveActor* liveActor, s32 frame) NO_INLINE {
        int maxFrames = MR::getBckFrameMax(liveActor);
        MR::setBckFrame(liveActor, maxFrames - frame % maxFrames);
    }
};  // namespace

ReturnDemoRailMove::ReturnDemoRailMove(LiveActor* pDemoStarter, LiveActor* pPowerStar, const JMapInfoIter& rIter, bool isGrandstar,
                                       TPos3f* pTransform)
    : mDemoStarter(pDemoStarter), mPowerStar(pPowerStar), mIsGrandStar(isGrandstar), mTransform(pTransform), mShootPath(nullptr),
      mPathDrawer(nullptr), mForward(0.0f, 0.0f, 1.0f) {
    mShootPath = new SpinDriverShootPath();
    mShootPath->init(rIter);

    mPathDrawer = new SpinDriverPathDrawer(mShootPath);
    mPathDrawer->initWithoutIter();
    mPathDrawer->setColorNormal();

    calcPathPosDir(nullptr, &mForward, 1.0f);
}

void ReturnDemoRailMove::posToStart() {
    TVec3f position;
    TVec3f forward;
    calcPathPosDir(&position, &forward, 0.0f);

    MR::makeMtxUpFront(getMtx(), forward, TVec3f(0.0f, -1.0f, 0.0f));
    mTransform->setTrans(position);
    MR::setPlayerBaseMtx(*mTransform);
};

void ReturnDemoRailMove::posToEnd() {
    TVec3f position;
    calcPathPosDir(&position, nullptr, 1.0f);

    MR::makeMtxUpFront(getMtx(), TVec3f(0.0f, 1.0f, 0.0f), mForward);
    mTransform->setTrans(position);
    MR::setPlayerBaseMtx(*mTransform);
};

void ReturnDemoRailMove::offPathDraw() {
    mPathDrawer->kill();
};

s32 ReturnDemoRailMove::getDemoFlyBrakeFrame() const {
    return (mIsGrandStar) ? 296 : 45;
};

void ReturnDemoRailMove::calcPathPosDir(TVec3f* position, TVec3f* direction, f32 t) const {
    if (position != nullptr) {
        mShootPath->calcPosition(position, t);
    }

    if (direction != nullptr) {
        mShootPath->calcDirection(direction, t, 0.01f);
    }
};

void ReturnDemoRailMove::setupPathDrawForGraneStarReturnDemo() {
    mPathDrawer->setMaskLength(100.0f);
    mPathDrawer->setFadeScale(0.1f);
};

void ReturnDemoRailMove::start() {
    const char* pBckName = (mIsGrandStar) ? "ResultFlyGrandStar" : "ResultFly";
    MR::startBckPlayer(pBckName, static_cast< const char* >(nullptr));

    MR::startBck(mPowerStar, pBckName, nullptr);
    mPathDrawer->_B0 = 0.0f;
    mPathDrawer->appear();
};

void ReturnDemoRailMove::update(s32 currentStep, s32 maxSteps) {
    int startStepFirstDemo = maxSteps - getDemoFlyBrakeFrame();
    int startStepSecondDemo = maxSteps - ((mIsGrandStar) ? 98 : 34);

    f32 progress = (static_cast< f32 >(currentStep) / maxSteps) - 1.0f;
    f32 t = 1.0f - progress * progress;

    if ((startStepFirstDemo < 0 && MR::isFirstStep(mDemoStarter)) || MR::isStep(mDemoStarter, startStepFirstDemo)) {
        const char* pBckName = (mIsGrandStar) ? "ResultFlyGrandStarEnd" : "ResultFlyEnd";

        MR::startBckPlayer(pBckName, static_cast< const char* >(nullptr));
        MR::startBck(mPowerStar, pBckName, nullptr);

        if (!mIsGrandStar) {
            MR::startSoundPlayer("SE_PM_S_SPIN_DRV_COOL_DOWN", -1);
        }
    }

    if (MR::isFirstStep(mDemoStarter)) {
        if (startStepFirstDemo < 0) {
            startStepFirstDemo = -startStepFirstDemo;
            MR::setBckFrame(MR::getPlayerDemoActor(), startStepFirstDemo);
            MR::setBckFrame(mPowerStar, startStepFirstDemo);
        } else {
            setResultFlyStartFrame(MR::getPlayerDemoActor(), startStepFirstDemo);
            setResultFlyStartFrame(mPowerStar, startStepFirstDemo);
        }
    }

    MR::startLevelSound(mPowerStar, "SE_OJ_LV_RET_POW_STAR_FLY");

    TVec3f position;
    TVec3f direction;
    calcPathPosDir(&position, &direction, t);

    MR::makeMtxUpFront(getMtx(), direction, TVec3f(0.0f, -1.0f, 0.0f));

    if (MR::isGreaterStep(mDemoStarter, startStepSecondDemo)) {
        f32 rate = MR::calcNerveEaseOutRate(mDemoStarter, startStepSecondDemo, maxSteps);

        TPos3f transform;
        MR::makeMtxUpFront(&transform, TVec3f(0.0f, 1.0f, 0.0f), mForward);
        MR::blendMtxRotateSlerp(*mTransform, transform, rate, *mTransform);
    }

    mTransform->setTrans(position);
    mPathDrawer->setCoord(t);
    MR::setPlayerBaseMtx(*mTransform);
};
