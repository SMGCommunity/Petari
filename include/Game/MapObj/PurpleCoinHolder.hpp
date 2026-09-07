#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class Coin;
class NameObj;
class PurpleCoinStarter;

class PurpleCoinHolder : public DeriveActorGroup< Coin > {
public:
    /// @brief Creates a new `PurpleCoinHolder`.
    PurpleCoinHolder();

    virtual void initAfterPlacement();

    void start();
    void registStarter(PurpleCoinStarter*);

    /* 0x18 */ PurpleCoinStarter* mStarter;
};

namespace MR {
    void createPurpleCoinHolder();
    void addToPurpleCoinHolder(const NameObj*, Coin*);
    void registPurpleCoinStarter(PurpleCoinStarter*);
};  // namespace MR
