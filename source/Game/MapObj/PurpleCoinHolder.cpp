#include "Game/MapObj/PurpleCoinHolder.h"
#include "Game/MapObj/Coin.h"
#include "Game/MapObj/PurpleCoinStarter.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/Util.h"

PurpleCoinHolder::PurpleCoinHolder() : DeriveActorGroup<Coin>("パープルコイン管理", 0x100) {
    mStarter = nullptr;
    MR::declarePowerStarCoin100();
    MR::createPurpleCoinCounter();
}

void PurpleCoinHolder::initAfterPlacement() {
    if (!mStarter) {
        MR::validatePurpleCoinCounter();
    }
}

void PurpleCoinHolder::start() {
    MR::validatePurpleCoinCounter();
}

namespace MR {
    void createPurpleCoinHolder() {
        MR::createSceneObj(SceneObj_PurpleCoinHolder);
    }

    void addToPurpleCoinHolder(const NameObj *pObj, Coin *pCoin) {
        MR::getSceneObj<PurpleCoinHolder*>(SceneObj_PurpleCoinHolder)->registerActor(pCoin);
    }

    void registPurpleCoinStarter(PurpleCoinStarter *pStarter) {
        PurpleCoinHolder* holder = MR::getSceneObj<PurpleCoinHolder*>(SceneObj_PurpleCoinHolder);
        holder->mStarter = pStarter;
        pStarter->setHost(holder);
    }
};

PurpleCoinHolder::~PurpleCoinHolder() {

}