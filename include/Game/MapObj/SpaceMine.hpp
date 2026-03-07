#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SpaceMine : public MapObjActor {
public:
    SpaceMine(const char*);

    virtual ~SpaceMine();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    bool isCalcShadowAlways() const;
    void exeWait();
    void exeAppear();

    inline bool isShadowValid(s32 val) const { return val > -1 && val < 2; }

    f32 _C4;
    TVec3f mClippingRange;  // 0xC8
    s32 _D4;                // Obj_arg0
    u8 _D8;
};
