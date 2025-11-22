#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace MR {
    class FunctorBase;
};

class OdhConverter : public LayoutActor {
public:
    /// @brief Creates a new `OdhConverter`.
    OdhConverter();

    /// @brief Intializes the `OdhConverter` while being placed into a scene.
    /// @param rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    /// @brief Draws the `OdhConverter` to the screen.
    virtual void draw() const;

    void capture() {
        convert();

        mIsRequestedCapture = false;
    }

    void requestCapture() {
        mIsRequestedCapture = true;
    }

    bool isRequestedCapture() const {
        return mIsRequestedCapture;
    }

    u8* getImage() const {
        return mImage;
    }

    u32 getImageSize() const {
        return mImageSize;
    }

    void convert();

private:
    /// @brief The width of the frame buffer, in bytes.
    /* 0x20 */ u32 mCaptureWidth;

    /// @brief The height of the frame buffer, in bytes.
    /* 0x24 */ u32 mCaptureHeight;

    /// @brief The capacity of the AJPG data buffer, in bytes.
    /* 0x28 */ u32 mLimitSize;

    /// @brief Determines if a screen capture was queued.
    /* 0x2C */ bool mIsRequestedCapture;

    /// @brief The size of the AJPG data buffer, in bytes.
    /* 0x30 */ u32 mImageSize;

    /// @brief A pointer to the AJPG data buffer.
    /* 0x34 */ u8* mImage;

    /// @brief A pointer to the frame buffer.
    /* 0x38 */ u8* mCaptureImage;
};

namespace MR {
    void            createOdhConverter();
    void            requestCaptureOdhImage();
    bool            isRequestedCaptureOdhImage();
    void            captureOdhImage();
    void            setPortCaptureOdhImage();
    const u8*       getOdhImage();
    u32             getOdhImageSize();
    NameObjAdaptor* createAdaptorAndConnectToWiiMessageBoard(const char*, const FunctorBase&);
}; // namespace MR
