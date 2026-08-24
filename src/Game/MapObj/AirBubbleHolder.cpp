#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/MapObj/AirBubble.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static const s32 sAirBubbleMax = 64;
    static const s32 sCreateAirBubbleNum = 32;
};  // namespace

AirBubbleHolder::AirBubbleHolder(const char* pName) : LiveActorGroup(pName, ::sAirBubbleMax) {
}

void AirBubbleHolder::init(const JMapInfoIter& rIter) {
    AirBubble* airBubble;

    for (s32 i = 0; i < ::sCreateAirBubbleNum; i++) {
        airBubble = new AirBubble("空気アワ(共用)");
        airBubble->initWithoutIter();
        airBubble->makeActorDead();

        registerActor(airBubble);
    }
}

void AirBubbleHolder::appearAirBubble(const TVec3f& rTrans, s32 lifeTime) {
    AirBubble* airBubble;

    if (getDeadActor() != nullptr) {
        airBubble = static_cast< AirBubble* >(getDeadActor());
    } else {
        airBubble = nullptr;
    }

    if (airBubble != nullptr) {
        airBubble->appearMove(rTrans, lifeTime);
    }
}

namespace MR {
    AirBubbleHolder* createAirBubbleHolder() {
        return static_cast< AirBubbleHolder* >(MR::createSceneObj(SceneObj_AirBubbleHolder));
    }

    void appearAirBubble(const TVec3f& rTrans, s32 lifeTime) {
        MR::getSceneObj< AirBubbleHolder >(SceneObj_AirBubbleHolder)->appearAirBubble(rTrans, lifeTime);
    }
};  // namespace MR
