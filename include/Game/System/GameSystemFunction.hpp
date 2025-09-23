#pragma once

#include <revolution.h>

class GameSystemFunction {
public:
    static void loadAudioStaticWaveData();
    static bool isLoadedAudioStaticWaveData();
    static void initAfterStationedResourceLoaded();
    static void setSceneNameObjHolderToNameObjRegister();

    static bool isOccurredSystemWarning();
    static bool isResetProcessing();

    static void resetCurrentScenarioNo();

    static void restartControllerLeaveWatcher();

    static void resetAllControllerRumble();

    static bool setPermissionToCheckWiiRemoteConnectAndScreenDimming(bool);
    static void onPauseBeginAllRumble();
    static void onPauseEndAllRumble();
};
