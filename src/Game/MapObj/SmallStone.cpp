#include "Game/MapObj/SmallStone.hpp"

#define TAU 6.2831855f
#define TOLERANCE 0.0099999998f

SmallStone::SmallStone(const char* pName) : LiveActor(pName), mMembers(0), mMemberCount(0), mVecScale(0.0f), mMemberType(0), _9C(true){};

void SmallStone::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    mMemberCount = 8;
    s32 v1 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &v1);
    if (v1 == -1) {
        mVecScale = 0.0f;
    } else {
        mVecScale = float(v1);
    }
    v1 = -1;
    MR::getJMapInfoArg1NoInit(rIter, &v1);
    if (v1 == -1) {
        _9C = true;
    } else {
        _9C = false;
        mRotation.x = deg2rad(mRotation.x);
        mRotation.y = deg2rad(mRotation.y);
        mRotation.z = deg2rad(mRotation.z);
        TQuat4f v2;
        v2.setEuler(mRotation.x, mRotation.y, mRotation.z);
        v2.getYDir(mGravity);
        mGravity.scale(-1);
    }
    v1 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &v1);
    if (v1 == -1) {
        _9D = false;
    } else {
        _9D = true;
    }
    const char* vName = nullptr;
    MR::getObjectName(&vName, rIter);
    if (MR::isEqualString(vName, "CircleShell")) {
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

void SmallStone::initAfterPlacement() {
    TVec3f vecOut;
    if(_9C) {
        MR::calcGravityVector(this, mPosition, &vecOut, nullptr, 0);
    } else {
        vecOut.set(mGravity);
    }
    TVec3f vec1;
    TVec3f vec2;
    vec1.set(1, 0 ,0);
    vec2.set(0, 1, 0);
    MR::makeAxisCrossPlane(&vec1, &vec2, vecOut);
    getSensor("Range")->mRadius = 0.0f;
    TVec3f vec3;
    TVec3f vec4;
    vec3.set(0, 0, 0);
    vec4.set(0, 0, 0);
    f32 angleStep = TAU / (float)mMemberCount; 
    bool anyHit = false;
    for(int i = 0; i < mMemberCount; i++) {
        TVec3f vec5 = vec1;
        f32 angle = i * angleStep;
        vec5.scale(MR::cos(angle));
        TVec3f vec6 = vec2;
        vec6.scale(MR::sin(angle));
        vec5.add(vec6);
        vec5.scale(300.0f);
        TVec3f vec7 = mPosition;
        vec7.add(vec5);
        vec7.sub(vecOut);
        TVec3f vec8 = vecOut;
        vec8.scale(1000);
        //any hit logic, low confidence here
        anyHit = anyHit || MR::getFirstPolyOnLineToMap(&mMembers[i]->mPosition, nullptr, vec7, vec8);
        if(_9C) {
            MR::calcGravityVector(mMembers[i], &mMembers[i]->mGravity, nullptr, 0);
        } else {
            mMembers[i]->mGravity.set(vecOut);
        }
        TVec3f vec9 = -mMembers[i]->mGravity;
        TVec3f vec10 = vec9;
        vec10.scale(mVecScale);
        vec10.add(mMembers[i]->mPosition);
        TVec3f vec11;
        vec11.set(1, 0, 0);
        if(MR::isSameDirection(mMembers[i]->mGravity, vec11, TOLERANCE)) {
            vec11.set(0, 1, 0);
        }
        TPos3f pos1;
        MR::calcMtxFromGravityAndZAxis(&pos1, mMembers[i], mMembers[i]->mGravity, vec11);
        if(mMemberType == 1 || mMemberType == 2) {
            TMtx34f mtx;
            MR::makeMtxRotate(mtx, mMembers[i]->mRotation);
            pos1.concat(mtx);
        }
    }

}

void SmallStone::initMember(const char* pChar, bool pBool) {
    mMembers = new SmallStoneMember*[mMemberCount];
    MR::declareStarPiece(this, mMemberCount);
    for (int i = 0; i < mMemberCount; i++) {
        SmallStoneMember* member = new SmallStoneMember(pChar);
        member->_90 = 1.0f;
        member->_94 = false;
        member->_A0 = false;
        member->_95 = false;
        MR::onCalcAnim(member);
        MR::invalidateClipping(member);
        member->appear();
        if (mMemberType == 1) {
            // do something random with rotation, idk
            // member->mRotation.x = 0.0f;
            // member->mRotation.y = MR::getRandom(-360.0f, 360.0f);
            // member->mRotation.z = 0.0f;
            member->mRotation.set(0.f, MR::getRandom(-365.f, 365.f), 0.f);
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
        mMembers[i] = member;
    }
    if (mMemberType == 1 || mMemberType == 2) {
        for (int i = 0; i < mMemberCount; i++) {
            SmallStoneMember* curr = mMembers[i];
            SmallStoneMember* rand = mMembers[(MR::getRandom(0l, i + 1))];
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
    : ModelObj("SmallStoneMember", pName, nullptr, 10, -1, -2, false), _90(1.0f), _94(false), _95(false), _98(-1), mBreakStr(nullptr), _A0(true) {
}

void SmallStoneMember::movementByHost(SmallStone* pParent) {
    animControl();
    if (_98 > -1) {
        _98--;
        if (_98 == -1) {
            doBreak(pParent);
        }
    }
    if (MR::isValidCalcAnim(this)) {
        MR::updateModelAnimPlayer(this);
    }
}

void SmallStoneMember::animControl() {
    if (_94 == true) {
        if (!MR::isBckPlaying(this, "WindLoop")) {
            _95 = false;
            MR::onCalcAnim(this);
            MR::startBck(this, "WindLoop", nullptr);
        }
        _94 = false;
    } else {
        if (MR::isBckPlaying(this, "WindLoop")) {
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
