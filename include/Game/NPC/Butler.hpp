#pragma once

#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "revolution/types.h"

class Butler : public NPCActor {
public:
    Butler(const char*);

    virtual ~Butler();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void killIfBatlerMapAppear();
    void startDemoButlerReport(const char*);
    void startDemoDomeLecture1();
    void startDemoDomeLecture2();
    void startDemoStarPiece1();
    void startDemoStarPiece2();
    void tryStartShowGalaxyMap();
    void resetStatus();
    bool messageBranchFunc(u32);
    void initTalkCtrlArray(const JMapInfoIter&);
    void initForAstroDome(const JMapInfoIter&);
    void initForAstroGalaxy(const JMapInfoIter&);
    TalkMessageCtrl* createTalkCtrl(const JMapInfoIter&, const char*) NO_INLINE;
    void forceNerveToWait();
    void tryReplaceStarPieceIfExecLecture();
    bool tryStartStarPieceReaction();
    void exeStarPieceReaction();
    void exeDemo();
    void exeDemoDomeLecture2();
    void exeDemoStarPiece2();
    void exeDemoShowGalaxyMap();
    inline void exeDemoWait();

    TalkMessageCtrl** mTalkMessage;  // 15C
    bool _160;
    s32 _164;
    s32 _168;
    ButlerStateStarPieceReaction* mButlerState;  // 16C
    bool _170;
    bool _171;
};
