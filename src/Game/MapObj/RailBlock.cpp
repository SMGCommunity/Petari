#include "Game/MapObj/RailBlock.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvRailBlock {
    NEW_NERVE(RailBlockNrvWait, RailBlock, Wait);
    NEW_NERVE(RailBlockNrvMove, RailBlock, Move);
    NEW_NERVE(RailBlockNrvTerminate, RailBlock, Terminate);
};  // namespace NrvRailBlock

RailBlock::RailBlock(const char* pName) : LiveActor(pName) {
    _8C = 0xA;
    _90 = 0x64;
    _94 = 0;
    _95 = 0;
}

void RailBlock::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objName = nullptr;
    MR::getObjectName(&objName, rIter);
    initModelManagerWithAnm(objName, nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensorMapObjPress(this, "body", 16, 0.0f, offs);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    MR::setClippingTypeSphere(this, MR::getCollisionBoundingSphereRange(this));
    initSound(4, false);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToNearestPos(this, mPosition);
        _94 = 1;
    } else {
        _94 = 0;
    }

    MR::setGroupClipping(this, rIter, 16);
    MR::getJMapInfoArg0NoInit(rIter, &_8C);
    MR::getJMapInfoArg1NoInit(rIter, &_90);
    initNerve(&NrvRailBlock::RailBlockNrvMove::sInstance);

    if (!strcmp(objName, "RailBlockBig")) {
        _95 = 1;
    } else if (!strcmp(objName, "TsukidashkunBig")) {
        _95 = 1;
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnOffB(this, MR::Functor< RailBlock >(this, &RailBlock::stopMove),
                                    MR::Functor< RailBlock >(this, &RailBlock::startMove));
        setNerve(&NrvRailBlock::RailBlockNrvWait::sInstance);
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void RailBlock::calcViewAndEntry() {
    LiveActor::calcViewAndEntry();
}

void RailBlock::startMove() {
    setNerve(&NrvRailBlock::RailBlockNrvMove::sInstance);
}

void RailBlock::stopMove() {
    setNerve(&NrvRailBlock::RailBlockNrvWait::sInstance);
}

void RailBlock::exeMove() {
    if (_94) {
        MR::moveCoordAndFollowTrans(this, _8C);
        MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_MOVE", -1, -1, -1);
        if (MR::isRailReachedGoal(this)) {
            MR::startLevelSound(this, "SE_OJ_TSUKIDASHI_STOP", -1, -1, -1);
            setNerve(&NrvRailBlock::RailBlockNrvTerminate::sInstance);
        }
    }
}

void RailBlock::exeTerminate() {
    if (getNerveStep() >= _90 - 60) {
        s32 step = getNerveStep() - _90 + 60;
        f32 v4 = step % 3;
        f32 v3;
        f32 val = 30.0f;

        if (((step / 3) & 1 ^ ((step / 3) >> 31)) == (step / 3) >> 31) {
            v3 = val * (v4 / 3.0f);
        } else {
            v3 = val * ((1.0f - v4) / 3.0f);
        }

        if (MR::isRailGoingToEnd(this)) {
            v3 = MR::getRailTotalLength(this) - v3;
        }

        MR::calcRailPosAtCoord(&mPosition, this, v3);
        MR::startLevelSound(this, "SE_OJ_LV_TSUKIDASHI_VIB", -1, -1, -1);
    }

    if (getNerveStep() >= _90) {
        MR::reverseRailDirection(this);
        MR::startSound(this, "SE_OJ_TSUKIDASHI_START", -1, -1);
        setNerve(&NrvRailBlock::RailBlockNrvMove::sInstance);
    }
}

void RailBlock::exeWait() {}

void RailBlock::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
}

RailBlock::~RailBlock() {}
