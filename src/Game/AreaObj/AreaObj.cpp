#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaObjFollower.hpp"
#include "Game/Map/SleepControllerHolder.hpp"

AreaObj::AreaObj(int type, const char* pName)
    : NameObj(pName),
      mType(type),
      mIsValid(true),
      _15(true),
      mIsAwake(true),
      mObjArg0(-1),
      mObjArg1(-1),
      mObjArg2(-1),
      mObjArg3(-1),
      mObjArg4(-1),
      mObjArg5(-1),
      mObjArg6(-1),
      mObjArg7(-1),
      mSwitchCtrl(nullptr) {
    switch (type) {
    case 0:
        mForm = new AreaFormCube(0);
        break;
    case 1:
        mForm = new AreaFormCube(1);
        break;
    case 2:
        mForm = new AreaFormSphere();
        break;
    case 4:
        mForm = new AreaFormBowl();
        break;
    case 3:
        mForm = new AreaFormCylinder();
        break;
    }
}

// Issues with functors
void AreaObj::init(const JMapInfoIter& rIter) {
    mForm->init(rIter);
    MR::addBaseMatrixFollowerAreaObj(this, rIter);
    MR::getJMapInfoArg0WithInit(rIter, &mObjArg0);
    MR::getJMapInfoArg1WithInit(rIter, &mObjArg1);
    MR::getJMapInfoArg2WithInit(rIter, &mObjArg2);
    MR::getJMapInfoArg3WithInit(rIter, &mObjArg3);
    MR::getJMapInfoArg4WithInit(rIter, &mObjArg4);
    MR::getJMapInfoArg5WithInit(rIter, &mObjArg5);
    MR::getJMapInfoArg6WithInit(rIter, &mObjArg6);
    MR::getJMapInfoArg7WithInit(rIter, &mObjArg7);

    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);

    if (mSwitchCtrl->isValidSwitchAppear()) {
        MR::FunctorV0M<AreaObj*, void (AreaObj::*)()> validateFunc = MR::Functor<AreaObj>(this, &AreaObj::validate);
        MR::FunctorV0M<AreaObj*, void (AreaObj::*)()> invalidateFunc = MR::Functor<AreaObj>(this, &AreaObj::invalidate);
        MR::listenNameObjStageSwitchOnOffAppear(this, mSwitchCtrl, validateFunc, invalidateFunc);
        mIsValid = false;
    }

    const char* pName;

    if (MR::getObjectName(&pName, rIter)) {
        setName(pName);
    }

    MR::getAreaObjManager(getManagerName())->entry(this);
    MR::getAreaObjManager(getManagerName());
    SleepControlFunc::addSleepControl(this, rIter);
}

bool AreaObj::isInVolume(const TVec3f& rPos) const {
    return mIsValid && _15 && mIsAwake && mForm->isInVolume(rPos);
}

void AreaObj::onSwitchA() {
    mSwitchCtrl->onSwitchA();
}

void AreaObj::offSwitchA() {
    mSwitchCtrl->offSwitchA();
}

bool AreaObj::isOnSwitchA() const {
    return mSwitchCtrl->isOnSwitchA();
}

bool AreaObj::isOnSwitchB() const {
    return mSwitchCtrl->isOnSwitchB();
}

bool AreaObj::isValidSwitchA() const {
    return mSwitchCtrl->isValidSwitchA();
}

bool AreaObj::isValidSwitchB() const {
    return mSwitchCtrl->isValidSwitchB();
}

void AreaObj::setFollowMtx(const TPos3f* pMtx) {
    mForm->_4 = const_cast<TPos3f*>(pMtx);
}

TPos3f* AreaObj::getFollowMtx() const {
    return mForm->_4;
}

AreaObjMgr::AreaObjMgr(s32 count, const char* pName)
    : NameObj(pName),
      mArray(),
      _18(count) {
}

void AreaObjMgr::entry(AreaObj* pAreaObj) {
    if (mArray.capacity() == 0) {
        mArray.init(_18);
    }

    mArray.push_back(pAreaObj);
}

// AreaObjMgr::find_in

void AreaObj::validate() {
    mIsValid = true;
}

void AreaObj::invalidate() {
    mIsValid = false;
}

const char* AreaObj::getManagerName() const {
    return mName;
}
