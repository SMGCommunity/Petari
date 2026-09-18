#pragma once

#include <revolution/types.h>

class NameObj;
class LiveActor;

namespace MR {
    class StageEffect {
    public:
        static const char* getStartSe(const char* pObjectName);
        static const char* getMovingSe(const char* pObjectName);
        static const char* getStopSe(const char* pObjectName);
        static s32 getStopSeSteps(const char* pObjectName);
        static bool isRiddleSeTypeStop(const char* pObjectName);

        static bool isExistStageEffectSeData(const char* pObjectName);
        static bool isExistStageEffectData(const char* pObjectName);
    
        static void shakeStartCamera(LiveActor* pActor, const char* pObjectName);
        static void shakeStopCamera(LiveActor* pActor, const char* pObjectName);
        static void shakeCameraMoving(NameObj* pActor, const char* pObjectName);
        static void stopShakingCameraMoving(NameObj* pActor, const char* pObjectName);

        static void rumblePadStart(LiveActor* pActor, const char* pObjectName);
        static void rumblePadStop(LiveActor* pActor, const char* pObjectName);
        static void rumblePadMoving(LiveActor* pActor, const char* pObjectName);

        static bool tryStageEffectStart(LiveActor* pActor, const char* pObjectName);
        static bool tryStageEffectMoving(LiveActor* pActor, const char* pObjectName);
        static bool tryStageEffectStop(LiveActor* pActor, const char* pObjectName);
    };
};  // namespace MR
