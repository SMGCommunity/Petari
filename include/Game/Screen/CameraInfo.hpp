#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CameraInfo : public LayoutActor {
public:
    /// @brief Creates a new `CameraInfo`.
    /// @param pName The pointer to the null-terminated name of the object.
    CameraInfo(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void movement();
    virtual void calcAnim();
    virtual void draw() const;

    void activate();
    void deactivate();
    void exeHide();
    void exeAppear();
    void exeNormal();
    void exeFirstPerson();
    void exeEnd();
    void updateCameraNG();
    void hideCrossLineAll();
    bool tryAnimShortCut();
    bool tryShow();
    bool tryHide();
    bool tryChangeCameraStateNerve();

public:
    /* 0x20 */ s32 mStep;
    /* 0x24 */ bool mIsActive;
    /* 0x25 */ bool _25;
};
