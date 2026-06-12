#pragma once

#include "Game/Enemy/CannonShellBase.hpp"

class KoopaJrShipCannonShell : public CannonShellBase {
public:
    KoopaJrShipCannonShell(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    virtual void launch(const TVec3f&, const TVec3f&);

    bool tryFreeze();
    bool isStateEnableExplosion() const;
    void explosion();
    void misfire();

    void exeWait();
    void exeFly();
    void exeDown();
    void exeFreeze();

    TQuat4f _8C;
    TVec3f _9C;
    TVec3f _A8;
    s32 _B4;  // 0xB4 : Freeze timing?
    bool _B8;
};

namespace CannonShellUtil {
    void registerKoopaJrShipCannonShell(CannonShellHolder* pHolder, int shellNum);
};  // namespace CannonShellUtil
