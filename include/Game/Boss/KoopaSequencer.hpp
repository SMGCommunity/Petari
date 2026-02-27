#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/JMapInfo.hpp"

class KoopaSequencer : public NerveExecutor {
public:
    KoopaSequencer(const char*, s32);

    virtual void init(Koopa*, const JMapInfoIter&);

    Koopa* mParent;
    KoopaDemoPowerUp* mDemoPowerUp;
    s32 _10;
    s32 _14;
    s32 _18;
    s32 _1C;
    bool _20;
};
