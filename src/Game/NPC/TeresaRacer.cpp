#include "Game/NPC/TeresaRacer.hpp"
#include "Game/Map/RaceRail.hpp"
#include "Game/MapObj/GCapture.hpp"

namespace {
    static const f32 sSpeed = 20.0f;
    static const f32 sAccele = 0.1f;
    static const f32 sCameraDirScale = 0.4f;
    static const s32 sBgmChangeStep = 480;
    static const s32 sNearBgm_Fadetime = 90;
    static const s32 sFarBgm_Fadetime = 150;
    static const f32 sNearBgmDist = 3000.0f;
    static const f32 sNearGoalDist = 500.0f;
    static const f32 sRasterParamMax = 2500.0f;
    static const s32 sRasterParamX = 300;
    static const s32 sRasterParamTime = 16;
    static const s32 sRasterParamY = 1000;
}  // namespace

namespace NrvTeresaRacer {
    NEW_NERVE(TeresaRacerNrvWait, TeresaRacer, Wait);
    NEW_NERVE(TeresaRacerNrvPre, TeresaRacer, Pre);
    NEW_NERVE(TeresaRacerNrvReady, TeresaRacer, Ready);
    NEW_NERVE(TeresaRacerNrvMove, TeresaRacer, Move);
    NEW_NERVE(TeresaRacerNrvGoal, TeresaRacer, Goal);
    NEW_NERVE(TeresaRacerNrvPost, TeresaRacer, Post);
    NEW_NERVE(TeresaRacerNrvTalk, TeresaRacer, Talk);
    NEW_NERVE(TeresaRacerNrvTakeOutStar, TeresaRacer, TakeOutStar);
}  // namespace NrvTeresaRacer

TeresaRacer::TeresaRacer(const char* pName) : NPCActor(pName), mTakeOutStar(nullptr), mCameraInfo(nullptr), mIsGoal(false), mBgmState(0) {
}

void TeresaRacer::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("TeresaRacer");
    caps.setDefault();
    caps.setIndirect();
    caps.mLodCtrl = false;
    caps._5D = true;
    caps.mSensorSize = 220.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = 25.0f;
    caps.mSensorOffset.z = 40.0f;
    caps.mWaitNerve = &NrvTeresaRacer::TeresaRacerNrvWait::sInstance;

    initialize(rIter, caps);

    MR::getJMapInfoArg0WithInit(rIter, &mRacerId);
    if (mRacerId == 0 && MR::isPlayerLuigi()) {
        makeActorDead();
    } else if (mRacerId == 1 && !MR::isPlayerLuigi()) {
        makeActorDead();
    } else {
        MR::declarePowerStar(this);

        MR::registerBranchFunc(getMsgCtrl(), TalkMessageFunc(this, &TeresaRacer::branchFunc));
        MR::registerAnimeFunc(getMsgCtrl(), TalkMessageFunc(this, &TeresaRacer::animeFunc));
        MR::registerEventFunc(getMsgCtrl(), TalkMessageFunc(this, &TeresaRacer::eventFunc));

        MR::setDistanceToTalk(getMsgCtrl(), 500.0f);
        RaceManagerFunction::entryRacerOthers(this);
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "会話");  // conversation

        mParam.setMoveAction("WaitOpen", "WaitOpen");
        mParam.setTalkAction("WaitOpenTalk", "WaitOpenTalk");
        setTalkAction("Chase");
        mParam._8 = 1.5f;
        _10C = sSpeed;
        _110 = sAccele;
        _114 = 0.05f;
        mParam._4 = 3000.0f;

        mTakeOutStar =
            new TakeOutStar(this, "TakeOutStarTeresaRacer", "TakeOutStarTeresaRacer", &NrvTeresaRacer::TeresaRacerNrvTakeOutStar::sInstance);
    }
}

void TeresaRacer::control() {
    if (_D8) {
        MR::startSound(this, "SE_SM_TERERACER_TRAMPLE", -1, -1);
    }
    NPCActor::control();
}

