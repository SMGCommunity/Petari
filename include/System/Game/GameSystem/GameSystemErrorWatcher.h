#pragma once

#include "Actor/Nerve/NerveExecutor.h"

class GameSystemErrorWatcher : public NerveExecutor
{
public:
    GameSystemErrorWatcher();

    void initAfterResourceLoaded();
    void movement();
    void draw();
    bool isWarning() const;
    void setPermissionUpdateWiiRemoteStatus(bool);
};