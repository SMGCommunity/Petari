#include "Game/MapObj/ChooChooTrain.hpp"
#include "Game/Util.hpp"
#include "Game/Util/Array.hpp"

ChooChooTrain::ChooChooTrain(const char *pName) : LiveActor(pName), mModelArray() {
    _98.x = 0.0f;
    _98.y = 0.0f;
    _98.z = 0.0f;
    mRailCoord = 5.0f;
    mWhistleTimer = 0;
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

void ChooChooTrain::init(const JMapInfoIter &rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::isConnectedWithRail(rIter);
    }

    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    initModelManagerWithAnm("ChooChooTrain", 0, false);
    MR::connectToSceneCollisionMapObj(this);
    initEffectKeeper(0, 0, false);
    initSound(4, false);
    mWhistleTimer = MR::getRandom((s32)0xB4, (s32)0x258);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 80.0f * mScale.y);
    initHitSensor(1);
    MR::addMessageSensorMapObjMoveCollision(this, "body");
    MR::initCollisionParts(this, "ChooChooTrain", getSensor("body"), 0);
    MR::onCalcGravity(this);
    s32 temp_var = -1;
    s32 defTrainParts = 3;
    MR::getJMapInfoArg0NoInit(rIter, &temp_var);

    if (temp_var > 0) {
        defTrainParts = temp_var;
    }

    temp_var = -1;
    MR::getJMapInfoArg1NoInit(rIter, &temp_var);

    if (temp_var > 0) {
        mRailCoord = temp_var;
    }

    mModelArray.init(defTrainParts);

    int i;

    for (i = 0; i < defTrainParts; i++) {
        ModelObj* pObj = new ModelObj("汽車ポッポ客車", "ChooChooTrainBody", 0, -2, 0x1E, 2, false);
        pObj->initWithoutIter();
        MR::initCollisionParts(pObj, "ChooChooTrainBody", getSensor("body"), 0);
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
        MR::startBck(mModelArray[i], "Run", 0);
    }

    MR::reverseRailDirection(this);
    MR::setRailCoord(this, railCoord);
    MR::initAndSetRailClipping(&_98, this, 100.0f, 500.0f);
    MR::startBck(this, "Run", 0);
}

void ChooChooTrain::control() {
    MR::moveCoord(this, mRailCoord);
    MR::moveTransToCurrentRailPos(this);
    TVec3f stack_74(MR::getRailDirection(this));
    MR::turnDirectionAndGravityH(this, stack_74, 0.5f, 1.0f);
    f32 railCoord = MR::getRailCoord(this);
    TVec3f stack_68;
    MR::calcFrontVec(&stack_68, this);
    TVec3f stack_5C;
    stack_5C = mPosition - ((stack_68 * 532.0f) * mScale.y);
    MR::reverseRailDirection(this);

    TVec3f* vec;

    for (int i = 0; i < mModelArray.size(); i++) {
        MR::moveCoord(this, 1080.0f * mScale.y);
        MR::moveTransToOtherActorRailPos(mModelArray[i], this);
        stack_74 = (stack_5C - mModelArray[i]->mPosition);
        MR::turnDirectionAndGravityH(mModelArray[i], stack_74, 0.5f, 1.0f);
        MR::calcFrontVec(&stack_68, mModelArray[i]);

        vec = &mModelArray[i]->mPosition;
        stack_5C = (*vec - ((stack_68 * 532.0f) * mScale.y));
    }

    MR::reverseRailDirection(this);
    MR::setRailCoord(this, railCoord);
    MR::startLevelSound(this, "SE_OJ_LV_TOY_SL_MOVE", -1, -1, -1);

    if (mWhistleTimer < 0) {
        MR::startSound(this, "SE_OJ_TOY_SL_WHISTLE", -1, -1);
        mWhistleTimer = MR::getRandom((s32)0xB4, (s32)0x258);
    }
    else {
        mWhistleTimer--;
    }
}