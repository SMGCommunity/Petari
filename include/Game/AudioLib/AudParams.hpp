#pragma once

#include <revolution/types.h>

namespace AudParams {
    static const f32 scCtgVolume[][16] = {
        {1.2f, 1.2f, 1.2f, 1.3f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 1.2f, 1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.8f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.8f, 0.8f, 1.3f, 0.6f, 0.6f, 0.6f, 1.2f, 0.6f, 0.6f, 1.2f, 0.6f, 0.6f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 1.5f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
    };

    int numFreeSe = 120;
    int numFreeSeq = 3;
    int numFreeStream = 3;
    int numFreeSoundChild = 160;
    void* aramAddr = nullptr;
    int aramSize = 0xE00000;
    int workAreaSize = 0x1000;
    int audioThreadPriority = 2;
    int dvdThreadPriority = 5;
    int dvdThreadSwitchID = -1;
    int audioThreadSwitchID = -1;
    int numFreeTracks = 200;
    int numFreeTrackChannels = 1;
    int streamBlockSize = 0x2760;
    int numStreamChannels = 4;
    f32 outputGain = 0.9f;
    int maxSeqDataSize = 0xDAC0;
    s32 numInspectableSoundObj = 8;
    s32 masterVolBgm = 1.2f;
    s32 masterVolSe = 1.2f;
    s32 masterVolStream = 1.6f;
    // ...
    s32 streamAramBlocks_perChunk = 36;
    f32 streamSampleRate = 32000.0f;
    f32 faderVolZeroVolume = 0.0f;
    f32 faderVolDownVolume = 0.3333f;
    f32 faderInterruptedVolume = 0.0f;
    s32 faderVolZeroTime = 5;
    s32 faderVolDownTime = 3;
    s32 faderInterruptedTime = 5;
    s32 faderVolZeroRecoverTime = 5;
    s32 faderVolDownRecoverTime = 20;
    s32 faderInterruptedRecoverTime = 60;
    f32 micFarDistance = 7000.0f;
    f32 micNearDistance = 600.0f;
    f32 micMinVolume = 0.0f;
    f32 micFrontDolbyZ = -1000.0f;
    f32 micBehindDolbyZ = 6000.0f;
    f32 micMaxDolby = 1.0f;
    f32 micMinDolby = 0.0f;
    f32 micCenterDolby = 0.85f;
    f32 micPanAmplitude = 0.5f;
    f32 micSonicSpeed = 800.0f;
    f32 micPitchDeltaRatioRange = 1.5f;
    s32 micPriorityDownRange = 255;
};
