#include "Game/Ride/SurfRayTutorial.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/SurfingGuidance.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace NrvSurfRayTutorial {
    NEW_NERVE(SurfRayTutorialNrvTutorialAllStart, SurfRayTutorial, TutorialAllStart);
    NEW_NERVE(SurfRayTutorialNrvTutorialStraightStart, SurfRayTutorial, TutorialStraightStart);
    NEW_NERVE(SurfRayTutorialNrvTutorialStraightFailure, SurfRayTutorial, TutorialStraightFailure);
    NEW_NERVE(SurfRayTutorialNrvTutorialStraightUpkeep, SurfRayTutorial, TutorialStraightUpkeep);
    NEW_NERVE(SurfRayTutorialNrvTutorialStraightSuccess, SurfRayTutorial, TutorialStraightSuccess);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnLeftStart, SurfRayTutorial, TutorialTurnLeftStart);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnLeftFailure, SurfRayTutorial, TutorialTurnLeftFailure);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnLeftUpkeep, SurfRayTutorial, TutorialTurnLeftUpkeep);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnLeftSuccess, SurfRayTutorial, TutorialTurnLeftSuccess);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnRightStart, SurfRayTutorial, TutorialTurnRightStart);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnRightFailure, SurfRayTutorial, TutorialTurnRightFailure);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnRightUpkeep, SurfRayTutorial, TutorialTurnRightUpkeep);
    NEW_NERVE(SurfRayTutorialNrvTutorialTurnRightSuccess, SurfRayTutorial, TutorialTurnRightSuccess);
    NEW_NERVE(SurfRayTutorialNrvTutorialAllEnd, SurfRayTutorial, TutorialAllEnd);
};  // namespace NrvSurfRayTutorial

namespace {
    static const s32 sStepTalk = 150;
    static const s32 sStepToSuccess = 90;
    static const s32 sStepToOK = 10;
    static const f32 sPadAccelStraightMinX = 0.25f;
    static const f32 sPadAccelStraightMinY = 0.45f;
    // static const f32 sPadAccelTurnMinX = _;
    // static const f32 sPadAccelTurnLimitZ = _;
    // static const f32 sPadAccelTurnLimitY = _;
};  // namespace

SurfRayTutorial::SurfRayTutorial(LiveActor* pHost, TalkMessageCtrl* pTalkCtrl, const JMapInfoIter& rIter)
    : NerveExecutor("チュートリアル演出"), mHost(pHost), mIsTutorialPass(false), mPadAccel(0.0f, 0.0f, 0.0f), mTalkCtrl(pTalkCtrl), mChangeStep(0),
      _28(0) {
    initNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialAllStart::sInstance);

    mSurfingGuidance = new SurfingGuidance();
    mSurfingGuidance->init(rIter);
}

bool SurfRayTutorial::update() {
    MR::getCorePadAcceleration(&mPadAccel, WPAD_CHAN0);
    updateNerve();

    return mIsTutorialPass;
}

void SurfRayTutorial::exeTutorialAllStart() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::sendMsgToGroupMember(ACTMES_TUTORIAL_START, mHost, MR::getSensor(mHost, 0), "body");
        MR::resetAndForwardNode(mTalkCtrl, 0);
    }

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        nextTutorial();
        mSurfingGuidance->activate();
    }
}

