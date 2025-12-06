#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class MiniatureGalaxy : public LiveActor {
public:
    MiniatureGalaxy(const char*);
    ~MiniatureGalaxy();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initPartsModel();
    void setScale(f32);
    bool tryZoomIn(bool);
    bool tryZoomOut();
    void updateZoomInOut();
    void updateNamePlate();
    void appearUnknownModel(const char*);
    void showUnknownModel(const char*);
    void showSelectModel();
    void hideUnknownModel();
    void appearSelectModel();
    void disappearSelectModel();
    void showNamePlate(bool);
    bool isUseKoopaFaceModel() const;
    void playPointerME();
    void playNeedStartME();
    void exeWait();
    void exeNotSelected();
    void exeOpen();

    const char* _118;
};
