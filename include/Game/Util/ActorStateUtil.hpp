#pragma once

class LiveActor;
class ActorStateBaseInterface;
class Nerve;

namespace MR {
    bool updateActorState(LiveActor *, ActorStateBaseInterface *);
    bool updateActorStateAndNextNerve(LiveActor *, ActorStateBaseInterface *, const Nerve *);
};
