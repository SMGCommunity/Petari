#pragma once

#include "Game/NameObj/NameObj.hpp"

class ImageEffectBase : public NameObj {
public:
    /// @brief Creates a new `ImageEffectBase`.
    /// @param pName A pointer to the null-terminated name of the object.
    ImageEffectBase(const char* pName);

    /* 0x1C */ virtual void calcAnim();

    /* 0x24 */ virtual void calcAnimSub() {
    }

    /* 0x28 */ virtual void notifyTurnOn() {
    }

    /* 0x2C */ virtual void notifyTurnOff() {
    }

    /* 0x30 */ virtual void notifyForceOn() {
    }

    /* 0x34 */ virtual void notifyForceOff() {
    }

    bool isSomething() const {
        return _C || _D;
    }

    f32 get_10() const {
        return _10;
    }

    /* 0x0C */ bool _C;
    /* 0x0D */ bool _D;
    /* 0x10 */ f32 _10;
};
