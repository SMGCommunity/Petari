#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class Coin;
class NameObj;
class PurpleCoinStarter;

class PurpleCoinHolder : public DeriveActorGroup< Coin > {
public:
    PurpleCoinHolder();

    /* 0x08 */ virtual ~PurpleCoinHolder();
    /* 0x10 */ virtual void initAfterPlacement();

    void start();

    PurpleCoinStarter* mStarter;  // 0x18
};

namespace MR {
    void createPurpleCoinHolder();
    void addToPurpleCoinHolder(const NameObj*, Coin*);
    void registPurpleCoinStarter(PurpleCoinStarter*);
};  // namespace MR