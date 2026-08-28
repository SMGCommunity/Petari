#pragma once

#include "Game/NPC/NPCActor.hpp"

class ButlerStateStarPieceReaction;

class ButlerMap : public NPCActor {
public:
    /// @brief Creates a new `ButlerMap`.
    /// @param pName A pointer to the null-terminated name of the object.
    ButlerMap(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void startLectureDemo();
    void resetStatus();
    void forceNerveToWait();

    void exeTalk();
    void exeShowGalaxyMap();
    void exeLectureDemoShowMapBefore();
    void exeLectureDemoShowMap();
    void exeLectureDemoShowMapAfter();
    void exeStarPieceReaction();

    /* 0x15C */ ButlerStateStarPieceReaction* mStateStarPieceReaction;
    /* 0x160 */ bool mObjArg0;
};
