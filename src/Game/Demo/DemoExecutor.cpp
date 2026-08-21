#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoActionKeeper.hpp"
#include "Game/Demo/DemoCameraKeeper.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Demo/DemoPlayerKeeper.hpp"
#include "Game/Demo/DemoSoundKeeper.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"
#include "Game/Demo/DemoTalkAnimCtrl.hpp"
#include "Game/Demo/DemoTimeKeeper.hpp"
#include "Game/Demo/DemoWipeKeeper.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <algorithm>

DemoExecutor::DemoExecutor(const char* pName)
    : DemoCastGroup(pName), mSheetName(), mTimeKeeper(), mSubPartKeeper(), mPlayerKeeper(), mCameraKeeper(), mActionKeeper(), mWipeKeeper(),
      mSoundKeeper(), mSheetKeeper(), _40(), _44(), _48(), _4C(-1), _50(), mActor(), mTalkAnimCtrl(), mTalkMessageCtrl() {
}

void DemoExecutor::init(const JMapInfoIter& rIter) {
    DemoCastGroup::init(rIter);

    mSheetName = MR::getDemoSheetName(rIter);
    mTimeKeeper = new DemoTimeKeeper(this);
    mSubPartKeeper = new DemoSubPartKeeper(this);
    mPlayerKeeper = new DemoPlayerKeeper(this);
    mCameraKeeper = new DemoCameraKeeper(this, rIter);
    mActionKeeper = new DemoActionKeeper(this);
    mWipeKeeper = new DemoWipeKeeper(this);
    mSheetKeeper.push_back(mWipeKeeper);
    mSoundKeeper = new DemoSoundKeeper(this);
    mSheetKeeper.push_back(mSoundKeeper);
    _40 = MR::createStageSwitchCtrl(this, rIter);

    if (_40->isValidSwitchAppear()) {
        MR::listenNameObjStageSwitchOnAppear(this, _40, MR::Functor_Inline(this, &DemoExecutor::startProperDemoSystem));
    }

    DemoFunction::registerDemoExecutor(this);
}

void DemoExecutor::registerDemoActor(LiveActor* pActor, const JMapInfoIter& rIter) {
    DemoCastGroup::registerDemoActor(pActor, rIter);
    mCameraKeeper->initCast(pActor, rIter);
    mActionKeeper->initCast(pActor, rIter);

    for (DemoSheetKeeperBase** it = mSheetKeeper.begin(); it != mSheetKeeper.end(); it++) {
        (*it)->initCast(pActor, rIter);
    }
}

void DemoExecutor::movement() {
    mTimeKeeper->update();

    if (mTimeKeeper->isDemoEnd()) {
        end();
    } else {
        if (!mTimeKeeper->mIsPaused) {
            mSubPartKeeper->update();
            mPlayerKeeper->update();
            mCameraKeeper->update();
            mActionKeeper->update();
            std::for_each(mSheetKeeper.begin(), mSheetKeeper.end(), std::mem_func(&DemoSheetKeeperBase::update));
        }

        /*
        for (DemoTalkAnimCtrl** it = mTalkAnimCtrl.begin(); it != mTalkAnimCtrl.end(); it++) {

        }
        */
    }
}

void DemoExecutor::start(NameObj* pParam1, const char* pParam2, s32 param3) {
    _44 = pParam1;
    _48 = pParam2;
    _4C = param3;

    mTimeKeeper->start();
    mCameraKeeper->start();
    std::for_each(mSheetKeeper.begin(), mSheetKeeper.end(), std::mem_func(&DemoSheetKeeperBase::start));

    /*
    for (DemoTalkAnimCtrl** it = mTalkAnimCtrl.begin(); it != mTalkAnimCtrl.end(); it++) {

    }
    */

    mActor.clear();

    for (s32 i = 0; i < mGroup->getObjectCount(); i++) {
        LiveActor* actor = mGroup->getActor(i);

        MR::sendMsgStartDemo(actor);
        DemoFunction::requestDemoCastMovementOn(actor);

        if (MR::isInvalidClipping(actor)) {
            continue;
        }

        mActor.push_back(actor);
    }
}

void DemoExecutor::startPart(NameObj* pParam1, const char* pParam2, const char* pParam3, s32 param4) {
    // std::for_each(mTalkAnimCtrl.begin(), mTalkAnimCtrl.end(), std::mem_func(&DemoTalkAnimCtrl::setupStartDemoPart));
    start(pParam1, pParam2, param4);
    mTimeKeeper->setStartPart(pParam3);
}

