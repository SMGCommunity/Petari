#include "Game/Boss/OtaKing.hpp"
#include "Game/MapObj/CocoNut.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

namespace {
    // const _ cHP = _;
    // const _ cThrowDistance = _;
    // const _ cWaitFrame = _;
    // const _ cValidThrowCocoNutDistance = _;
    // const _ cThrowCocoNutStep = _;
    // const _ cCocoNutThrowNum = _;
    // const _ cThrowAngleMin = _;
    // const _ cThrowAngleMax = _;
    // const _ cThrowCocoNutWaitFrame = _;
    // const _ cThrowFireBallStep = _;
    // const _ cFireBallThrowNum = _;
    // const _ cFireBallThrowSpeed = _;
    // const _ cFireBallThrowAngle = _;
    // const _ cThrowFireBallWaitFrame = _;
    // const _ cBubbleAppearStep = _;
    // const _ cBubbleAppearNum = _;
    // const _ cBubbleAppearRandomAngle = _;
    // const _ cAppearDemoRippleStepS = _;
    // const _ cAppearDemoRippleStepL = _;
    // const _ cCocoNutRippleStep = _;
    // const _ cFireBallRippleStep = _;
    // const _ cDownRippleStep = _;
    // const _ cNearAttackRippleStep = _;
    // const _ cBubbleAttackRippleStep = _;
    // const _ cDownSinkStartStep = _;
    // const _ cBodySensorRadius = _;
    // const _ cEyeSensorRadius = _;
    // const _ cTurnSpeedMax = _;
    // const _ cTurnSpeedAccel = _;
    // const _ cPowerStarAppearOffsetY = _;
    // const _ cStarPieceAppearOffsetY = _;
    // const _ cStarPieceAppearNum = _;
    // const _ cStarPieceAppearRange = _;
    // const _ cStarPieceAppearVelocity = _;
    // const _ cDamageCrownFallFrame = _;
    // const _ cDamageAngryRumbleFrame = _;
    // const _ cDamageFrontFootClipingHeight = _;
    // const _ cDownDemoFrame = _;
    // const _ cDownDemoCocoNutScale = _;
    // const _ cDownDemoRumbleFrame1 = _;
    // const _ cDownDemoRumbleFrame2 = _;
    // const _ cPowerStarExistBgmStartStep = _;
    // const _ cAppearSeStep0 = _;
    // const _ cAppearSeStep1 = _;
    // const _ cAppearSeStep2 = _;
    // const _ cAppearSeStep3 = _;
    // const _ cAppearDemoRumblePad01 = _;
    // const _ cAppearDemoRumblePad02 = _;
    // const _ cAppearDemoRumblePad03 = _;
    // const _ cAppearDemoRumblePad04 = _;
    // const _ cAppearDemoRumblePad05 = _;
    // const _ cAppearDemoRumblePad06 = _;
    // const _ cBlurStartStep = _;
    // const _ cBlurTotalFrame = _;
};  // namespace

namespace NrvOtaKing {
    NEW_NERVE(OtaKingNrvWaitOnSwitch, OtaKing, WaitOnSwitch);
    NEW_NERVE(OtaKingNrvAppearDemo, OtaKing, AppearDemo);
    NEW_NERVE(OtaKingNrvWait, OtaKing, Wait);
    NEW_NERVE(OtaKingNrvThrowCocoNutWait, OtaKing, ThrowCocoNutWait);
    NEW_NERVE(OtaKingNrvThrowCocoNut, OtaKing, ThrowCocoNut);
    NEW_NERVE(OtaKingNrvThrowFireBallWait, OtaKing, ThrowFireBallWait);
    NEW_NERVE(OtaKingNrvThrowFireBall, OtaKing, ThrowFireBall);
    NEW_NERVE(OtaKingNrvDamage, OtaKing, Damage);
    NEW_NERVE(OtaKingNrvPowerUp, OtaKing, PowerUp);
    NEW_NERVE(OtaKingNrvDown, OtaKing, Down);
    NEW_NERVE(OtaKingNrvDownDemo, OtaKing, DownDemo);
    NEW_NERVE(OtaKingNrvAppearStar, OtaKing, AppearStar);
    NEW_NERVE(OtaKingNrvDead, OtaKing, Dead);
    NEW_NERVE(OtaKingNrvHitBackStart, OtaKing, HitBackStart);
    NEW_NERVE(OtaKingNrvHitBack, OtaKing, HitBack);
    NEW_NERVE(OtaKingNrvNearAttack, OtaKing, NearAttack);
    NEW_NERVE(OtaKingNrvBubbleAttack, OtaKing, BubbleAttack);
    NEW_NERVE(OtaKingNrvWaitStartDemo, OtaKing, WaitStartDemo);
};  // namespace NrvOtaKing

OtaKing::OtaKing(const char* pName)
    : LiveActor(pName), mMagma(), mCocoNutBallArray(), mFireBallArray(), mFireBubbleArray(), _E4(), _E8(), _EC(), _F0(), _F4(), _F8(gZeroVec), _104(),
      _108(), _10C(), mIsLv2(), _154() {
    MR::zeroMemory(_8C, sizeof(_8C));
    MR::zeroMemory(mLongFootArray, sizeof(mLongFootArray));
    _A8.identity();
    MR::zeroMemory(_110, sizeof(_110));
}

void OtaKing::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool arg1 = false;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    bool isLv2 = arg1;

    pArchiveList->addArchive(CocoNut::getModelName());
    pArchiveList->addArchive("FireBall");
    pArchiveList->addArchive("FireBubble");
    pArchiveList->addArchive("OtaKingMagma");
    pArchiveList->addArchive("OtaKingMagmaBloom");

    if (!isLv2) {
        pArchiveList->addArchive("OtaKing");
        pArchiveList->addArchive("OtaKingFoot");
        pArchiveList->addArchive("OtaKingLongFoot");
    } else {
        pArchiveList->addArchive("OtaKingLv2");
        pArchiveList->addArchive("OtaKingFootLv2");
    }
}
