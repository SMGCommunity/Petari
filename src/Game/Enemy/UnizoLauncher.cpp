#include "Game/Enemy/UnizoLauncher.hpp"
#include "Game/Enemy/Unizo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace {
    const s32 sUnizoNumber = 4;
    const s32 sUnizoInter = 300;
}  // namespace

namespace NrvUnizoLauncher {
    INIT_NERVE(UnizoLauncherNrvWait);
    INIT_NERVE(UnizoLauncherNrvLaunch);

    inline void UnizoLauncherNrvWait::execute(Spine* pSpine) const {
        UnizoLauncher* pActor = static_cast< UnizoLauncher* >(pSpine->mExecutor);
        if (MR::isFirstStep(pActor)) {
        }

        if (MR::isGreaterStep(pActor, sUnizoInter)) {
            pActor->setNerve(&UnizoLauncherNrvLaunch::sInstance);
        }
    }

    inline void UnizoLauncherNrvLaunch::execute(Spine* pSpine) const {
        static_cast< UnizoLauncher* >(pSpine->mExecutor)->exeLaunch();
    }
}  // namespace NrvUnizoLauncher

UnizoLauncher::UnizoLauncher(const char* pName) : LiveActor(pName) {
}

void UnizoLauncher::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemyMovement(this);
    initHitSensor(1);
    MR::addHitSensorEnemy(this, "Body", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initSound(8, false);
    initNerve(&NrvUnizoLauncher::UnizoLauncherNrvWait::sInstance);

    mUnizoNum = sUnizoNumber;
    mUnizos = new Unizo*[sUnizoNumber];
    MR::setGroupClipping(this, rIter, 16);

    for (s32 i = 0; i < mUnizoNum; i++) {
        mUnizos[i] = new Unizo("陸ウニゾー");
        mUnizos[i]->init(rIter);
        mUnizos[i]->makeActorDead();
        MR::setGroupClipping(mUnizos[i], rIter, 16);
    }

    makeActorAppeared();
}

void UnizoLauncher::exeLaunch() {
    if (MR::isFirstStep(this)) {
        for (s32 i = 0; i < mUnizoNum; i++) {
            Unizo* pUnizo = mUnizos[i];
            if (MR::isDead(pUnizo)) {
                MR::copyTransRotateScale(this, pUnizo);
                pUnizo->mPosition.z += i;
                pUnizo->appear();
                break;
            }
        }
    }

    setNerve(&NrvUnizoLauncher::UnizoLauncherNrvWait::sInstance);
}

UnizoLauncher::~UnizoLauncher() {
}
