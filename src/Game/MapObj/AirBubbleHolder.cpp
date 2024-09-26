#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/MapObj/AirBubble.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

AirBubbleHolder::AirBubbleHolder(const char *pName) : LiveActorGroup(pName, 64) { }

#ifdef NON_MATCHING // AirBubble isn't finished so the size is wrong
void AirBubbleHolder::init(const JMapInfoIter &rIter) {
    for (s32 i = 0; i < 32; i++) {
        AirBubble* bubble = new AirBubble("空気アワ(共用)");
        bubble->initWithoutIter();
        bubble->makeActorAppeared();
        registerActor(bubble);
    }
}
#endif

void AirBubbleHolder::appearAirBubble(const TVec3f &a1, s32 a2) {
    AirBubble* bubble;
    if (getDeadActor()) {
        bubble = reinterpret_cast<AirBubble*>(getDeadActor());
    }
    else {
        bubble = nullptr;
    }

    if (bubble) {
        bubble->appearMove(a1, a2);
    }
}

namespace MR {
    AirBubbleHolder* createAirBubbleHolder() {
        return reinterpret_cast<AirBubbleHolder*>(MR::createSceneObj(SceneObj_AirBubbleHolder));
    }

    void appearAirBubble(const TVec3f &a1, s32 a2) {
        reinterpret_cast<AirBubbleHolder*>(MR::getSceneObjHolder()->getObj(SceneObj_AirBubbleHolder))->appearAirBubble(a1, a2);
    }
};