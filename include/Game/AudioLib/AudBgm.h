#pragma once

class AudBgm {
public:
    virtual void init() = 0;
    virtual void start(u32, bool) = 0;
    virtual void stop(u32) = 0;
    virtual bool isPreparedPlay() = 0;
    virtual void playAfterPrepared() = 0;
    virtual void movement() = 0;
    virtual void moveVolume(f32, u32) = 0;
    virtual void moveVolumeForNateFairy(f32, u32) = 0;
    virtual void changeTrackMuteState(s32, s32) = 0;
    virtual u32* getHandle() = 0;
    virtual u32* getRhythmHandle() = 0;
    virtual bool isSoundAttached() const = 0;
    virtual void pause(bool) = 0;
};