#pragma once

#include "Game/NameObj/NameObj.hpp"

class ActorLightCtrl;
class AreaLightInfo;
class LightDataHolder;
class LightPointCtrl;
class LightZoneDataHolder;
class ResourceHolder;

class LightDirector : public NameObj {
public:
    /// @brief Creates a new `LightDirector`.
    LightDirector();

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void initData();
    void loadLightPlayer() const;
    void loadLightCoin() const;

    /* 0x0C */ u32 _C;
    /* 0x10 */ LightDataHolder* mDataHolder;
    /* 0x14 */ LightZoneDataHolder* mZoneDataHolder;
    /* 0x18 */ AreaLightInfo* mDefaultAreaLight;
    /* 0x1C */ ActorLightCtrl* _1C;
    /* 0x20 */ LightPointCtrl* mPointCtrl;
    /* 0x24 */ ResourceHolder* mResourceHolder;
};
