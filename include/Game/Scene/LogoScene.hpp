#pragma once

#include "Game/Scene/Scene.hpp"

class IsbnManager;
class LayoutActor;
class LogoFader;
class SimpleLayout;

class LogoScene : public Scene {
public:
    /// @brief Creates a new `LogoScene`.
    LogoScene();

    /// @brief Destroys the `LogoScene`.
    /* 0x08 */ virtual ~LogoScene();

    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x1C */ virtual void calcAnim();
    /* 0x18 */ virtual void draw() const;

    bool isDisplayStrapRemineder() const;
    void exeCensorshipFadein();
    void exeCensorshipDisplay();
    void exeCensorshipFadeout();
    void exeStrapFadein();
    void exeStrapDisplay();
    void exeStrapFadeout();
    void exeMountGameData();
    void exeWaitReadDoneSystemArchive();
    void exeDeactive();
    void initLayout();
    bool tryFadeinLayout(LayoutActor* pActor);
    bool tryFadeoutLayout(LayoutActor* pActor);
    bool tryFadeinLayout();
    bool tryFadeoutLayout();

private:
    /* 0x14 */ IsbnManager* mIsbnManager;
    /* 0x18 */ SimpleLayout* mStrapLayout;
    /* 0x1C */ LogoFader* mLogoFader;
};
