#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class JMapInfoIter;
class LiveActor;
class SurfingGuidance;
class TalkMessageCtrl;

class SurfRayTutorial : public NerveExecutor {
public:
    SurfRayTutorial(LiveActor*, TalkMessageCtrl*, const JMapInfoIter&);

    bool update();
    void exeTutorialAllStart();
    void exeTutorialStraightStart();
    void exeTutorialStraightFailure();
    void exeTutorialStraightUpkeep();
    void exeTutorialStraightSuccess();
    void exeTutorialTurnLeftStart();
    void exeTutorialTurnLeftFailure();
    void exeTutorialTurnLeftUpkeep();
    void exeTutorialTurnLeftSuccess();
    void exeTutorialTurnRightStart();
    void exeTutorialTurnRightFailure();
    void exeTutorialTurnRightUpkeep();
    void exeTutorialTurnRightSuccess();
    void exeTutorialAllEnd();
    bool isSuccessStraight() const;
    bool isSuccessTurnLeft() const;
    bool isSuccessTurnRight() const;
    bool isFailureTwistLeftMore() const NO_INLINE;
    bool isFailureTwistRightMore() const NO_INLINE;
    bool isFailureStand() const NO_INLINE;
    void omitTutorial() const;
    void nextTutorial();
    void prevTutorial();
    void startTimerSound(s32, s32);

    /* 0x08 */ LiveActor* mHost;
    /* 0x0C */ bool mIsTutorialPass;
    /* 0x10 */ TVec3f mPadAccel;
    /* 0x1C */ TalkMessageCtrl* mTalkCtrl;
    /* 0x20 */ SurfingGuidance* mSurfingGuidance;
    /* 0x24 */ s32 mChangeStep;
    /* 0x28 */ s32 _28;
};
