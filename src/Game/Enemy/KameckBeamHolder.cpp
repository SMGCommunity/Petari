#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckTurtle.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

KameckBeamHolder::KameckBeamHolder() : DeriveActorGroup< KameckBeam >("カメックビーム管理", 0x10) {
    for (s32 i = 0; i < 0x10; i++) {
        KameckBeam* b = new KameckBeam("カメックビーム");
        b->initWithoutIter();
        registerActor(b);
    }
}

KameckBeamHolder::~KameckBeamHolder() {
}

KameckBeamTurtleHolder::KameckBeamTurtleHolder() : DeriveActorGroup< KameckTurtle >("カメックビーム用カメ管理", 0x10) {
    for (s32 i = 0; i < 0x10; i++) {
        KameckTurtle* turtle = new KameckTurtle("カメックビーム用カメ");
        turtle->initWithoutIter();
        turtle->makeActorDead();
        registerActor(turtle);
    }
}

KameckBeamTurtleHolder::~KameckBeamTurtleHolder() {
}

namespace MR {
    void createKameckBeamHolder() {
        MR::createSceneObj(SceneObj_KameckBeamHolder);
    }

    void createKameckFireBallHolder() {
        MR::createSceneObj(SceneObj_KameckFireBallHolder);
    }

    void createKameckBeamTurtleHolder() {
        MR::createSceneObj(SceneObj_KameckBeamTurtleHolder);
    }
};  // namespace MR
