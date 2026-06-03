#include "Game/MapObj/SmallStone.hpp"

SmallStone::SmallStone(const char* pName) : LiveActor(pName), _8C(0), _90(0), _94(0.0f), mMemberType(0), _9C(true){};

void SmallStone::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    _90 = 8;
    s32 v1 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &v1);
    if(v1 == -1) {
        _94 = 0.0f;
    } else {
        _94 = float(v1);
    }
    v1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &v1);
    if(v1 == -1) {
        _9C = true;
    } else {
        mRotation.x *= 0.017453292f;
        mRotation.y *= 0.017453292f;
        mRotation.z *= 0.017453292f;
        _9C = false;
        TQuat4f v2;
        v2.setEuler(mRotation.x, mRotation.y, mRotation.z);
        v2.getYDir(mGravity);
        mGravity.scale(-1);
    }
    v1 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &v1);
    if(v1 == -1) {
        _9D = false;
    } else {
        _9D = true;
    }
    const char* vName = nullptr;
    MR::getObjectName(&vName, rIter);
    if(MR::isEqualString(vName, "CircleShell")) {
        mMemberType = 1;
        initMember("CircleShell", _9C);
    } else if (MR::isEqualString(vName, "CircleStrawberry")) {
        mMemberType = 2;
        initMember("CircleStrawberry", _9C);
    } else {
        mMemberType = 0;
        initMember("SmallStone", _9C);
    }
    initSound(4, false);
    appear();

    
}

// void SmallStone::control() {
//     if(_90 < 0) {

//     }
// }


SmallStoneMember::SmallStoneMember(const char* pName)
    : ModelObj("SmallStoneMember", pName, nullptr, 10, -1, -2, false), _90(1.0f), _94(false), _95(false), _98(-1), _9C(0), _A0(true) {}

void SmallStoneMember::movementByHost(SmallStone* pParent) {
    animControl();
    if(_98 > -1) {
        _98--;
        if(_98 == -1) {
            doBreak(pParent);
        }
    }
    if(MR::isValidCalcAnim(this)) {
        MR::updateModelAnimPlayer(this);
    }
}

void SmallStoneMember::animControl() {
    if(_94 == true) {
        if(!MR::isBckPlaying(this, "WindLoop")) {
            _95 = false;
            MR::onCalcAnim(this);
            MR::startBck(this, "WindLoop", nullptr);
        }
        _94 = false;
    } else {
        if(MR::isBckPlaying(this, "WindLoop")) {
            _95 = false;
            MR::onCalcAnim(this);
            MR::startBck(this, "WindEnd", nullptr);
            MR::setBckRate(this, _90);
        } else if (!_95 && MR::isBckStopped(this)) {
            _95 = true;
            MR::offCalcAnim(this);
        }
    }
}
