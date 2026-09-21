#include "Game/MapObj/ChooChooTrain.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace {
    // static const f32 hNextCoord = _;
    // static const f32 hConnectPosFront = _;
    // static const f32 hTurnToConnectAngle = _;
    static const s32 hWhistleStepMin = 180;
    static const s32 hWhistleStepMax = 600;
};

ChooChooTrain::ChooChooTrain(const char* pName) : LiveActor(pName), mModelArray(), _98(0.0f, 0.0f, 0.0f), mRailSpeed(5.0f), mWhistleTimer() {
}

void ChooChooTrain::startClipped() {
    LiveActor::startClipped();

    for (int i = 0; i < mModelArray.size(); i++) {
        mModelArray[i]->startClipped();
    }
}

void ChooChooTrain::endClipped() {
    LiveActor::endClipped();

    for (int i = 0; i < mModelArray.size(); i++) {
        mModelArray[i]->endClipped();
    }
}

void ChooChooTrain::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        if (MR::isConnectedWithRail(rIter)) {
        }
    }

    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    initModelManagerWithAnm("ChooChooTrain", 0, false);
    MR::connectToSceneCollisionMapObj(this);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    mWhistleTimer = MR::getRandom(::hWhistleStepMin, ::hWhistleStepMax);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 80.0f * mScale.y);
    initHitSensor(1);
    MR::addMessageSensorMapObjMoveCollision(this, "body");
    MR::initCollisionParts(this, "ChooChooTrain", getSensor("body"), 0);
    MR::onCalcGravity(this);
    s32 arg = -1;
    s32 defTrainParts = 3;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (arg > 0) {
        defTrainParts = arg;
    }

    arg = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg);

    if (arg > 0) {
        mRailSpeed = arg;
    }

    mModelArray.init(defTrainParts);

    int i;

    for (i = 0; i < defTrainParts; i++) {
        ModelObj* pObj =
            new ModelObj("汽車ポッポ客車", "ChooChooTrainBody", nullptr, -2, MR::MovementType_CollisionMapObj, MR::CalcAnimType_CollisionMapObj, false);
        pObj->initWithoutIter();
        MR::initCollisionParts(pObj, "ChooChooTrainBody", getSensor("body"), nullptr);
        MR::invalidateClipping(pObj);
        mModelArray.push_back(pObj);
    }

    makeActorAppeared();
    MR::moveCoordToNearestPos(this, mPosition);
    MR::moveTransToCurrentRailPos(this);
    f32 railCoord = MR::getRailCoord(this);
    MR::reverseRailDirection(this);

    for (i = 0; i < defTrainParts; i++) {
        MR::moveCoord(this, 1080.0f * mScale.y);
        MR::moveTransToOtherActorRailPos(mModelArray[i], this);
        MR::onCalcGravity(mModelArray[i]);
        mModelArray[i]->makeActorAppeared();
        MR::startBck(mModelArray[i], "Run");
    }

    MR::reverseRailDirection(this);
    MR::setRailCoord(this, railCoord);
    MR::initAndSetRailClipping(&_98, this, 100.0f, 500.0f);
    MR::startBck(this, "Run");
}

void ChooChooTrain::control() {
    MR::moveCoord(this, mRailSpeed);
    MR::moveTransToCurrentRailPos(this);
    TVec3f stack_74(MR::getRailDirection(this));
    MR::turnDirectionAndGravityH(this, stack_74, 0.5f, 1.0f);
    f32 railCoord = MR::getRailCoord(this);
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    TVec3f stack_5C;
    stack_5C = mPosition - ((frontVec * 532.0f) * mScale.y);
    MR::reverseRailDirection(this);

    TVec3f* vec;

    for (int i = 0; i < mModelArray.size(); i++) {
        MR::moveCoord(this, 1080.0f * mScale.y);
        MR::moveTransToOtherActorRailPos(mModelArray[i], this);
        stack_74 = (stack_5C - mModelArray[i]->mPosition);
        MR::turnDirectionAndGravityH(mModelArray[i], stack_74, 0.5f, 1.0f);
        MR::calcFrontVec(&frontVec, mModelArray[i]);

        vec = &mModelArray[i]->mPosition;
        stack_5C = (*vec - ((frontVec * 532.0f) * mScale.y));
    }

    MR::reverseRailDirection(this);
    MR::setRailCoord(this, railCoord);
    MR::startLevelSound(this, "SE_OJ_LV_TOY_SL_MOVE");

    if (mWhistleTimer <= 0) {
        MR::startSound(this, "SE_OJ_TOY_SL_WHISTLE");
        mWhistleTimer = MR::getRandom(::hWhistleStepMin, ::hWhistleStepMax);
    } else {
        mWhistleTimer--;
    }
}
