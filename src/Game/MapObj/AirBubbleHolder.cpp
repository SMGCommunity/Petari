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

void AirBubbleHolder::appearAirBubble(const TVec3f& a1, s32 a2) {
    AirBubble* airBubble;

    if (getDeadActor() != nullptr) {
        airBubble = static_cast< AirBubble* >(getDeadActor());
    } else {
        airBubble = nullptr;
    }

    if (airBubble != nullptr) {
        airBubble->appearMove(a1, a2);
    }
}

namespace MR {
    AirBubbleHolder* createAirBubbleHolder() {
        return static_cast< AirBubbleHolder* >(MR::createSceneObj(SceneObj_AirBubbleHolder));
    }

    void appearAirBubble(const TVec3f& a1, s32 a2) {
        MR::getSceneObj< AirBubbleHolder >(SceneObj_AirBubbleHolder)->appearAirBubble(a1, a2);
    }
};  // namespace MR