void DemoExecutor::startProperDemoSystem() {
    if (mPlayerKeeper->mNumPlayerInfos > 0) {
        MR::requestStartTimeKeepDemoMarioPuppetable(this, mName, nullptr);
    } else {
        MR::requestStartTimeKeepDemo(this, mName, nullptr);
    }
}

void DemoExecutor::startDemoSystemPart(const char* pParam1, s32 param2) {
    // std::for_each(mTalkAnimCtrl.begin(), mTalkAnimCtrl.end(), std::mem_func(&DemoTalkAnimCtrl::setupStartDemoPart));

    switch (param2) {
    case 1:
        MR::startTimeKeepDemo(this, mName, nullptr);
        break;
    case 2:
        MR::startTimeKeepDemoMarioPuppetable(this, mName, nullptr);
        break;
    }

    mTimeKeeper->setStartPart(pParam1);
}

bool DemoExecutor::tryStartProperDemoSystem() {
    if (mPlayerKeeper->mNumPlayerInfos > 0) {
        return MR::requestStartTimeKeepDemoMarioPuppetable(this, mName, nullptr);
    } else {
        return MR::requestStartTimeKeepDemo(this, mName, nullptr);
    }
}

bool DemoExecutor::tryStartDemoSystemPart(const char* pParam1, s32 param2) {
    // std::for_each(mTalkAnimCtrl.begin(), mTalkAnimCtrl.end(), std::mem_func(&DemoTalkAnimCtrl::setupStartDemoPart));

    bool result = false;

    switch (param2) {
    case 1:
        result = MR::tryStartTimeKeepDemo(this, mName, nullptr);
        break;
    case 2:
        result = MR::tryStartTimeKeepDemoMarioPuppetable(this, mName, nullptr);
        break;
    }

    if (result) {
        mTimeKeeper->setStartPart(pParam1);

        return true;
    }

    return false;
}

bool DemoExecutor::tryStartProperDemoSystemPart(const char* pParam1) {
    if (mPlayerKeeper->mNumPlayerInfos > 0) {
        return MR::tryStartTimeKeepDemoMarioPuppetable(this, mName, pParam1);
    } else {
        return MR::tryStartTimeKeepDemo(this, mName, pParam1);
    }
}

void DemoExecutor::pause() {
    mTimeKeeper->mIsPaused = true;
}

void DemoExecutor::resume() {
    mTimeKeeper->mIsPaused = false;
}

void DemoExecutor::addTalkAnimCtrl(DemoTalkAnimCtrl* pCtrl) {
    mTalkAnimCtrl.push_back(pCtrl);
}

void DemoExecutor::addTalkMessageCtrl(LiveActor* pActor, TalkMessageCtrl* pCtrl) {
    DemoTalkMessageCtrl talkMessageCtrl = {pActor, pCtrl};
    mTalkMessageCtrl.push_back(talkMessageCtrl);
}

TalkMessageCtrl* DemoExecutor::findTalkMessageCtrl(const LiveActor* pActor) const {
    for (const DemoTalkMessageCtrl* it = mTalkMessageCtrl.begin(); it != mTalkMessageCtrl.end(); it++) {
        if (it->mActor != pActor) {
            continue;
        }

        return it->mTalkMessageCtrl;
    }

    return nullptr;
}

void DemoExecutor::setTalkMessageCtrl(const LiveActor* pActor, TalkMessageCtrl* pCtrl) {
    for (DemoTalkMessageCtrl* it = mTalkMessageCtrl.begin(); it != mTalkMessageCtrl.end(); it++) {
        if (it->mActor != pActor) {
            continue;
        }

        it->mTalkMessageCtrl = pCtrl;
        break;
    }
}

void DemoExecutor::end() {
    mTimeKeeper->end();
    mSubPartKeeper->end();
    mCameraKeeper->end();
    std::for_each(mSheetKeeper.begin(), mSheetKeeper.end(), std::mem_func(&DemoSheetKeeperBase::end));

    switch (_4C) {
    case 1:
        MR::endDemo(_44, _48);
        break;
    case 2:
        MR::endDemo(_44, _48);
        break;
    }

    if (_40->isValidSwitchDead()) {
        _40->onSwitchDead();
    }

    _44 = nullptr;
    _48 = nullptr;
    _4C = -1;

    for (LiveActor** it = mActor.begin(); it != mActor.end(); it++) {
        MR::validateClipping(*it);
    }

    mActor.clear();
}
