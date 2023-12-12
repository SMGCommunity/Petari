#pragma once

#include "Game/LiveActor/LiveActorGroup.h"

class Coin;
class NameObj;
class PurpleCoinStarter;

class PurpleCoinHolder : public DeriveActorGroup<Coin> {
public:
    PurpleCoinHolder();

    virtual ~PurpleCoinHolder();
    virtual void initAfterPlacement();

    void start();

    PurpleCoinStarter* mStarter;    // _18
};

namespace MR {
    void createPurpleCoinHolder();
    void addToPurpleCoinHolder(const NameObj *, Coin *);
    void registPurpleCoinStarter(PurpleCoinStarter *);
};