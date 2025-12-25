#pragma once

#include "Game/AudioLib/AudBgmRhythmStrategy.hpp"
#include "Game/AudioLib/AudTrackController.hpp"

class JAISoundID;

class AudBgm {
public:
    /// @brief Creates a new `AudBgm`.
    AudBgm();

    virtual void init() = 0;
    virtual void start(u32, bool) = 0;
    virtual void stop(u32) = 0;
    virtual bool isPreparedPlay() = 0;
    virtual void playAfterPrepared() = 0;
    virtual void movement() = 0;
    virtual void moveVolume(f32, u32) = 0;
    virtual void moveVolumeForNoteFairy(f32, u32) = 0;
    virtual void changeTrackMuteState(s32, s32) = 0;
    virtual u32* getHandle() = 0;
    virtual u32* getRhythmHandle() = 0;
    virtual bool isSoundAttached() const = 0;
    virtual void pause(bool) = 0;
    virtual bool isStopping() const = 0;
    virtual bool isPaused() const = 0;
    virtual JAISoundID getSoundID() const = 0;
    virtual void setVolumeController();
    virtual AudBgmRhythmStrategy* getRhythmStrategy();
    virtual void sendToSyncStream() = 0;
    virtual void rejectFromSyncStream() = 0;
    virtual void resetAuxVolume();

    /* 0x04 */ u32 _4;
    /* 0x08 */ AudBgmRhythmStrategy mRhythmStrategy;
};

class AudSingleBgm : public AudBgm {
public:
    /// @brief Creates a new `AudSingleBgm`.
    AudSingleBgm();

    virtual void init();
    virtual void start(u32, bool);
    virtual void stop(u32);
    virtual bool isPreparedPlay();
    virtual void playAfterPrepared();
    virtual void movement();
    virtual void moveVolume(f32, u32);
    virtual void moveVolumeForNoteFairy(f32, u32);
    virtual void changeTrackMuteState(s32, s32);
    virtual u32* getHandle();
    virtual u32* getRhythmHandle();
    virtual bool isSoundAttached() const;
    virtual void pause(bool);
    virtual bool isStopping() const;
    virtual bool isPaused() const;
    virtual JAISoundID getSoundID() const;
    virtual void sendToSyncStream();
    virtual void rejectFromSyncStream();

    void initTrackController();
    void startTrackControl();
    void stopTrackControl();
    void updateTrackControl();

    /* 0x14 */ u32 _14;
    /* 0x18 */ s32 _18;
    /* 0x1C */ AudTrackController mTrackController[16];
};

class AudMultiBgm : public AudBgm {
public:
    /// @brief Creates a new `AudMultiBgm`.
    AudMultiBgm();

    virtual void init();
    virtual void start(u32, bool);
    virtual void stop(u32);
    virtual bool isPreparedPlay();
    virtual void playAfterPrepared();
    virtual void movement();
    virtual void moveVolume(f32, u32);
    virtual void moveVolumeForNoteFairy(f32, u32);
    virtual void changeTrackMuteState(s32, s32);
    virtual u32* getHandle();
    virtual u32* getRhythmHandle();
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
    void* prepare(u32);
    bool isPrepared();
    void unlock();
    void updateSyncProcess();
    void pauseSyncProcess();
    void setStreamVolume(f32, f32);

    /* 0x014 */ u32 _14;
    /* 0x018 */ s32 _18;
    /* 0x01C */ AudTrackController mTrackController[16];
    /* 0x1DC */ AudFader mFader[2];
    /* 0x1F4 */ u32 _1F4;
    /* 0x1F8 */ s32 _1F8;
    /* 0x1FC */ u8 _1FC;
};
