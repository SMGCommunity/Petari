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
    /* 0x8C */ TVec3f mNearTarget;
    /* 0x98 */ TVec3f mPrevNearTarget;
    /* 0xA4 */ TVec3f mWPoint;
    /* 0xB0 */ TVec3f mPrevWPoint;
    /* 0xBC */ f32 mFovy;
    /* 0xC0 */ f32 mPrevFovy;
    /* 0xC4 */ TVec3f mUpVec;
};
