#pragma once

#include "Game/Map/LightDataHolder.hpp"
#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Map/LightZoneDataHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/System/ResourceHolder.hpp"

class LightDirector : public NameObj {
public:
    LightDirector();

    virtual ~LightDirector();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void initData();
    void loadLightPlayer() const;
    void loadLightCoin() const;

    u32 _C;
    LightDataHolder* mHolder;          // 0x10
    LightZoneDataHolder* mZoneHolder;  // 0x14
    u32 _18;
    u32 _1C;                          // ActorLightCtrl
    LightPointCtrl* mPointCtrl;       // 0x20
    ResourceHolder* mResourceHolder;  // 0x24
};