bool TeresaRacer::branchFunc(u32 state) {
    return RaceManagerFunction::getRaceRank() == 1;
}

bool TeresaRacer::animeFunc(u32 anime) {
    if (!isNerve(&NrvTeresaRacer::TeresaRacerNrvReady::sInstance)) {
        pushNerve(&NrvTeresaRacer::TeresaRacerNrvReady::sInstance);
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        popNerve();
        return true;
    }

    switch (anime) {
    case 0:
        MR::tryStartAction(this, "Close");

        mParam.setTalkAction("CloseTalk", "CloseTalk");
        mParam.setMoveAction("CloseTalk", "CloseTalk");
        break;
    case 1:
        mParam.setMoveAction("Defeat", "Defeat");
        mParam.setTalkAction("Defeat", "Defeat");
        popNerve();
        return true;
    case 2:
        MR::tryStartAction(this, "Open");
        mParam.setTalkAction("WaitOpenTalk", "WaitOpenTalk");
        mParam.setMoveAction("WaitOpen", "WaitOpen");
        break;
    case 3:
        MR::tryStartAction(this, "Open");
        mParam.setTalkAction("WaitOpenTalk", "WaitOpenTalk");
        mParam.setMoveAction("WaitOpen", "WaitOpen");
        break;
    default:
        break;
    }

    return false;
}

bool TeresaRacer::eventFunc(u32 state) {
    return mTakeOutStar->takeOut();
}

void TeresaRacer::exeWait() {
    if (MR::tryTalkNearPlayerAndStartTalkAction(this)) {
        if (mRacerId == -1) {
            MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", -1);
        }
        setNerve(&NrvTeresaRacer::TeresaRacerNrvPre::sInstance);
    }
}

void TeresaRacer::exePre() {
    if (MR::isFirstStep(this)) {
        MR::resetGCapture();
    }

    if (!MR::isActionContinuous(this) && MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        if (MR::tryTalkSelectLeft(getMsgCtrl())) {
            RaceManagerFunction::startRaceWithWipe();
            setNerve(&NrvTeresaRacer::TeresaRacerNrvReady::sInstance);
        } else {
            if (mRacerId == -1) {
                MR::endMultiActorCamera(this, mCameraInfo, "会話", false, -1);
            }
            setNerve(&NrvTeresaRacer::TeresaRacerNrvWait::sInstance);
        }
    }
}

void TeresaRacer::exeReady() {
}

void TeresaRacer::exePost() {
    if (!MR::isActionContinuous(this) && MR::tryTalkForceAtEndAndStartTalkAction(this)) {
        MR::endMultiActorCamera(this, mCameraInfo, "会話", true, -1);
        if (RaceManagerFunction::getRaceRank() == 1) {
            setNerve(&NrvTeresaRacer::TeresaRacerNrvTalk::sInstance);
        } else {
            MR::forceKillPlayerByGroundRace();
            setNerve(&NrvTeresaRacer::TeresaRacerNrvReady::sInstance);
        }
    }
}

void TeresaRacer::exeMove() {
    if (MR::isRailReachedNearGoal(this, sNearGoalDist) || !MR::isRailGoingToEnd(this)) {
        setNerve(&NrvTeresaRacer::TeresaRacerNrvGoal::sInstance);
    } else {
        MR::getCurrentRailPointArg0NoInit(this, &_10C);  // speed
        TVec3f up;
        MR::extractMtxYDir(getBaseMtx(), &up);
        MR::adjustmentRailCoordSpeed(this, _10C, _110);
        MR::moveRailRider(this);
        if (mRacerId == -1) {
            MR::decidePose(this, up, MR::getRailDirection(this), MR::getRailPos(this), 1.0f, 0.05f, 0.1f);
        } else {
            TVec3f dir = MR::getRailDirection(this) + MR::getCamZdir().invertOperatorInternal().multInLine(sCameraDirScale);

            if (!MR::normalizeOrZero(&dir)) {
                MR::decidePose(this, up, dir, MR::getRailPos(this), 1.0f, 0.05f, 0.1f);
            }
        }
    }

    if (mRacerId != -1 && MR::calcDistanceToPlayer(this) < sRasterParamMax) {
        MR::setRasterScroll(sRasterParamTime, sRasterParamX, sRasterParamY);
        MR::startAtmosphereLevelSE("SE_AT_LV_DEATH_PROM_RASTER", -1, -1);
    }
    changeBgmState();
}

