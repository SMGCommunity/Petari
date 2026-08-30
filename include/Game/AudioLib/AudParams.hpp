#pragma once

#include <JSystem/JAudio2/JAISeMgr.hpp>

namespace AudParams {
    extern const f32 scCtgVolume[][16];
    extern const int numFreeSe;
    extern const int numFreeSeq;
    extern const int numFreeStream;
    extern const int numFreeSoundChild;
    extern const int aramAddr;
    extern const int aramSize;
    extern const int workAreaSize;
    extern const int audioThreadPriority;
    extern const int dvdThreadPriority;
    extern const int dvdThreadSwitchID;
    extern const int audioThreadSwitchID;
    extern const int numFreeTracks;
    extern const int numFreeTrackChannels;
    extern const int streamBlockSize;
    extern const int numStreamChannels;
    extern const f32 outputGain;
    extern const int maxSeqDataSize;
    extern const s32 numInspectableSoundObj;
    extern const f32 masterVolBgm;
    extern const f32 masterVolSe;
    extern const f32 masterVolStream;
    extern const JAISeCategoryArrangement arrangement;
    extern const s32 streamAramBlocks_perChunk;
    extern const f32 streamSampleRate;
    extern const f32 faderVolZeroVolume;
    extern const f32 faderVolDownVolume;
    extern const f32 faderInterruptedVolume;
    extern const s32 faderVolZeroTime;
    extern const s32 faderVolDownTime;
    extern const s32 faderInterruptedTime;
    extern const s32 faderVolZeroRecoverTime;
    extern const s32 faderVolDownRecoverTime;
    extern const s32 faderInterruptedRecoverTime;
    extern const f32 micFarDistance;
    extern const f32 micNearDistance;
    extern const f32 micMinVolume;
    extern const f32 micFrontDolbyZ;
    extern const f32 micBehindDolbyZ;
    extern const f32 micMaxDolby;
    extern const f32 micMinDolby;
    extern const f32 micCenterDolby;
    extern const f32 micPanAmplitude;
    extern const f32 micSonicSpeed;
    extern const f32 micPitchDeltaRatioRange;
    extern const s32 micPriorityDownRange;
};  // namespace AudParams