void SurfRayTutorial::exeTutorialStraightStart() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 1);
        mSurfingGuidance->levelOffReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);

    if (MR::isGreaterStep(this, sStepTalk)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialStraightFailure() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;
    }

    if (isSuccessStraight()) {
        if (mChangeStep++ == sStepToOK) {
            nextTutorial();
        }
    } else {
        mChangeStep = 0;
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialStraightUpkeep() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;

        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_OK", -1, -1);
        mSurfingGuidance->levelOffHold();
    }

    startTimerSound(getNerveStep(), _28);

    if (isSuccessStraight()) {
        mChangeStep = 0;

        if (MR::isStep(this, sStepToSuccess)) {
            MR::startSystemSE("SE_SY_SURF_TUTORIAL_GONEXT", _28, -1);

            _28++;

            nextTutorial();
            return;
        }
    } else if (mChangeStep++ == sStepToOK) {
        prevTutorial();
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_NG", -1, -1);
        mSurfingGuidance->levelOffReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialStraightSuccess() {
    if (MR::isFirstStep(this)) {
        mSurfingGuidance->levelOffSuccess();
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 2);
    }

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialTurnLeftStart() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 3);
        mSurfingGuidance->turnLeftReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);

    if (MR::isGreaterStep(this, sStepTalk)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialTurnLeftFailure() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;
    }

    if (isSuccessTurnLeft()) {
        if (mChangeStep++ == sStepToOK) {
            nextTutorial();
        }
    } else {
        mChangeStep = 0;
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialTurnLeftUpkeep() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;

        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_OK", -1, -1);
        mSurfingGuidance->turnLeftHold();
    }

    startTimerSound(getNerveStep(), _28);

    if (isSuccessTurnLeft()) {
        mChangeStep = 0;

        if (MR::isStep(this, sStepToSuccess)) {
            MR::startSystemSE("SE_SY_SURF_TUTORIAL_GONEXT", _28, -1);

            _28++;

            nextTutorial();
            return;
        }
    } else if (mChangeStep++ == sStepToOK) {
        prevTutorial();
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_NG", -1, -1);
        mSurfingGuidance->turnLeftReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialTurnLeftSuccess() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 4);
        mSurfingGuidance->turnLeftSuccess();
    }

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialTurnRightStart() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 5);
        mSurfingGuidance->turnRightReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);

    if (MR::isGreaterStep(this, sStepTalk)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialTurnRightFailure() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;
    }

    if (isSuccessTurnRight()) {
        if (mChangeStep++ == sStepToOK) {
            nextTutorial();
        }
    } else {
        mChangeStep = 0;
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialTurnRightUpkeep() {
    if (MR::isFirstStep(this)) {
        mChangeStep = 0;

        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_OK", -1, -1);
        mSurfingGuidance->turnRightHold();
    }

    startTimerSound(getNerveStep(), _28);

    if (isSuccessTurnRight()) {
        mChangeStep = 0;

        if (MR::isStep(this, sStepToSuccess)) {
            MR::startSystemSE("SE_SY_SURF_TUTORIAL_GONEXT2", _28, -1);

            _28++;

            nextTutorial();
            return;
        }
    } else if (mChangeStep++ == sStepToOK) {
        prevTutorial();
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_NG", -1, -1);
        mSurfingGuidance->turnRightReady();
    }

    MR::tryTalkForceWithoutDemoMarioPuppetable(mTalkCtrl);
}

void SurfRayTutorial::exeTutorialTurnRightSuccess() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 6);
        mSurfingGuidance->turnRightSuccess();
    }

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        nextTutorial();
    }
}

void SurfRayTutorial::exeTutorialAllEnd() {
    if (MR::isFirstStep(this)) {
        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::resetAndForwardNode(mTalkCtrl, 7);
        mSurfingGuidance->deactivate();
    }

    if (MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mTalkCtrl)) {
        MR::sendMsgToGroupMember(ACTMES_TUTORIAL_PASS, mHost, MR::getSensor(mHost, 0), "body");

        mIsTutorialPass = true;
    }
}

bool SurfRayTutorial::isSuccessStraight() const {
    if (__fabsf(mPadAccel.x) < sPadAccelStraightMinX && __fabsf(mPadAccel.y) < sPadAccelStraightMinY) {
        return true;
    }

    return false;
}

bool SurfRayTutorial::isSuccessTurnLeft() const {
    if (!isFailureTwistLeftMore() && !isFailureStand()) {
        return true;
    }

    return false;
}

bool SurfRayTutorial::isSuccessTurnRight() const {
    if (!isFailureTwistRightMore() && !isFailureStand()) {
        return true;
    }

    return false;
}

bool SurfRayTutorial::isFailureTwistLeftMore() const {
    return mPadAccel.x > -0.65f;
}

bool SurfRayTutorial::isFailureTwistRightMore() const {
    return mPadAccel.x < 0.65f;
}

bool SurfRayTutorial::isFailureStand() const {
    return mPadAccel.y < -0.5f;
}

void SurfRayTutorial::omitTutorial() const {
    MR::sendMsgToGroupMember(ACTMES_TUTORIAL_OMIT, mHost, MR::getSensor(mHost, 0), "body");
}

void SurfRayTutorial::nextTutorial() {
    if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialAllStart::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightStart::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightStart::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightFailure::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightFailure::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightUpkeep::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightSuccess::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightSuccess::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftStart::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftStart::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftFailure::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftFailure::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftUpkeep::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftSuccess::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftSuccess::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightStart::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightStart::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightFailure::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightFailure::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightUpkeep::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightSuccess::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightSuccess::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialAllEnd::sInstance);
    }

    MR::sendMsgToGroupMember(ACTMES_TUTORIAL_NEXT, mHost, MR::getSensor(mHost, 0), "body");
}

void SurfRayTutorial::prevTutorial() {
    if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialStraightFailure::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnLeftFailure::sInstance);
    } else if (isNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightUpkeep::sInstance)) {
        setNerve(&NrvSurfRayTutorial::SurfRayTutorialNrvTutorialTurnRightFailure::sInstance);
    }

    MR::sendMsgToGroupMember(ACTMES_TUTORIAL_PREV, mHost, MR::getSensor(mHost, 0), "body");
}

void SurfRayTutorial::startTimerSound(s32 step, s32 param2) {
    if (step == 20) {
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_TIMER", param2, -1);
    }

    if (step == 43) {
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_TIMER2", param2, -1);
    }

    if (step == 66) {
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_TIMER3", param2, -1);
    }
}
