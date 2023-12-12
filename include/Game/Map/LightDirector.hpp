#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/Map/LightDataHolder.h"
#include "Game/Map/LightPointCtrl.h"
#include "Game/Map/LightZoneDataHolder.h"

class LightDirector : public NameObj {
public:
    LightDirector();

    virtual ~LightDirector();
    virtual void init(const JMapInfoIter &);
    virtual void movement();

    void initData();
    void loadLightPlayer() const;
    void loadLightCoin() const;

    u32 _C;
    LightDataHolder* mHolder;           // _10
    LightZoneDataHolder* mZoneHolder;   // _14
    u32 _18;
    u32 _1C; // ActorLightCtrl
    LightPointCtrl* mPointCtrl;         // _20
    ResourceHolder* mResourceHolder;    // _24
};