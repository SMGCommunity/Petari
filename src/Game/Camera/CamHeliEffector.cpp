#include "Game/Camera/CamHeliEffector.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Player/MarioActor.hpp"  // DO NOT REMOVE

CamHeliEffector::CamHeliEffector() : _0(0), _4(0.0f) {}

namespace NrvMarioActor {
    MarioActorNrvWait::MarioActorNrvWait() {}

    void MarioActorNrvWait::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeWait();
    }

    MarioActorNrvGameOver::MarioActorNrvGameOver() {}

    void MarioActorNrvGameOver::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOver();
    }

    MarioActorNrvGameOverAbyss::MarioActorNrvGameOverAbyss() {}

    void MarioActorNrvGameOverAbyss::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverAbyss();
    }

    MarioActorNrvGameOverAbyss2::MarioActorNrvGameOverAbyss2() {}

    void MarioActorNrvGameOverAbyss2::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverAbyss();
    }

    MarioActorNrvGameOverFire::MarioActorNrvGameOverFire() {}

    void MarioActorNrvGameOverFire::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverFire();
    }

    MarioActorNrvGameOverBlackHole::MarioActorNrvGameOverBlackHole() {}

    void MarioActorNrvGameOverBlackHole::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverBlackHole2();
    }

    MarioActorNrvGameOverNonStop::MarioActorNrvGameOverNonStop() {}

    void MarioActorNrvGameOverNonStop::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverNonStop();
    }

    MarioActorNrvGameOverSink::MarioActorNrvGameOverSink() {}

    void MarioActorNrvGameOverSink::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeGameOverSink();
    }

    MarioActorNrvTimeWait::MarioActorNrvTimeWait() {}

    void MarioActorNrvTimeWait::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeTimeWait();
    }

    MarioActorNrvNoRush::MarioActorNrvNoRush() {}

    void MarioActorNrvNoRush::execute(Spine* pSpine) const {
        MarioActor* actor = reinterpret_cast< MarioActor* >(pSpine->mExecutor);
        actor->exeWait();
    }

    MarioActorNrvWait MarioActorNrvWait::sInstance;
    MarioActorNrvGameOver MarioActorNrvGameOver::sInstance;
    MarioActorNrvGameOverAbyss MarioActorNrvGameOverAbyss::sInstance;
    MarioActorNrvGameOverAbyss2 MarioActorNrvGameOverAbyss2::sInstance;
    MarioActorNrvGameOverFire MarioActorNrvGameOverFire::sInstance;
    MarioActorNrvGameOverBlackHole MarioActorNrvGameOverBlackHole::sInstance;
    MarioActorNrvGameOverNonStop MarioActorNrvGameOverNonStop::sInstance;
    MarioActorNrvGameOverSink MarioActorNrvGameOverSink::sInstance;
    MarioActorNrvTimeWait MarioActorNrvTimeWait::sInstance;
    MarioActorNrvNoRush MarioActorNrvNoRush::sInstance;
}  // namespace NrvMarioActor
