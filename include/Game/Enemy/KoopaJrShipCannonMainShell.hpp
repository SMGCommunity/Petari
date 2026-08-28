#pragma once

#include "Game/Enemy/CannonShellBase.hpp"

class KoopaJrShipCannonMainShell : public CannonShellBase {
public:
    KoopaJrShipCannonMainShell(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    /* 0x78 */ virtual void launch(const TVec3f&, const TVec3f&);
    virtual f32 getBaseScale() const;

    void explosion();

    void exeWait();
    void exeFly();

    TQuat4f _8C;
};

namespace CannonShellUtil {
    void registerKoopaJrShipCannonMainShell(CannonShellHolder* pHolder, int shellNum);
};  // namespace CannonShellUtil
