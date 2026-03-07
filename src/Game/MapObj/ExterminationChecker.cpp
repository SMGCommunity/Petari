#include "Game/MapObj/ExterminationChecker.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/MapObj/KeySwitch.hpp"

namespace NrvExterminationChecker {
    NEW_NERVE(ExterminationCheckerNrvWatching, ExterminationChecker, Watching);
    NEW_NERVE(ExterminationCheckerNrvTryStartDemoAppear, ExterminationChecker, TryStartDemoAppear);
    NEW_NERVE(ExterminationCheckerNrvAppearStar, ExterminationChecker, AppearStar);
    NEW_NERVE(ExterminationCheckerNrvAppearKeySwitch, ExterminationChecker, AppearKeySwitch);
};  // namespace NrvExterminationChecker

namespace {
    template < typename T >
    LiveActor* createLiveActor(const char* pName) {
        return new T(pName);
    }

    // we will define the creation funcs later
    static const ExterminationEntry sCreateTable[] = {{"ChildKuribo", nullptr}, {"ChildSkeletalFishBaby", nullptr}, {"ChildMeramera", nullptr}};

    inline CreationFunc findEntry(const char* pName) {
        for (s32 j = 0; j < sizeof(sCreateTable) / sizeof(*sCreateTable); j++) {
            if (sCreateTable[j].mChildName && MR::isEqualStringCase(sCreateTable[j].mChildName, pName)) {
                return sCreateTable[j].mCreationFunc;
            }
        }

        return nullptr;
    }
};  // namespace

ExterminationChecker::ExterminationChecker(const char* pName) : LiveActor(pName) {
    mGroup = nullptr;
    mKeySwitch = nullptr;
    mKeySwitchPos.x = 0.0f;
    mKeySwitchPos.y = 0.0f;
    mKeySwitchPos.z = 0.0f;
    _A0 = 1;
    _A1 = 0;
}

void ExterminationChecker::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    s32 objNum = MR::getChildObjNum(rIter);
    mGroup = new LiveActorGroup("アクター管理", objNum);

    for (s32 i = 0; i < objNum; i++) {
        const char* objName = nullptr;
        MR::getChildObjName(&objName, rIter, i);
        LiveActor* actor = findEntry(objName)(MR::getJapaneseObjectName(objName));
        MR::initChildObj(actor, rIter, i);
        mGroup->registerActor(actor);
    }

    MR::invalidateClipping(this);
    MR::useStageSwitchSleep(this, rIter);

    if (_A1) {
        mKeySwitch = new KeySwitch("全滅用キースイッチ");
        mKeySwitch->initKeySwitchByOwner(rIter);
    }

    if (_A0) {
        MR::declarePowerStar(this);
    }

    initNerve(&NrvExterminationChecker::ExterminationCheckerNrvWatching::sInstance);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void ExterminationChecker::control() {}

void ExterminationChecker::exeWatching() {
    for (s32 i = 0; i < mGroup->mObjectCount; i++) {
        LiveActor* cur = mGroup->getActor(i);
        if (!MR::isDead(cur)) {
            mKeySwitchPos.setPS(mGroup->getActor(i)->mPosition);
            return;
        }
    }

    if (_A0) {
        setNerve(&NrvExterminationChecker::ExterminationCheckerNrvTryStartDemoAppear::sInstance);
    }

    if (_A1) {
        setNerve(&NrvExterminationChecker::ExterminationCheckerNrvAppearKeySwitch::sInstance);
    }
}

void ExterminationChecker::exeTryStartDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(10);
    }

    if (MR::isGreaterStep(this, 5)) {
        if (MR::tryStartDemo(this, "全滅スター出現")) {
            setNerve(&NrvExterminationChecker::ExterminationCheckerNrvAppearStar::sInstance);
        }
    }
}

void ExterminationChecker::exeAppearStar() {
    if (MR::isStep(this, 0)) {
        MR::startSystemSE("SE_SY_TOTAL_COMPLETE", -1, -1);
    }

    if (MR::isStep(this, 120)) {
        MR::endDemo(this, "全滅スター出現");
        MR::startAfterBossBGM();
        MR::requestAppearPowerStar(this);
        kill();
    }
}

void ExterminationChecker::exeAppearKeySwitch() {
    if (MR::isStep(this, 30)) {
        mKeySwitch->appearKeySwitch(mKeySwitchPos);
        kill();
    }
}

namespace MR {
    NameObj* createExterminationPowerStar(const char* pName) {
        ExterminationChecker* checker = new ExterminationChecker(pName);
        checker->_A0 = 1;
        checker->_A1 = 0;
        return checker;
    }

    NameObj* createExterminationKeySwitch(const char* pName) {
        ExterminationChecker* checker = new ExterminationChecker(pName);
        checker->_A0 = 0;
        checker->_A1 = 1;
        return checker;
    }
};  // namespace MR

ExterminationChecker::~ExterminationChecker() {}
