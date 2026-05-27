#include "Game/Map/RaceRail.hpp"

namespace {
    static const f32 sRaceJudgeLength = 10000.0f;
    // static const f32 sWaterSurface = ;
}  // namespace

RaceRail::RaceRail(const char* pName) : NameObj(pName) {
}

PlayerRacer::PlayerRacer(const char* pName) : LiveActor(pName), _B0(0), _B4(0), mCameraInfo(nullptr) {
}

void PlayerRacer::appear() {
    RaceManagerFunction::startRaceImmediately();
}

void PlayerRacer::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    initRailRider(rIter);
    mCameraInfo = MR::createActorCameraInfo(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }

    MR::invalidateClipping(this);
}

void PlayerRacer::startRacer() {
    MR::endActorCamera(this, mCameraInfo, true, -1);
}

bool PlayerRacer::updateRacer(const RaceManager* pRaceManager) {
    PlayerRacer* racer = pRaceManager->mPlayerRacer;
    f32 railCoord = MR::calcNearestRailCoord(racer, *MR::getPlayerCenterPos());
    f32 railLength = MR::getRailTotalLength(racer);

    mPrevPosition.set(mCurrPosition);
    mCurrPosition.set(*MR::getPlayerCenterPos());

    TVec3f railPos;
    MR::calcNearestRailPos(&railPos, racer, *MR::getPlayerCenterPos());

    if (railPos.subOtherInline(*MR::getPlayerCenterPos()).length() > sRaceJudgeLength) {
        return false;
    }
    if ((f32)__fabs(mRailCoord - railCoord) < sRaceJudgeLength) {
        mRailCoord = railCoord;
    }

    s32 a1, a2;
    MR::getCurrentRailPointArg0WithInit(this, &a1);
    MR::setRailCoord(this, mRailCoord);
    MR::getCurrentRailPointArg0WithInit(this, &a2);

    if (a1 != -1 && a1 == a2) {
        mRailCoord = railCoord;
    }

    if ((f32)__fabs(mRailCoord - railLength) < 100.0f) {
        if (pRaceManager->isGoal(this)) {
            MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
            return true;
        }
        return false;
    }

    return false;
}

bool PlayerRacer::goalRacer() {
    return true;
}

void PlayerRacer::loseRacer() {
}

void PlayerRacer::exitRacer() {
}

PlayerRacer::~PlayerRacer() {
}

RaceRail::~RaceRail() {
}

void RaceRail::init(const JMapInfoIter& rIter) {
    mPlayerRacer = new PlayerRacer("レース判定用レール");
    mPlayerRacer->init(rIter);
    RaceManagerFunction::entryRacerPlayer(mPlayerRacer);
}

void PlayerRacer::initRacer() {
    AbstractRacer::initRacer();
    MR::startBckPlayer("Watch", (char*)nullptr);
}

void PlayerRacer::prepRacer(const RaceManager* pRaceManager) {
    MR::moveCoordToStartPos(this);
    MR::moveTransToCurrentRailPos(this);

    TVec3f grav;
    MR::calcGravityVector(this, mPosition, &grav, nullptr, 0);

    TPos3f mtx;
    TVec3f pos;
    switch (pRaceManager->mCurrentRace) {
    case 0: {  // RaceName_Penguin
        TVec3f s = mPosition.subOtherInline(grav * 500.f);
        TVec3f pos(mPosition);
        MR::getFirstPolyOnLineToWaterSurface(&pos, nullptr, s, grav * 1000.0f);
        pos -= grav * 15.0f;
        MR::makeMtxFrontUpPos(&mtx, MR::getRailDirection(this), -grav, pos);
        MR::startBckPlayer("SwimDrift", 1);
        break;
    }
    case 1: {  // RaceName_TeresaPhantom
        pos = mPosition;
        MR::convertPosOnGround(&pos, grav * 1000.0f);
        MR::makeMtxFrontUpPos(&mtx, MR::getRailDirection(this), -grav, pos);
        MR::startBckPlayer("BattleWait", 1);
        break;
    }
    case 2: {  // RaceName_TeresaDeathPromenade
        pos = mPosition;
        MR::makeMtxFrontUpPos(&mtx, MR::getRailDirection(this), -grav, pos);
        MR::startBckPlayer("Wait", 1);
        MR::onFollowDemoEffect();
        break;
    }
    }

    MR::tryPlayerKillTakingActor();
    MR::setPlayerBaseMtx(mtx);
    MR::startActorCameraTargetPlayer(this, mCameraInfo, -1);
}

void PlayerRacer::resetRacer(const RaceManager* pRaceManager) {
    MR::tryPlayerKillTakingActor();
    MR::setPlayerPosOnGround("レース終了後位置");
    switch (pRaceManager->mCurrentRace) {
    case 0:  // RaceName_Penguin
    case 1:  // RaceName_TeresaPhantom
    case 3:  // RaceName_SurfingTrial
    case 4:  // RaceName_SurfingChallenge
        MR::startBckPlayer("Watch", 0L);
        break;
    case 2:  // RaceName_TeresaDeathPromenade
        MR::onFollowDemoEffect();
        if (pRaceManager->mRank != 1) {
            MR::startBckPlayer("Sad", 0L);
        } else {
            MR::startBckPlayer("Wait", 0L);
        }
        break;
    }

    MR::progressPlayerBckOnPause();
    MR::resetPlayerStatus();
    MR::resetPlayerEffect();
}
