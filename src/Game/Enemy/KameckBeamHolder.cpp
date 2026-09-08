#include "Game/Enemy/KameckBeamHolder.hpp"
#include "Game/Enemy/KameckBeam.hpp"
#include "Game/Enemy/KameckFireBall.hpp"
#include "Game/Enemy/KameckTurtle.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static const s32 sMaxKameckBeam = 16;
    static const s32 sMaxKameckFireBall = 16;
    static const s32 sMaxKameckBeamTurtle = 16;
};  // namespace

KameckBeamHolder::KameckBeamHolder() : DeriveActorGroup< KameckBeam >("カメックビーム管理", ::sMaxKameckBeam) {
    KameckBeam* pBeam;

    for (s32 i = 0; i < ::sMaxKameckBeam; i++) {
        pBeam = new KameckBeam("カメックビーム");
        pBeam->initWithoutIter();
        registerActor(pBeam);
    }
}

KameckFireBallHolder::KameckFireBallHolder() : DeriveActorGroup< KameckFireBall >("カメック火の玉管理", ::sMaxKameckFireBall) {
    KameckFireBall* pFireBall;

    for (s32 i = 0; i < ::sMaxKameckFireBall; i++) {
        pFireBall = new KameckFireBall("カメックビーム用炎");
        pFireBall->initWithoutIter();
        pFireBall->makeActorDead();
        registerActor(pFireBall);
    }
}

KameckBeamTurtleHolder::KameckBeamTurtleHolder() : DeriveActorGroup< KameckTurtle >("カメックビーム用カメ管理", ::sMaxKameckBeamTurtle) {
    KameckTurtle* pTurtle;

    for (s32 i = 0; i < ::sMaxKameckBeamTurtle; i++) {
        pTurtle = new KameckTurtle("カメックビーム用カメ");
        pTurtle->initWithoutIter();
        pTurtle->makeActorDead();
        registerActor(pTurtle);
    }
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

    KameckBeam* startFollowKameckBeam(s32 beamKind, MtxPtr pMtx, f32 scale, const TVec3f& rWandLocalPos, KameckBeamEventListener* pEventListener) {
        KameckBeam* pBeam = getKameckBeam();

        if (pBeam == nullptr) {
            return nullptr;
        }

        pBeam->setBeamKind(beamKind);

        if (!pBeam->requestFollowWand(pMtx, scale)) {
            return nullptr;
        }

        pBeam->setWandLocalPosition(rWandLocalPos);
        pBeam->setEventListener(pEventListener);

        return pBeam;
    }

    KameckBeam* getKameckBeam() {
        return MR::getSceneObj< KameckBeamHolder >(SceneObj_KameckBeamHolder)->getDeadMember();
    }

    KameckFireBall* getKameckFireBall() {
        return MR::getSceneObj< KameckFireBallHolder >(SceneObj_KameckFireBallHolder)->getDeadMember();
    }

    KameckTurtle* getKameckBeamTurtle() {
        return MR::getSceneObj< KameckBeamTurtleHolder >(SceneObj_KameckBeamTurtleHolder)->getDeadMember();
    }
};  // namespace MR
