#ifndef GAMESYSTEMERRORWATCHER_H
#define GAMESYSTEMERRORWATCHER_H

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

#endif // GAMESYSTEMERROROWATCHER_H