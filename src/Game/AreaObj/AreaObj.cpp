#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObjFollower.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Util.hpp"

AreaObj::AreaObj(int type, const char *pName) : NameObj(pName) {
    mType = type;
    mValid = true;
    _15 = true;
    mAwake = true;
    mObjArg0 = -1;
    mObjArg1 = -1;
    mObjArg2 = -1;
    mObjArg3 = -1;
    mObjArg4 = -1;
    mObjArg5 = -1;
    mObjArg6 = -1;
    mObjArg7 = -1;
    mSwitchCtrl = 0;

    switch(type) {
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
void AreaObj::init(const JMapInfoIter &rIter) {
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
        MR::FunctorV0M<AreaObj *, void (AreaObj::*)()> validateFunc = MR::Functor<AreaObj>(this, &AreaObj::validate);
        MR::FunctorV0M<AreaObj *, void (AreaObj::*)()> invalidateFunc = MR::Functor<AreaObj>(this, &AreaObj::invalidate);
        MR::listenNameObjStageSwitchOnOffAppear(this, mSwitchCtrl, validateFunc, invalidateFunc);
        mValid = false;
    }

    const char* objName;
    bool validName = MR::getObjectName(&objName, rIter);

    if (validName) {
        setName(objName);
    }

    const char* managerName = getManagerName();
    MR::getAreaObjManager(managerName)->entry(this);
    MR::getAreaObjManager(getManagerName());
    SleepControlFunc::addSleepControl(this, rIter);
}

bool AreaObj::isInVolume(const TVec3f &rPos) const {
    bool ret = false;

    if (mValid && _15 && mAwake) {
        if (mForm->isInVolume(rPos)) {
            ret = true;
        }
    }

    return ret;
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
    return mSwitchCtrl->isOnSwitchA();
}

bool AreaObj::isValidSwitchB() const {
    return mSwitchCtrl->isOnSwitchB();
}

void AreaObj::setFollowMtx(const TPos3f *pMtx) {
    mForm->_4 = (TPos3f*)pMtx;
}

TPos3f* AreaObj::getFollowMtx() const {
    return mForm->_4;
}

AreaObjMgr::AreaObjMgr(s32 count, const char* pName) : NameObj(pName), mArray() {
    mArray.mCount = 0;
    _18 = count;
}

void AreaObjMgr::entry(AreaObj *pAreaObj) {
    if (!mArray.mCount) {
        u32 cnt = _18;
        mArray.mArray.mArr = new AreaObj*[cnt];
        mArray.mArray.mMaxSize = cnt;
    }

    u32 count = mArray.mCount;
    mArray.mCount = count + 1;
    mArray.mArray.mArr[count] = pAreaObj;
}

// AreaObjMgr::find_in

void AreaObj::validate() {
    mValid = true;
}

void AreaObj::invalidate() {
    mValid = false;
}

const char* AreaObj::getManagerName() const {
    return mName;
}
