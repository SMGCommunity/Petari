#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObjFollower.hpp"
#include "Game/Map/SleepControllerHolder.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <algorithm>
#include <functional.hpp>


AreaObj::AreaObj(int formType, const char* pName)
    : NameObj(pName), mFormType(formType), mIsValid(true), _15(true), mIsAwake(true), mObjArg0(-1), mObjArg1(-1), mObjArg2(-1), mObjArg3(-1),
      mObjArg4(-1), mObjArg5(-1), mObjArg6(-1), mObjArg7(-1), mSwitchCtrl() {
    switch (formType) {
    case AreaForm::Type_Cube1:
        mForm = new AreaFormCube(0);
        break;
    case AreaForm::Type_Cube2:
        mForm = new AreaFormCube(1);
        break;
    case AreaForm::Type_Sphere:
        mForm = new AreaFormSphere();
        break;
    case AreaForm::Type_Bowl:
        mForm = new AreaFormBowl();
        break;
    case AreaForm::Type_Cylinder:
        mForm = new AreaFormCylinder();
        break;
    }
}

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
        MR::listenNameObjStageSwitchOnOffAppear(this, mSwitchCtrl, MR::Functor(this, &AreaObj::validate), MR::Functor(this, &AreaObj::invalidate));
        mIsValid = false;
    }

    const char* name;

    if (MR::getObjectName(&name, rIter)) {
        setName(name);
    }

    MR::getAreaObjManager(getManagerName())->entry(this);
    MR::getAreaObjManager(getManagerName());
    SleepControlFunc::addSleepControl(this, rIter);
}

bool AreaObj::isInVolume(const TVec3f& rPos) const {
    return isValid() && mForm->isInVolume(rPos);
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
    mForm->_4 = const_cast< TPos3f* >(pMtx);
}

TPos3f* AreaObj::getFollowMtx() const {
    return mForm->_4;
}

AreaObjMgr::AreaObjMgr(s32 count, const char* pName) : NameObj(pName), mArray(), _18(count) {
}

void AreaObjMgr::entry(AreaObj* pAreaObj) {
    if (mArray.capacity() == 0) {
        mArray.init(_18);
    }

    mArray.push_back(pAreaObj);
}

AreaObj* AreaObjMgr::find_in(const TVec3f& rVec) const {
    AreaObj* const* Begin = mArray.begin();
    AreaObj* const* End = mArray.end();
    AreaObj* const* p = std::rfind_if(End - 1, Begin - 1, std::bind2nd(std::mem_func(&AreaObj::isInVolume), rVec));

    p = (p == Begin - 1 ? End : p);

    if (p != mArray.end()) {
        return *p;
    }

    return nullptr;
}
