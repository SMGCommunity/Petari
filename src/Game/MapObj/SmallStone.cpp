#include "Game/MapObj/SmallStone.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"

SmallStone::SmallStone(const char* pName) : LiveActor(pName), mArray(0), mArrayCount(0), _94(0.0f), mMemberType(0), _9C(true){};

void SmallStone::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    mArrayCount = 8;
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

void SmallStone::initMember(const char* pChar, bool pBool) {
    mArray = new SmallStoneMember*[mArrayCount];
    MR::declareStarPiece(this, mArrayCount);
    for(int i = 0; i < mArrayCount; i++) {
        SmallStoneMember* member = new SmallStoneMember(pChar);
        member->_90 = 1.0f;
        member->_94 = false;
        member->_A0 = false;
        member->_95 = false;
        MR::onCalcAnim(member);
        MR::invalidateClipping(member); 
        member->appear();
        if(mMemberType == 1) {
            //do something random with rotation, idk
            member->mRotation.x = 0.0f;
            member->mRotation.y = MR::getRandom(-360.0f, 360.0f);
            member->mRotation.z = 0.0f;
            f32 randScale = MR::getRandom(0.75f, 1.25f);
            member->mScale.x = randScale;
            member->mScale.y = randScale;
            member->mScale.z = randScale;
            MR::startBva(member, "Kind");
            MR::stopBva(member);
            s16 frameMax = MR::getBvaFrameMax(member, "Kind");
            u32 frame = (i + frameMax) % frameMax;
            MR::setBvaFrameAndStop(member, (float)frame);
            switch (frame) {
                case 0:
                    member->mBreakStr = "Break2";
                    break;
                case 1:
                    member->mBreakStr = "Break1";
                    break;
                case 2:
                    member->mBreakStr = "Break3";
                    break;
                case 3:
                    member->mBreakStr = "Break1";
                    break;
                case 4:
                    member->mBreakStr = "Break3";
                    break;
            }
        } else {
            mRotation.set(0.0f, 0.0f, 0.0f);
            mScale.set(1.0f, 1.0f, 1.0f);
            member->mBreakStr = "Break";
        }
        mArray[i] = member;
    }
    if(mMemberType == 1 || mMemberType == 2) {
        for(int i = 0; i < mArrayCount; i++) {
            SmallStoneMember* curr = mArray[i];
            SmallStoneMember* rand = mArray[(MR::getRandom(0l, i + 1))];
            f32 currBVA = MR::getBvaFrame(curr);
            f32 randBVA = MR::getBvaFrame(rand);
            MR::setBvaFrame(curr, randBVA);
            MR::setBvaFrame(rand, currBVA);
            const char* currBreakStr = curr->mBreakStr;
            const char* randBreakStr = rand->mBreakStr;
            curr->mBreakStr = randBreakStr;
            rand->mBreakStr = currBreakStr;
        }
    }
    initHitSensor(1);
    MR::addHitSensorMapObjSimple(this, "Range", 16, 100.0f, gZeroVec);
        
}


SmallStoneMember::SmallStoneMember(const char* pName)
    : ModelObj("SmallStoneMember", pName, nullptr, 10, -1, -2, false), _90(1.0f), _94(false), _95(false), _98(-1), mBreakStr(nullptr), _A0(true) {}

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
