#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class Koopa;
class LiveActor;

class BossAccessor : public NameObj {
public:
    /// @brief Creates a new `BossAccessor`
    BossAccessor();

    /// @brief Destroys the `BossAccessor`.
    virtual ~BossAccessor();

    LiveActor* getBoss() const { return mBoss; }

    void setBoss(LiveActor* pBoss) { mBoss = pBoss; }

private:
    /* 0x0 */ LiveActor* mBoss;
};

namespace BossAccess {
    Koopa* getBossAccessorKoopa();
    void setBossAccessorKoopa(Koopa* pKoopa);
};  // namespace BossAccess
