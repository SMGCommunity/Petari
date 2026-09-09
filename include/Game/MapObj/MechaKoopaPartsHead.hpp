#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MechaKoopaPartsHead : public MapObjActor {
public:
    /// @brief Creates a new `MechaKoopaPartsHead`.
    /// @param pName A pointer to the null-terminated name of the object.
    MechaKoopaPartsHead(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&) {};
    virtual void initCaseNoUseSwitchA(const MapObjActorInitInfo&) {};
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {};
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&) {};

    void exeWait();
    void exeDemoBreak();
    void exeDemoWhiteFadeOut();
    void exeDemoWhiteWait();
    void exeDemoWhiteFadeIn();
    void exeDemoAppearStar();
};
