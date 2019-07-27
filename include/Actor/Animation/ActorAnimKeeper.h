#ifndef ACTORANIMKEEPER_H
#define ACTORANIMKEEPER_H

class LiveActor;

class ActorAnimKeeper
{
public:
    ActorAnimKeeper(LiveActor *);

    static ActorAnimKeeper* tryCreate(LiveActor *);
    void start(const char *);
    void update();
    bool isPlaying(const char *) const;
    void initAnimData();
};

#endif // ACTORANIMKEEPER_H