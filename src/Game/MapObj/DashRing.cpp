#include "Game/MapObj/DashRing.hpp"

DashRing::~DashRing() {

}

void DashRing::initCommon(const JMapInfoIter &rIter, const char *pName) {
    MR::initDefaultPos(this, rIter);
    _A4 = 0;
    _A6 = 0;
    _B4 = 0;

    if (pName) {
        initModelManagerWithAnm(pName, nullptr, false);
        MR::connectToSceneMapObj(this);
        _B4 = 1;
    }
    else {
        MR::connectToScene(this, 34, -1, -1, 56);
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 4, 1000.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::validateClipping(this);
    calcAxis();
    
    _A8 = 300;
    _AC = 120;
    _B0 = 2.5f;
    _B5 = 1;

    if (MR::checkJMapDataEntries(rIter)) {
        s32 arg0 = -1;
        s32 arg1 = -1;
        s32 arg2 = -1;

        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        MR::getJMapInfoArg1NoInit(rIter, &arg1);

        if (arg0 != -1) {
            _A8 = arg0;
        }

        if (arg1 != -1) {
            _AC = arg1;
        }

        if (MR::getJMapInfoArg2NoInit(rIter, &arg2) && arg2 != -1) {
            _B0 = arg2 / 100.0f;
        }

        s32 arg3 = -1;
        MR::getJMapInfoArg3NoInit(rIter, &arg3);

        if (arg3 != -1) {
            _B5 = 0;
        }
    }

    initSound(3, false);
    initEffectKeeper(3, nullptr, false);
    appear();

    if (_B4) {
        const char* animName = "Loop";
        MR::startBck(this, animName, 0);
        MR::startBrk(this, animName);
    }

    _B8 = mPosition;
}

// DashRing::attackSensor
// DashRing::control

void DashRing::calcAndSetBaseMtx() {
    if (_B4) {
        TPos3f pos;
        MR::makeMtxFrontSidePos(&pos, mAxis, mSubAxis, mPosition);
        PSMTXCopy(pos.mMtx, getBaseMtx());
    }
}

void DashRing::calcAxis() {
    TVec3f stack_8(0.0f, 1.0f, 0.0f);
    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y, mRotation.z);
    PSMTXMultVec(mtx, &stack_8, &stack_8);
    mAxis = stack_8;
    calcSubAxis();
}

void DashRing::calcSubAxis() {
    TVec3f stack_14(mAxis);
    TVec3f stack_8;

    if (MR::isNearZero(stack_14.y, 0.001f)) {
        stack_8.x = stack_14.y;
        stack_8.y = -stack_14.x;
        stack_8.z = 0.0f;
    }
    else {
        stack_8.x = stack_14.x;
        stack_8.y = 0.0f;
        stack_8.z = -stack_14.y;
    }

    MR::normalizeOrZero(&stack_8);
    mSubAxis = stack_8;
}

// DashRing::draw

void DashRing::init(const JMapInfoIter &rIter) {
    initCommon(rIter, "DashRing");
}
