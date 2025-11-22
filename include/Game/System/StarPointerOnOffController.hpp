#pragma once

#include "Game/System/NerveExecutor.hpp"

enum StarPointerModeEnum {
    StarPointerMode_HomeButton = 0,
    StarPointerMode_ErrorWindow = 1,
    StarPointerMode_SaveLoad = 2,
    StarPointerMode_Logo = 3,
    StarPointerMode_Title = 4,
    StarPointerMode_FileSelect = 5,
    StarPointerMode_ScenarioSelectScene = 6,
    StarPointerMode_PauseMenu = 7,
    StarPointerMode_ChooseYesNo = 8,
    StarPointerMode_PictureBook = 9,
    StarPointerMode_DemoWithStarPointer = 10,
    StarPointerMode_DemoWithHandPointerFinger = 11,
    StarPointerMode_Demo = 12,
    StarPointerMode_DemoMarioDeath = 13,
    StarPointerMode_PowerStarGetDemo = 14,
    StarPointerMode_1PInvalid2PValid = 15,
    StarPointerMode_SphereSelectorOnReaction = 16,
    StarPointerMode_SphereSelectorFinger = 17,
    StarPointerMode_MarioLauncher = 18,
    StarPointerMode_CommandStream = 19,
    StarPointerMode_BlueStar = 20,
    StarPointerMode_BlueStarReady = 21,
    StarPointerMode_StarPieceTarget = 22,
    StarPointerMode_BigBubble = 23,
    StarPointerMode_Base = 24,
    StarPointerMode_Game = 25,

    StarPointerMode_Count,
};

class StarPointerOnOffController : public NerveExecutor {
public:
    /// @brief Creates a new `StarPointerOnOffController`.
    StarPointerOnOffController();

    void update();
    void setStateToTitle(void*);
    void setStateToFileSelect(void*);
    void setStateToBase(void*);
    void popState(void*);
    void requestMode(void*, StarPointerModeEnum);
    void setupHomeButton();
    void setupErrorWindow();
    void setupLogo();
    void setupTitle();
    void setupFileSelect();
    void setupPictureBook();
    void setupDemo();
    void setupDemoWithStarPointer();
    void setupDemoWithHandPointerFinger();
    void setupDemoMarioDeath();
    void setup1PInvalid2PValid();
    void setupMarioLauncher();
    void setupChooseYesNo();
    void setupPauseMenu();
    void setupScenarioSelectScene();
    void setupBlueStar();
    void setupBlueStarReady();
    void setupPowerStarGetDemo();
    void setupStarPieceTarget();
    void setupSphereSelectorFinger();
    void setupSphereSelectorOnReaction();
    void setupBigBubble();
    void setupCommandStream();
    void setupGame();
    void setupBase();
    void updateMode();
    void clearInfo();
    void incModeCounter(void*, s32);
    void decModeCounter(void*);

    /* 0x08 */ bool _8;
    /* 0x0C */ void* _C[16];
    /* 0x4C */ u32 _4C[StarPointerMode_Count];
    /* 0xB4 */ bool _B4[StarPointerMode_Count];
    /* 0xD0 */ s32 _D0;
};
