#pragma once

#include "Game/Scene/Scene.hpp"

class IntermissionScene : public Scene {
public:
    IntermissionScene();

    virtual void update();
    virtual void draw() const;

    void setCurrentSceneControllerState(const char*, ...);

    char mState[0x40];  // 0x14
    u32 _54;
};
