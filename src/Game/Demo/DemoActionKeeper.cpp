#include "Game/Demo/DemoActionKeeper.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Util.hpp"

void DemoActionInfo::registerCast(LiveActor *pActor) {
    mCastList[mCastCount] = pActor;
    mCastCount++;
}

void DemoActionInfo::registerFunctor(const LiveActor *pActor, const MR::FunctorBase &rFunctor) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mFunctors[i] = rFunctor.clone(nullptr);
            return;
        }
    }
}

void DemoActionInfo::registerNerve(const LiveActor *pActor, const Nerve *pNerve) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mNerves[i] = pNerve;
            return;
        }
    }
}

void DemoActionKeeper::registerNerve(const LiveActor *pActor, const Nerve *pNerve, const char *pPartName) {
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
            }
            else if (MR::isDemoPartLastStep(info->mPartName)) {
                mInfoArray[i]->executeActionLast();
            }
        }
    }
}

bool DemoActionKeeper::isRegisteredDemoActionAppear(const LiveActor *pActor) const {
    return isRegisteredDemoAction(pActor, 0);
}

bool DemoActionKeeper::isRegisteredDemoActionFunctor(const LiveActor *pActor) const {
    return isRegisteredDemoAction(pActor, 2);
}

bool DemoActionKeeper::isRegisteredDemoActionNerve(const LiveActor *pActor) const {
    return isRegisteredDemoAction(pActor, 3);
}

DemoActionKeeper::DemoActionKeeper(const DemoExecutor *pExector) {
    mDemoExecutor = pExector;
    mNumInfos = 0;
    mInfoArray = nullptr;
    JMapInfo* jmap = nullptr;
    mNumInfos = DemoFunction::createSheetParser(mDemoExecutor, "Action", &jmap);
    mInfoArray = new DemoActionInfo*[mNumInfos];

    for (s32 i = 0; i < mNumInfos; i++) {
        mInfoArray[i] = new DemoActionInfo();
        DemoActionInfo* info = mInfoArray[i];
        jmap->getValue<const char*>(i, "PartName", &info->mPartName);
        jmap->getValue<const char*>(i, "CastName", &info->mCastName);
        jmap->getValue<s32>(i, "CastID", &info->mCastID);
        jmap->getValue<s32>(i, "ActionType", &info->mActionType);
        jmap->getValue<const char*>(i, "PosName", &info->mPosName);
        jmap->getValue<const char*>(i, "AnimName", &info->mAnimName);
    }
}
