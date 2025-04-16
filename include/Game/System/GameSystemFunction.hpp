#pragma once

#include <revolution.h>

class GameSystemFunction {
public:
    static void resetCurrentScenarioNo();

    static void initAfterStationedResourceLoaded();

    static void setSceneNameObjHolderToNameObjRegister();

    static void loadAudioStaticWaveData();

    static bool isLoadedAudioStaticWaveData();

    static bool setPermissionToCheckWiiRemoteConnectAndScreenDimming(bool);

    static void onPauseBeginAllRumble();
    static void onPauseEndAllRumble();
};
