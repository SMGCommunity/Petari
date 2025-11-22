#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/NPCUtil.hpp"

class ButlerMap : public NPCActor {
public:
    ButlerMap(const char*);

    virtual ~ButlerMap();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void startLectureDemo();
    void resetStatus();
    void forceNerveToWait();
    void exeTalk();
    void exeShowGalaxyMap();
    void exeLectureDemoShowMapBefore();
    void exeLectureDemoShowMap();
    void exeLectureDemoShowMapAfter();
    void exeStarPieceReaction();

    ButlerStateStarPieceReaction* _15C;
    bool _160;
};
