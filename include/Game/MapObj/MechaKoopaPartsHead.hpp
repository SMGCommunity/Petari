#pragma once

#include "Game/MapObj/MapObjActor.h"

namespace {
    const char* sDemoPartNameBreak = "壊れ開始";
    const char* sDemoPartNameFadeOut = "フェードアウト";
    const char* sDemoPartNameWhite = "白画面";
    const char* sDemoPartNameFadeIn = "フェードイン";
};

class MechaKoopaPartsHead : public MapObjActor {
public:
    MechaKoopaPartsHead(const char *);

    virtual ~MechaKoopaPartsHead();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);

    void exeDemoBreak();
    void exeDemoWhiteFadeOut();
    void exeDemoWhiteWait();
    void exeDemoWhiteFadeIn();
    void exeDemoAppearStar();
};

namespace NrvMechaKoopaPartsHead {
    NERVE(MechaKoopaPartsHeadNrvWait);
    NERVE(MechaKoopaPartsHeadNrvDemoBreak);
    NERVE(MechaKoopaPartsHeadNrvDemoWhiteFadeOut);
    NERVE(MechaKoopaPartsHeadNrvDemoWhiteWait);
    NERVE(MechaKoopaPartsHeadNrvDemoWhiteFadeIn);
    NERVE(MechaKoopaPartsHeadNrvDemoAppearStar);
};
