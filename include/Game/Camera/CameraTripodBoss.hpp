#pragma once

#include "Game/Camera/CameraTower.h"

class CameraTripodBoss : public CameraTower {
public:
    CameraTripodBoss(const char *);
    virtual ~CameraTripodBoss();

    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void arrangeRound();

    f32 upZ;    // _8C
};