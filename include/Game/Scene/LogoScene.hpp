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
    virtual ~LogoScene();

    virtual void init();
    virtual void update();
    virtual void calcAnim();
    virtual void draw() const;

    bool isDisplayStrapRemineder() const;

#if (VERSION == RMGK01)
    void exeCensorshipFadein();
    void exeCensorshipDisplay();
    void exeCensorshipFadeout();
#endif

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
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
    /* 0x14 */ SimpleLayout* mStrapLayout;
    /* 0x18 */ LogoFader* mLogoFader;
#else
    /* 0x14 */ IsbnManager* mIsbnManager;
    /* 0x18 */ SimpleLayout* mStrapLayout;
    /* 0x1C */ LogoFader* mLogoFader;
#endif
};
