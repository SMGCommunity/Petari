#pragma once

#include "Game/MapObj/MapObjActor.hpp"

namespace {
    const char* cDemoCameraName = "注目カメラ";
};

class CapsuleCage : public MapObjActor {
public:
    CapsuleCage(const char *);

    virtual ~CapsuleCage();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void connectToScene(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo &);

    void exeStartCamera();
    inline void exeEndCamera();
    void exeOpen();
    void startOpen();

    ActorCameraInfo* mInfo;     // 0xC4
};

namespace NrvCapsuleCage {
    NERVE_DECL_NULL(CapsuleCageNrvWait);
    NERVE_DECL(CapsuleCageNrvStartCamera, CapsuleCage, CapsuleCage::exeStartCamera);
    NERVE_DECL(CapsuleCageNrvOpen, CapsuleCage, CapsuleCage::exeOpen);
    NERVE_DECL(CapsuleCageNrvEndCamera, CapsuleCage, CapsuleCage::exeEndCamera);
};
