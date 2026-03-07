#include "Game/Demo/DemoActionKeeper.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Demo/DemoTalkAnimCtrl.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util.hpp"

namespace {
    TalkMessageCtrl* findTalkMessageCtrl(LiveActor* pActor) {
        DemoExecutor* executor = DemoFunction::findDemoExecutorActive(pActor);
        if (executor) {
            return executor->findTalkMessageCtrl(pActor);
        }
        return nullptr;
    }

    void setTalkAnimCtrlInterpole(LiveActor* pActor, s32 interpole) {
        DemoExecutor* executor = DemoFunction::findDemoExecutorActive(pActor);
        for (s32 i = 0; i < executor->mNumTalkAnimCtrls; i++) {
            executor->mTalkAnimCtrls[i]->_38 = interpole;
        }
    }
}

DemoActionInfo::DemoActionInfo() {
    mPartName = nullptr;
    mCastName = nullptr;
    mCastID = -1;
    mActionType = 0;
    mPosName = nullptr;
    mAnimName = nullptr;
    _18 = 128;
    mCastCount = 0;
    mCastList = nullptr;
    mFunctors = nullptr;
    mNerves = nullptr;
    _2C = 0;
    mCastList = new LiveActor*[128];
    mFunctors = new MR::FunctorBase*[_18];
    mNerves = new const Nerve*[_18];
    for (s32 i = 0; i < _18; i++) {
        mCastList[i] = nullptr;
        mFunctors[i] = nullptr;
        mNerves[i] = nullptr;
    }
}

void DemoActionInfo::registerCast(LiveActor* pActor) {
    mCastList[mCastCount] = pActor;
    mCastCount++;
}

void DemoActionInfo::registerFunctor(const LiveActor* pActor, const MR::FunctorBase& rFunctor) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mFunctors[i] = rFunctor.clone(nullptr);
            return;
        }
    }
}

void DemoActionInfo::registerNerve(const LiveActor* pActor, const Nerve* pNerve) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mNerves[i] = pNerve;
            return;
        }
    }
}

void DemoActionInfo::executeActionFirst() const {
    if (mCastCount > 0) {
        setTalkAnimCtrlInterpole(mCastList[0], 0);
    }

    for (s32 i = 0; i < mCastCount; i++) {
        LiveActor* actor = mCastList[i];
        if (mActionType == 2) {
            mFunctors[i]->operator()();
        } else if (mActionType == 3) {
            actor->setNerve(mNerves[i]);
        } else if (mActionType == 0) {
            actor->makeActorAppeared();
        } else if (mActionType == 1) {
            actor->makeActorDead();
        } else if (mActionType == 4) {
            MR::onSwitchA(actor);
        } else if (mActionType == 5) {
            MR::onSwitchB(actor);
        } else if (mActionType == 6) {
            MR::showModel(actor);
        } else if (mActionType == 7) {
            MR::hideModel(actor);
        } else if (mActionType == 8) {
            MR::tryTalkTimeKeepDemoMarioPuppetable(findTalkMessageCtrl(actor));
        } else if (mActionType == 9) {
            MR::tryTalkTimeKeepDemoWithoutPauseMarioPuppetable(findTalkMessageCtrl(actor));
        } else if (mActionType == 10) {
            MR::tryTalkTimeKeepDemoWithoutPauseMarioPuppetable(findTalkMessageCtrl(actor));
        } else if (mActionType == 11) {
            // noop
        } else if (mActionType == 12) {
            MR::offSwitchA(actor);
        } else if (mActionType == 13) {
            MR::offSwitchB(actor);
        }

        if (!MR::isNullOrEmptyString(mAnimName)) {
            MR::startAction(actor, mAnimName);
        }

        if (!MR::isNullOrEmptyString(mPosName)) {
            MR::findNamePos(mPosName, &actor->mPosition, &actor->mRotation);
        }
    }
}

void DemoActionInfo::executeActionLast() const {
    if (mCastCount > 0) {
        setTalkAnimCtrlInterpole(mCastList[0], 0);
    }

    for (s32 i = 0; i < mCastCount; i++) {
        LiveActor* actor = mCastList[i];
        if (mActionType == 9) {
            setTalkAnimCtrlInterpole(actor, -1);
        }
    }
}

