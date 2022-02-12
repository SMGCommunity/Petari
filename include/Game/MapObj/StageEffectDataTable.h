#pragma once

class NameObj;
class LiveActor;

namespace MR {
    class StageEffect {
    public:
        static const char* getStartSe(const char *);
        static const char* getMovingSe(const char *);
        static const char* getStopSe(const char *);

        static s32 getStopSeSteps(const char *);

        static bool isRiddleSeTypeStop(const char *);

        static bool tryStageEffectStart(LiveActor *, const char *);
        static bool tryStageEffectMoving(LiveActor *, const char *);
        static bool tryStageEffectStop(LiveActor *, const char *);

        static bool isExistStageEffectData(const char *);

        static void rumblePadStart(LiveActor *, const char *);
        static void rumblePadStop(LiveActor *, const char *);

        static void shakeCameraMoving(NameObj *, const char *);
        static void stopShakingCameraMoving(NameObj *, const char *);
    };
};