#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaDemoPowerUp : public ActorStateBase< Koopa > {
public:
    KoopaDemoPowerUp(Koopa*);

    virtual ~KoopaDemoPowerUp();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeWaitDemo();
    void exeDemo();
};
