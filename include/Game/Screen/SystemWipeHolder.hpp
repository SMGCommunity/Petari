#pragma once

#include "Game/Screen/WipeHolderBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SystemWipeHolder : public WipeHolderBase {
public:
    /// @brief Creates a new `SystemWipeHolder`.
    SystemWipeHolder();

    /// @brief Intializes the `SystemWipeHolder` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    /* 0x14 */ virtual void movement();

    /// @brief Draws the `SystemWipeHolder` to the screen.
    /* 0x18 */ virtual void draw() const;

    /* 0x1C */ virtual void calcAnim();
    /* 0x50 */ virtual void updateWipe(const char*);

    void setWipeRingCenter(const TVec3f&);
    void startGameScreenCapture();
    void drawGameScreenCapture() const;

    bool isCurrentAlive() const;

private:
    /* 0x1C */ bool _1C;
};

namespace MR {
    SystemWipeHolder* createSystemWipeHolder();
};  // namespace MR
