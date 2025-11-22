#pragma once

#include "Game/Screen/SimpleLayout.hpp"
#include "Game/System/NerveExecutor.hpp"

class EncouragePal60Window;
class TriggerChecker;

namespace TitleSequenceProductSub {
    class LogoLayout;
};

class TitleSequenceProduct : public NerveExecutor {
public:
    /// @brief Creates a new `TitleSequenceProduct`.
    TitleSequenceProduct();

    void appear();
    void kill();
    bool isActive() const;
    void updateButtonReaction(TriggerChecker* pButtonChecker, const char* pAnimName);
    void updatePressStartReaction();
    void exeDisplayEncouragePal60Window();
    void exeBgmPrepare();
    void exeLogoFadein();
    void exeLogoWait();
    void exeLogoDisplay();
    void exeDecide();
    void exeDead();

private:
    /* 0x08 */ TitleSequenceProductSub::LogoLayout* mLogoLayout;
    /* 0x0C */ SimpleLayout*                        mPressStartLayout;
    /* 0x10 */ EncouragePal60Window*                mEncouragePal60Window;
    /* 0x14 */ TriggerChecker*                      mAButtonChecker;
    /* 0x18 */ TriggerChecker*                      mBButtonChecker;
    /* 0x1C */ bool                                 mIsDisplayEncouragePal60Window;
};

namespace TitleSequenceProductSub {
    class LogoLayout : public SimpleLayout {
    public:
        /// @brief Creates a new `LogoLayout`.
        LogoLayout();
    };
}; // namespace TitleSequenceProductSub
