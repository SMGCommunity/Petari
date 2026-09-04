#pragma once

#include "Game/AudioLib/AudBgmRhythmStrategy.hpp"
#include "Game/AudioLib/AudTrackController.hpp"
#include "JSystem/JAudio2/JAISound.hpp"

class JAISoundID;
class AudBgmVolumeController;

class AudBgm {
public:
    /// @brief Creates a new `AudBgm`.
    AudBgm();

    virtual void init() = 0;
    virtual JAISoundHandle* start(u32, bool) = 0;
    virtual void stop(u32) = 0;
    virtual bool isPreparedPlay() = 0;
    virtual void playAfterPrepared() = 0;
    virtual void movement() = 0;
    virtual bool moveVolume(f32, u32) = 0;
    virtual bool moveVolumeForNoteFairy(f32, u32) = 0;
    virtual void changeTrackMuteState(s32, s32) = 0;
    virtual JAISoundHandle* getHandle() = 0;
    virtual JAISoundHandle* getRhythmHandle() = 0;
    virtual bool isSoundAttached() const = 0;
    virtual void pause(bool) = 0;
    virtual bool isStopping() const = 0;
    virtual bool isPaused() const = 0;
    virtual JAISoundID getSoundID() const = 0;
    virtual void setVolumeController(AudBgmVolumeController* pController) {
        mVolumeController = pController;
    }
    virtual AudBgmRhythmStrategy* getRhythmStrategy() {
        return &mRhythmStrategy;
    }
    virtual void sendToSyncStream() = 0;
    virtual void rejectFromSyncStream() = 0;
    virtual void resetAuxVolume();

    static const s32 mNumTracks = 16;

    /* 0x04 */ AudBgmVolumeController* mVolumeController;
    /* 0x08 */ AudBgmRhythmStrategy mRhythmStrategy;
};

class AudSingleBgm : public AudBgm {
public:
    /// @brief Creates a new `AudSingleBgm`.
    AudSingleBgm();

    virtual void init();
    virtual JAISoundHandle* start(u32, bool);
    virtual void stop(u32);
    virtual bool isPreparedPlay();
    virtual void playAfterPrepared();
    virtual void movement();
    virtual bool moveVolume(f32, u32);
    virtual bool moveVolumeForNoteFairy(f32, u32);
    virtual void changeTrackMuteState(s32, s32);
    virtual JAISoundHandle* getHandle() {
        return &mHandle;
    }
    virtual JAISoundHandle* getRhythmHandle();
    virtual bool isSoundAttached() const {
        return mHandle.isSoundAttached();
    }
    virtual void pause(bool pause) {
        if (mHandle.isSoundAttached()) {
            mHandle->pause(pause);
        }
    }
    virtual bool isStopping() const {
        if (mHandle.isSoundAttached()) {
            return mHandle->isStopping();
        }
        return true;
    }
    virtual bool isPaused() const {
        if (mHandle.isSoundAttached()) {
            return mHandle->isPaused();
        }
        return false;
    }
    virtual JAISoundID getSoundID() const {
        // FIXME: register again.
        if (!mHandle.isSoundAttached()) {
            return 0;
        }
        return mSoundID;
    }
    virtual void sendToSyncStream() {
    }
    virtual void rejectFromSyncStream() {
    }

    void initTrackController();
    void startTrackControl();
    void stopTrackControl();
    void updateTrackControl();

    /* 0x14 */ JAISoundHandle mHandle;
    /* 0x18 */ JAISoundID mSoundID;
    /* 0x1C */ AudTrackController mTrackController[mNumTracks];
};

class AudMultiBgm : public AudBgm {
public:
    /// @brief Creates a new `AudMultiBgm`.
    AudMultiBgm();

    virtual void init();
    virtual JAISoundHandle* start(u32, bool);
    virtual void stop(u32);
    virtual bool isPreparedPlay();
    virtual void playAfterPrepared();
    virtual void movement();
    virtual bool moveVolume(f32, u32);
    virtual bool moveVolumeForNoteFairy(f32, u32);
    virtual void changeTrackMuteState(s32, s32);
    virtual JAISoundHandle* getHandle();
    virtual JAISoundHandle* getRhythmHandle();
    virtual bool isSoundAttached() const;
    virtual void pause(bool);
    virtual bool isStopping() const;
    virtual bool isPaused() const;
    virtual JAISoundID getSoundID() const;
    virtual void sendToSyncStream();
    virtual void rejectFromSyncStream();

    void initTrackController();
    void startTrackControl();
    void updateTrackControl();
    JAISoundHandle* prepare(u32);
    bool isPrepared();
    void unlock();
    void updateSyncProcess();
    void pauseSyncProcess();
    void setStreamVolume(f32, f32);

    static const s32 mNumFaders = 2;

    /* 0x014 */ JAISoundHandle mHandle;
    /* 0x018 */ JAISoundHandle mRhythmHandle;
    /* 0x01C */ AudTrackController mTrackController[mNumTracks];
    /* 0x1DC */ AudFader mFader[mNumFaders];
    /* 0x1F4 */ s32 _1F4;  // mSyncState
    /* 0x1F8 */ s32 mBgmId;
    /* 0x1FC */ bool mIsLocked;  // TODO: better name?
};
