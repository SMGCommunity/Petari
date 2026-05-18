#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution/types.h>

class LiveActor;
class Koopa;
class KoopaBattleMapStair;

namespace KoopaFunction {
    s32 registerBattleMapStair(KoopaBattleMapStair*);
    s32 registerKoopaPowerUpSwitch(LiveActor*);
    s32 registerKoopaSwitchKeeper(LiveActor*);
    s32 registerKoopaViewSwitchKeeper(LiveActor*);

    void initKoopaCamera(Koopa*, const char*);
    void startKoopaTargetCamera(Koopa*, const char*);
    void endKoopaCamera(Koopa*, const char*, bool, s32);
    LiveActor* getKoopaPowerUpSwitch(Koopa*);

    void setKoopaPos(Koopa*, const char*);

    void startRecoverKoopaArmor(Koopa*);
    void startRecoverKoopaTailThorn(Koopa*);
    void endFaceCtrl(Koopa*, s32);
    void endFaceCtrlDirect(Koopa*);

    TVec3f* getKoopaFrontPtr(Koopa*);

    bool tryRestartKoopa();

    bool isKoopaVs1(const Koopa*);
    bool isKoopaVs2(const Koopa*);
    bool isKoopaVs3(const Koopa*);
    bool isKoopaLv1(const Koopa*);
    bool isKoopaLv2(const Koopa*);
    bool isKoopaLv3(const Koopa*);
};  // namespace KoopaFunction
