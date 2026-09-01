#include "Game/AudioLib/AudParams.hpp"

namespace AudParams {

    const f32 scCtgVolume[][16] = {
        {1.2f, 1.2f, 1.2f, 1.3f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f, 1.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 1.2f, 1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.8f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.8f, 0.8f, 1.3f, 0.6f, 0.6f, 0.6f, 1.2f, 0.6f, 0.6f, 1.2f, 0.6f, 0.6f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 1.5f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
        {1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 1.0f, 1.0f},
    };

    const int numFreeSe = 120;
    const int numFreeSeq = 3;
    const int numFreeStream = 3;
    const int numFreeSoundChild = 160;
    const int aramAddr = nullptr;
    const int aramSize = 0xE00000;
    const int workAreaSize = 0x1000;
    const int audioThreadPriority = 2;
    const int dvdThreadPriority = 5;
    const int dvdThreadSwitchID = -1;
    const int audioThreadSwitchID = -1;
    const int numFreeTracks = 200;
    const int numFreeTrackChannels = 1;
    const int streamBlockSize = 0x2760;
    const int numStreamChannels = 4;
    const f32 outputGain = 0.9f;
    const int maxSeqDataSize = 0xDAC0;
    const s32 numInspectableSoundObj = 8;
    const f32 masterVolBgm = 1.2f;
    const f32 masterVolSe = 1.2f;
    const f32 masterVolStream = 2.6f;
    const JAISeCategoryArrangementItem arrangeSE_SY = {8, 16};
    const JAISeCategoryArrangementItem arrangeSE_PV = {2, 4};
    const JAISeCategoryArrangementItem arrangeSE_PM = {10, 18};
    const JAISeCategoryArrangementItem arrangeSE_BV = {8, 12};
    const JAISeCategoryArrangementItem arrangeSE_BM = {20, 24};
    const JAISeCategoryArrangementItem arrangeSE_OJ = {10, 18};
    const JAISeCategoryArrangementItem arrangeSE_AT = {24, 28};
    const JAISeCategoryArrangementItem arrangeSE_DM = {10, 20};
    const JAISeCategoryArrangementItem arrangeSE_EV = {24, 32};
    const JAISeCategoryArrangementItem arrangeSE_EM = {32, 36};
    const JAISeCategoryArrangementItem arrangeSE_SV = {16, 20};
    const JAISeCategoryArrangementItem arrangeSE_SM = {8, 12};
    const JAISeCategoryArrangementItem arrangeSE_RS = {16, 20};
    const JAISeCategoryArrangementItem arrangeHOMESE = {8, 16};
    const s32 streamAramBlocks_perChunk = 36;
    const f32 streamSampleRate = 32000.0f;
    const f32 faderVolZeroVolume = 0.0f;
    const f32 faderVolDownVolume = 0.3333f;
    const f32 faderInterruptedVolume = 0.0f;
    const s32 faderVolZeroTime = 5;
    const s32 faderVolDownTime = 3;
    const s32 faderInterruptedTime = 5;
    const s32 faderVolZeroRecoverTime = 5;
    const s32 faderVolDownRecoverTime = 20;
    const s32 faderInterruptedRecoverTime = 60;
    const f32 micFarDistance = 7000.0f;
    const f32 micNearDistance = 600.0f;
    const f32 micMinVolume = 0.0f;
    const f32 micFrontDolbyZ = -1000.0f;
    const f32 micBehindDolbyZ = 6000.0f;
    const f32 micMaxDolby = 1.0f;
    const f32 micMinDolby = 0.0f;
    const f32 micCenterDolby = 0.85f;
    const f32 micPanAmplitude = 0.5f;
    const f32 micSonicSpeed = 800.0f;
    const f32 micPitchDeltaRatioRange = 1.5f;
    const s32 micPriorityDownRange = 255;
};  // namespace AudParams
