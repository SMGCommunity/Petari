#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"

namespace KoopaFunction {
    s32 registerBattleMapStair(KoopaBattleMapStair*);
    void startRecoverKoopaArmor(Koopa*);
    void startRecoverKoopaTailThorn(Koopa*);
    void endFaceCtrl(Koopa*, s32);
    void initKoopaCamera(Koopa*, const char*);
    void endKoopaCamera(Koopa*, const char*, bool, s32);
    void setKoopaPos(Koopa*, const char*);
    void startKoopaTargetCamera(Koopa*, const char*);
    bool isKoopaVs1(const Koopa*);
    bool isKoopaVs2(const Koopa*);
    bool isKoopaLv3(const Koopa*);
    bool isKoopaVs3(const Koopa*);
    LiveActor* getKoopaPowerUpSwitch(Koopa*);
};  // namespace KoopaFunction
