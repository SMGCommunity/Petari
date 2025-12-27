#pragma once

#include "Game/Camera/CameraTower.hpp"

class CameraTripodBoss : public CameraTower {
public:
    CameraTripodBoss(const char* pName = "三脚ボスカメラ");
    virtual ~CameraTripodBoss();

    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void arrangeRound();

    f32 upZ;  // 0x8C
};