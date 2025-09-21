#pragma once

#include "Game/Screen/WipeHolderBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class SystemWipeHolder : public WipeHolderBase {
public:
    /// @brief Creates a new `SystemWipeHolder`.
    SystemWipeHolder();

    /// @brief Destroys the `SystemWipeHolder`.
    virtual ~SystemWipeHolder() {}

    /// @brief Intializes the `SystemWipeHolder` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    virtual void movement();

    /// @brief Draws the `SystemWipeHolder` to the screen.
    virtual void draw() const;

    virtual void calcAnim();
    virtual void updateWipe(const char*);

    void setWipeRingCenter(const TVec3f&);
    void startGameScreenCapture();
    void drawGameScreenCapture() const;

private:
    /* 0x1C */ bool _1C;
};

namespace MR {
    SystemWipeHolder* createSystemWipeHolder();
};
