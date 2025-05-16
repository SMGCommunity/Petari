#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "revolution/types.h"

class Butler : public NPCActor {
public:
    Butler(const char *);

    virtual ~Butler();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void killIfBatlerMapAppear();
    void startDemoButlerReport();
    void startDemoDomeLecture1();
    void startDemoDomeLecture2();
    void startDemoStarPiece1();
    void startDemoStarPiece2();
    bool tryStartShowGalaxyMap();
    void resetStatus();
    void messageBranchFunc(u32);
    void initTalkCtrlArray(const JMapInfoIter &);
    void initForAstroDome(const JMapInfoIter &);
    void initForAstroGalaxy(const JMapInfoIter &);
    void createTalkCtrl(const JMapInfoIter &, const char *);
    void forceNerveToWait();
    bool tryReplaceStarPieceIfExecLecture();
    bool tryStartStarPieceReaction();
    void exeStarPieceReaction();
    void exeDemo();
    void exeDemoDomeLecture2();
    void exeDemoStarPiece2();
    void exeDemoShowGalaxyMap();

    u32 _15C;
    bool _160;
    u32 _164;
    u32 _168;
    u32 _16C;
    bool _170;
    bool _171;
};
