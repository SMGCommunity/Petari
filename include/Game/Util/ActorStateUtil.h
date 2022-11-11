#pragma once

class LiveActor;
class ActorStateBaseInterface;

namespace MR {
    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
};