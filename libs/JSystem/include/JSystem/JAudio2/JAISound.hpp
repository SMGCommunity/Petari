#pragma once

#include <revolution/types.h>

class JAISound;

class JAISoundID {
public:
    /// @brief Creates a new `JAISoundID`.
    JAISoundID() {
    }

    JAISoundID(u32 id) : mID(id) {
    }

    JAISoundID(const JAISoundID& rOther) : mID(rOther.mID) {
    }

    operator u32() const {
        return mID;
    }

    bool isAnonymous() const {
        return mID == -1;
    }

    void setAnonymous() {
        mID = -1;
    }

private:
    /* 0x00 */ u32 mID;
};

class JAISoundHandle {
public:
    /// @brief Creates a new `JAISoundHandle`.
    JAISoundHandle() : mSound(nullptr) {
    }

    /// @brief Destroys the `JAISoundHandle`.
    ~JAISoundHandle() {
        releaseSound();
    }

    JAISound* operator->() const {
        return mSound;
    }

    JAISound* getSound() {
        return mSound;
    }

    bool isSoundAttached() const {
        return mSound != nullptr;
    }

    void releaseSound();

private:
    /* 0x00 */ JAISound* mSound;
};

class JAISound {
public:
    /// @brief Creates a new `JAISound`.
    JAISound();

    void updateLifeTime(u32);
};
