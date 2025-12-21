#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AstroDomeCameraController.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/Screen/GalaxyConfirmLayout.hpp"
#include "Game/Screen/GalaxySelectBackButton.hpp"
#include "Game/Screen/GalaxySelectInfo.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class AstroDomeGalaxySelector : public LiveActor {
public:
    AstroDomeGalaxySelector(const char*);
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void showGalaxyInfo(const MiniatureGalaxy*);
    bool tryStartLectureDemo(const Nerve*);

    void exeGalaxySelect();
    void exeGalaxySelectStart();
    void exeGalaxyConfirmStart();
    void exeGalaxySelectCancel();
    void exeWaitStartDemo() {};  // Yeah this is just a return I guess?
    void exeDemoDomeLecture();
    void exeGalaxyConfirm();
    void exeGalaxyConfirmCancel();  // Auto inlined
    void exeGalaxyMoveLecture();
    void exeDemoJumpOut();

    AstroDomeCameraController* pDomeCamCtrl;  // _8C
    GalaxySelectBackButton* pGSBackButton;    // _90
    GalaxySelectInfo* pGSInfo;                // _94
    GalaxyConfirmLayout* pGConfirmLayout;     // _98
    bool _9C;
};
