#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class NeedlePlant : public MapObjActor {
public:
    /// @brief Creates a new `NeedlePlant`.
    /// @param pName A pointer to the null-terminated name of the object.
    NeedlePlant(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual void connectToScene(const MapObjActorInitInfo&);

    void exeWait();
    void exeShake();

    /* 0xC4 */ s32 mObjArg0;
};