void TeresaRacer::exeGoal() {
    if (MR::isFirstStep(this)) {
        mParam._0 = true;
        _10C = 0.0f;
        mParam._1 = true;
    }

    if (MR::getRailCoordSpeed(this) < 1.0f) {
        MR::tryStartMoveTurnAction(this);
    } else {
        MR::tryStartTurnAction(this);
    }

    if (mRacerId != -1 && MR::calcDistanceToPlayer(this) < sRasterParamMax) {
        MR::setRasterScroll(sRasterParamTime, sRasterParamX, sRasterParamY);
        MR::startAtmosphereLevelSE("SE_AT_LV_DEATH_PROM_RASTER", -1, -1);
    }
    changeBgmState();
}

void TeresaRacer::exeTalk() {
    MR::tryTalkNearPlayerAndStartTalkAction(this);
}

void TeresaRacer::exeTakeOutStar() {
}

void TeresaRacer::initRacer() {
    AbstractRacer::initRacer();
}

void TeresaRacer::prepRacer(const RaceManager* pRaceManager) {
    MR::invalidateClipping(this);
    MR::moveCoordToStartPos(this);
    MR::setRailCoordSpeed(this, 0.0f);
    MR::followRailPose(this, 1.0f, 1.0f);
    MR::requestMovementOn(this);
    MR::startAction(this, "Chase");
    mParam._0 = false;
    mParam._1 = false;
}

void TeresaRacer::startRacer() {
    mIsGoal = false;
    mBgmState = 0;
    setNerve(&NrvTeresaRacer::TeresaRacerNrvMove::sInstance);
}

bool TeresaRacer::updateRacer(const RaceManager* pRaceManager) {
    f32 coord = MR::calcNearestRailCoord(pRaceManager->mPlayerRacer, mPosition);
    f32 length = MR::getRailTotalLength(pRaceManager->mPlayerRacer);
    mPrevPosition.set(mCurrPosition);
    mCurrPosition.set(mPosition);
    if ((f32)__fabs(mRailCoord - coord) < 2000.0f) {
        mRailCoord = coord;
    }

    if ((f32)__fabs(mRailCoord - length) < 100.0f) {
        mIsGoal = pRaceManager->isGoal(this);
        return mIsGoal;
    }

    return false;
}

bool TeresaRacer::goalRacer() {
    return false;
}

void TeresaRacer::loseRacer() {
}

void TeresaRacer::resetRacer(const RaceManager* pRaceManager) {
    if (pRaceManager->mCurrentRace == 1) {
        MR::setDefaultPose(this);
    } else {
        MR::setNPCActorPos(this, "レース終了後位置テレサ");
        setInitPose();
    }

    turnToPlayer(180.0f);
    setNerve(&NrvTeresaRacer::TeresaRacerNrvPost::sInstance);
    MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", -1);
    mParam._0 = true;
    mParam._1 = true;
}

void TeresaRacer::exitRacer() {
    MR::validateClipping(this);
}

void TeresaRacer::changeBgmState() {
    if (!MR::isLessStep(this, sBgmChangeStep) && !mIsGoal) {
        if (MR::calcDistanceToPlayer(this) < sNearBgmDist) {
            if (mBgmState != 2) {
                MR::setStageBGMState(2, sNearBgm_Fadetime);
            }
            mBgmState = 2;
        } else {
            if (mBgmState != 1) {
                MR::setStageBGMState(1, sFarBgm_Fadetime);
            }
            mBgmState = 1;
        }
    }
}
