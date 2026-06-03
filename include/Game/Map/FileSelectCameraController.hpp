#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FileSelectCameraController : public LiveActor {
public:
    /// @brief Creates a new `FileSelectCameraController`.
    /// @param pName A pointer to the null-terminated name of the object.
    FileSelectCameraController(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void goToFarPoint();
    void goToNearPoint(const TVec3f&);
    bool isAtFarPoint() const;
    bool isAtNearPoint() const;
    bool isToOrAtFarPoint() const;
    bool isToOrAtNearPoint() const;

    void exeTitle();
    void exeMoveToFarPoint();
    void exeFarPoint();
    void exeMoveToNearPoint();
    void exeNearPoint();

private:
    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ u8 _98[0xC];
    /* 0xA4 */ TVec3f _A4;
    /* 0xB0 */ TVec3f _B0;
    /* 0xBC */ f32 mPrevFovy;
    /* 0xC0 */ f32 mFovy;
    /* 0xC4 */ TVec3f _C4;
};
