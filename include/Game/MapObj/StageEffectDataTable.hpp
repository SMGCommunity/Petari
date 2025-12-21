#pragma once

#include <revolution.h>

class NameObj;
class LiveActor;

struct StageEffectSoundData {
    const char* objectName;         // 0x0
    const char* startSoundEffect;   // 0x4
    const char* movingSoundEffect;  // 0x8
    const char* stopSoundEffect;    // 0xC
    u32 stopSoundEffectSteps;       // 0x10
    s32 soundEffectType;            // 0x14
};

namespace MR {
    class StageEffect {
    public:
        static const char* getStartSe(const char*);
        static const char* getMovingSe(const char*);
        static const char* getStopSe(const char*);

        static s32 getStopSeSteps(const char*);

        static bool isRiddleSeTypeStop(const char*);

        static bool tryStageEffectStart(LiveActor*, const char*);
        static bool tryStageEffectMoving(LiveActor*, const char*);
        static bool tryStageEffectStop(LiveActor*, const char*);

        static bool isExistStageEffectData(const char*);

        static bool isExistStageEffectSeData(const char*);

        static void rumblePadMoving(LiveActor*, const char*);
        static void rumblePadStart(LiveActor*, const char*);
        static void rumblePadStop(LiveActor*, const char*);

        static void shakeCameraMoving(NameObj*, const char*);
        static void shakeStopCamera(LiveActor*, const char*);
        static void stopShakingCameraMoving(NameObj*, const char*);
    };
};  // namespace MR
