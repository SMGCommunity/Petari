#pragma once

#include "Game/NPC/NPCActor.hpp"

class PenguinMaster : public NPCActor {
public:
    /// @brief Creates a new `PenguinMaster`.
    /// @param pName A pointer to the null-terminated name of the object.
    PenguinMaster(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeReaction();
};
