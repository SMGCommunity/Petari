#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckFireBall.hpp"
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

KameckFireBallHolder::KameckFireBallHolder() : DeriveActorGroup< KameckFireBall >("カメック火の玉管理", 0x10) {
    for (s32 i = 0; i < 0x10; i++) {
        KameckFireBall* b = new KameckFireBall("カメックビーム用炎");
        b->initWithoutIter();
        b->makeActorDead();
        registerActor(b);
    }
}

KameckBeamTurtleHolder::KameckBeamTurtleHolder() : DeriveActorGroup< KameckTurtle >("カメックビーム用カメ管理", 0x10) {
    for (s32 i = 0; i < 0x10; i++) {
        KameckTurtle* turtle = new KameckTurtle("カメックビーム用カメ");
        turtle->initWithoutIter();
        turtle->makeActorDead();
        registerActor(turtle);
    }
}

KameckFireBallHolder::~KameckFireBallHolder() {
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

    // MR::startFollowKameckBeam

    KameckBeam* getKameckBeam() {
        KameckBeamHolder* beam = MR::getSceneObj< KameckBeamHolder >(SceneObj_KameckBeamHolder);

        if (beam->getDeadActor()) {
            return reinterpret_cast< KameckBeam* >(beam->getDeadActor());
        }

        return nullptr;
    }

    KameckFireBall* getKameckFireBall() {
        KameckFireBallHolder* fire = MR::getSceneObj< KameckFireBallHolder >(SceneObj_KameckFireBallHolder);

        if (fire->getDeadActor()) {
            return reinterpret_cast< KameckFireBall* >(fire->getDeadActor());
        }

        return nullptr;
    }

    KameckTurtle* getKameckBeamTurtle() {
        KameckBeamTurtleHolder* t = MR::getSceneObj< KameckBeamTurtleHolder >(SceneObj_KameckBeamTurtleHolder);

        if (t->getDeadActor()) {
            return reinterpret_cast< KameckTurtle* >(t->getDeadActor());
        }

        return nullptr;
    }
};  // namespace MR
