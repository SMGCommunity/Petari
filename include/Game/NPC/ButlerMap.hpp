#pragma once

#include "Game/NPC/NPCActor.hpp"

class ButlerStateStarPieceReaction;

class ButlerMap : public NPCActor {
public:
    /// @brief Creates a new `ButlerMap`.
    /// @param pName A pointer to the null-terminated name of the object.
    ButlerMap(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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
