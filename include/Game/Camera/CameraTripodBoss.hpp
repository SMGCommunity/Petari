#pragma once

#include "Game/Camera/CameraTower.hpp"

class CameraTripodBoss : public CameraTower {
public:
    CameraTripodBoss(const char* pName = "三脚ボスカメラ");
    /* 0x08 */ virtual ~CameraTripodBoss();

    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void arrangeRound();

    /* 0x8C */ f32 mAngleY;
};
