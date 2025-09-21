#pragma once

#include "Game/NameObj/NameObj.hpp"

class JUTTexture;
class ResTIMG;

struct TimingInfo {
    /* 0x0 */ const char* mName;
    /* 0x0 */ s32 mTiming;
    /* 0x0 */ bool _8;
    /* 0x0 */ u32 _C;
};

class CaptureScreenActor : public NameObj {
public:
    /// @brief Creates a new `CaptureScreenActor`.
    /// @param param1 TODO.
    /// @param pCameraName TODO.
    CaptureScreenActor(u32 param1, const char* pCameraName);

    /// @brief Destroys the `CaptureScreenActor`.
    virtual ~CaptureScreenActor() {}

    /// @brief Draws the `CaptureScreenActor` to the screen.
    virtual void draw() const;

private:
    /* 0xC */ const char* mCameraName;
};

class CaptureScreenDirector : public NameObj {
public:
    /// @brief Creates a new `CaptureScreenDirector`.
    CaptureScreenDirector();

    /// @brief Destroys the `CaptureScreenDirector`.
    virtual ~CaptureScreenDirector() {}

    void captureIfAllow(const char* pName);
    void capture();
    void requestCaptureTiming(const char* pName);
    void invalidateCaptureTiming(const char* pName);
    const ResTIMG* getResTIMG() const;
    const u8* getTexImage() const;
    const TimingInfo* getUsingTiming() const;
    const TimingInfo* getCurrentTiming() const;
    const TimingInfo* findFromName(const char* pName) const;

private:
    /* 0x0C */ const char* _C;
    /* 0x10 */ const char* mTimingType;
    /* 0x14 */ JUTTexture* mTexture;
    /* 0x18 */ bool _18;
};