void DemoActionKeeper::initCast(LiveActor* pActor, const JMapInfoIter& rIter) {
    if (mNumInfos <= 0) {
        return;
    }

    for (s32 i = 0; i < mNumInfos; i++) {
        DemoActionInfo* info = mInfoArray[i];
        if (DemoFunction::isTargetDemoCast(pActor, rIter, info->mCastName, info->mCastID)) {
            mInfoArray[i]->registerCast(pActor);
        }
    }
}

void DemoActionKeeper::registerFunctor(const LiveActor* pActor, const MR::FunctorBase& rFunctor, const char* pPartName) {
    for (s32 i = 0; i < mNumInfos; i++) {
        DemoActionInfo* info = mInfoArray[i];

        if (info && (pPartName == nullptr || MR::isEqualString(info->mPartName, pPartName))) {
            for (s32 j = 0; j < info->mCastCount; j++) {
                if (MR::isSame(info->mCastList[j], pActor)) {
                    info->registerFunctor(pActor, rFunctor);
                }
            }
        }
    }
}

void DemoActionKeeper::registerNerve(const LiveActor* pActor, const Nerve* pNerve, const char* pPartName) {
    for (s32 i = 0; i < mNumInfos; i++) {
        DemoActionInfo* info = mInfoArray[i];

        if (info && (pPartName == nullptr || MR::isEqualString(info->mPartName, pPartName))) {
            for (s32 j = 0; j < info->mCastCount; j++) {
                if (MR::isSame(info->mCastList[j], pActor)) {
                    info->registerNerve(pActor, pNerve);
                }
            }
        }
    }
}

void DemoActionKeeper::update() {
    DemoActionInfo* info;
    if (mNumInfos > 0) {
        for (s32 i = 0; i < mNumInfos; i++) {
            info = mInfoArray[i];
            if (MR::isDemoPartFirstStep(info->mPartName)) {
                mInfoArray[i]->executeActionFirst();
            } else if (MR::isDemoPartLastStep(info->mPartName)) {
                mInfoArray[i]->executeActionLast();
            }
        }
    }
}

bool DemoActionKeeper::isRegisteredDemoAction(const LiveActor* pActor, s32 actionType) const {
    for (s32 i = 0; i < mNumInfos; i++) {
        DemoActionInfo* info = mInfoArray[i];
        for (s32 j = 0; j < info->mCastCount; j++) {
            if (pActor == info->mCastList[j] && info->mActionType == actionType) {
                return true;
            }
        }
    }

    return false;
}

bool DemoActionKeeper::isRegisteredDemoActionAppear(const LiveActor* pActor) const {
    return isRegisteredDemoAction(pActor, 0);
}

bool DemoActionKeeper::isRegisteredDemoActionFunctor(const LiveActor* pActor) const {
    return isRegisteredDemoAction(pActor, 2);
}

bool DemoActionKeeper::isRegisteredDemoActionNerve(const LiveActor* pActor) const {
    return isRegisteredDemoAction(pActor, 3);
}

DemoActionKeeper::DemoActionKeeper(const DemoExecutor* pExector) {
    mDemoExecutor = pExector;
    mNumInfos = 0;
    mInfoArray = nullptr;
    JMapInfo* jmap = nullptr;
    mNumInfos = DemoFunction::createSheetParser(mDemoExecutor, "Action", &jmap);
    mInfoArray = new DemoActionInfo*[mNumInfos];

    for (s32 i = 0; i < mNumInfos; i++) {
        mInfoArray[i] = new DemoActionInfo();
        DemoActionInfo* info = mInfoArray[i];
        jmap->getValue< const char* >(i, "PartName", &info->mPartName);
        jmap->getValue< const char* >(i, "CastName", &info->mCastName);
        jmap->getValue< s32 >(i, "CastID", &info->mCastID);
        jmap->getValue< s32 >(i, "ActionType", &info->mActionType);
        jmap->getValue< const char* >(i, "PosName", &info->mPosName);
        jmap->getValue< const char* >(i, "AnimName", &info->mAnimName);
    }
}
