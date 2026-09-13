#pragma once

#include "Game/NameObj/NameObj.hpp"

class ShadowController;

class ShadowDrawer : public NameObj {
public:
    ShadowDrawer(const char*);

    virtual ~ShadowDrawer() {
    }

    ShadowController* getController() const;
    void setShadowController(ShadowController*);

    /* 0x0C */ ShadowController* mController;
};