#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeCameraController;
class GalaxyConfirmLayout;
class GalaxySelectBackButton;
class GalaxySelectInfo;
class MiniatureGalaxy;

class AstroDomeGalaxySelector : public LiveActor {
public:
    AstroDomeGalaxySelector(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void showGalaxyInfo(const MiniatureGalaxy*);
    bool tryStartLectureDemo(const Nerve*);

    void exeGalaxySelectStart();
    void exeGalaxySelect();
    void exeGalaxySelectCancel();
    void exeGalaxyConfirmStart();
    void exeGalaxyConfirm();
    void exeGalaxyConfirmCancel();
    void exeDemoJumpOut();
    void exeDemoDomeLecture();
    void exeGalaxyMoveLecture();
    void exeWaitStartDemo();

    AstroDomeCameraController* pDomeCamCtrl;  // _8C
    GalaxySelectBackButton* pGSBackButton;    // _90
    GalaxySelectInfo* pGSInfo;                // _94
    GalaxyConfirmLayout* pGConfirmLayout;     // _98
    bool _9C;
};
