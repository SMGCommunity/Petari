#pragma once

#include "Game/Enemy/CannonShellBase.hpp"

class KoopaJrShipCannonMainShell : public CannonShellBase {
public:
    KoopaJrShipCannonMainShell(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    virtual void launch(const TVec3f&, const TVec3f&);
    virtual f32 getBaseScale() const;

    void explosion();

    void exeWait();
    void exeFly();

    TQuat4f _8C;
};

namespace CannonShellUtil {
    void registerKoopaJrShipCannonMainShell(CannonShellHolder* pHolder, int shellNum);
};  // namespace CannonShellUtil
