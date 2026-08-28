#pragma once

#include "Game/AudioLib/AudBgmRhythmStrategy.hpp"
#include "Game/AudioLib/AudTrackController.hpp"

class JAISoundID;
class AudBgmVolumeController;

class AudBgm {
public:
    /// @brief Creates a new `AudBgm`.
    AudBgm();

    /* 0x08 */ virtual void init() = 0;
    /* 0x0C */ virtual JAISoundHandle* start(u32, bool) = 0;
    /* 0x10 */ virtual void stop(u32) = 0;
    /* 0x14 */ virtual bool isPreparedPlay() = 0;
    /* 0x18 */ virtual void playAfterPrepared() = 0;
    /* 0x1C */ virtual void movement() = 0;
    /* 0x20 */ virtual void moveVolume(f32, u32) = 0;
    /* 0x24 */ virtual void moveVolumeForNoteFairy(f32, u32) = 0;
    /* 0x28 */ virtual void changeTrackMuteState(s32, s32) = 0;
    /* 0x2C */ virtual JAISoundHandle* getHandle() = 0;
    /* 0x30 */ virtual JAISoundHandle* getRhythmHandle() = 0;
    /* 0x34 */ virtual bool isSoundAttached() const = 0;
    /* 0x38 */ virtual void pause(bool) = 0;
    /* 0x3C */ virtual bool isStopping() const = 0;
    /* 0x40 */ virtual bool isPaused() const = 0;
    /* 0x44 */ virtual JAISoundID getSoundID() const = 0;
    /* 0x48 */ virtual void setVolumeController(AudBgmVolumeController*);
    /* 0x4C */ virtual AudBgmRhythmStrategy* getRhythmStrategy();
    /* 0x50 */ virtual void sendToSyncStream() = 0;
    /* 0x54 */ virtual void rejectFromSyncStream() = 0;
    /* 0x58 */ virtual void resetAuxVolume();

    /* 0x04 */ u32 _4;
    /* 0x08 */ AudBgmRhythmStrategy mRhythmStrategy;
    /* 0x014 */ JAISoundHandle* mRhythmHandle;
    /* 0x018 */ s32 _18;
    /* 0x01C */ AudTrackController mTrackController[16];
};

class AudSingleBgm : public AudBgm {
public:
    /// @brief Creates a new `AudSingleBgm`.
    AudSingleBgm();

    /* 0x08 */ virtual void init();
    /* 0x0C */ virtual JAISoundHandle* start(u32, bool);
    /* 0x10 */ virtual void stop(u32);
    /* 0x14 */ virtual bool isPreparedPlay();
    /* 0x18 */ virtual void playAfterPrepared();
    /* 0x1C */ virtual void movement();
    /* 0x20 */ virtual void moveVolume(f32, u32);
    /* 0x24 */ virtual void moveVolumeForNoteFairy(f32, u32);
    /* 0x28 */ virtual void changeTrackMuteState(s32, s32);
    /* 0x2C */ virtual JAISoundHandle* getHandle();
    /* 0x30 */ virtual JAISoundHandle* getRhythmHandle();
    /* 0x34 */ virtual bool isSoundAttached() const;
    /* 0x38 */ virtual void pause(bool);
    /* 0x3C */ virtual bool isStopping() const;
    /* 0x40 */ virtual bool isPaused() const;
    /* 0x44 */ virtual JAISoundID getSoundID() const;
    /* 0x50 */ virtual void sendToSyncStream();
    /* 0x54 */ virtual void rejectFromSyncStream();

    void initTrackController();
    void startTrackControl();
    void stopTrackControl();
    void updateTrackControl();
};

class AudMultiBgm : public AudBgm {
public:
    /// @brief Creates a new `AudMultiBgm`.
    AudMultiBgm();

    /* 0x08 */ virtual void init();
    /* 0x0C */ virtual JAISoundHandle* start(u32, bool);
    /* 0x10 */ virtual void stop(u32);
    /* 0x14 */ virtual bool isPreparedPlay();
    /* 0x18 */ virtual void playAfterPrepared();
    /* 0x1C */ virtual void movement();
    /* 0x20 */ virtual void moveVolume(f32, u32);
    /* 0x24 */ virtual void moveVolumeForNoteFairy(f32, u32);
    /* 0x28 */ virtual void changeTrackMuteState(s32, s32);
    /* 0x2C */ virtual JAISoundHandle* getHandle();
    /* 0x30 */ virtual JAISoundHandle* getRhythmHandle();
    /* 0x34 */ virtual bool isSoundAttached() const;
    /* 0x38 */ virtual void pause(bool);
    /* 0x3C */ virtual bool isStopping() const;
    /* 0x40 */ virtual bool isPaused() const;
    /* 0x44 */ virtual JAISoundID getSoundID() const;
    /* 0x50 */ virtual void sendToSyncStream();
    /* 0x54 */ virtual void rejectFromSyncStream();

    void initTrackController();
    void startTrackControl();
    void updateTrackControl();
    void* prepare(u32);
    bool isPrepared();
    void unlock();
    void updateSyncProcess();
    void pauseSyncProcess();
    void setStreamVolume(f32, f32);

    /* 0x1DC */ AudFader mFader[2];
    /* 0x1F4 */ u32 _1F4;
    /* 0x1F8 */ s32 _1F8;
    /* 0x1FC */ u8 _1FC;
};
