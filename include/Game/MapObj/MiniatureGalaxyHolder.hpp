#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"
#include <revolution.h>

class MiniatureGalaxyHolder : LiveActor {
public:
    MiniatureGalaxyHolder();
    ~MiniatureGalaxyHolder();
    void registerActor(LiveActor*, const JMapInfoIter&);
    virtual void init(const JMapInfoIter&);
    bool isRegisteredActor(const LiveActor*);
    MiniatureGalaxy* findMiniatureGalaxy(const char*) const; // MiniatureGalaxy* ?
    void killAllMiniatureGalaxy();
    s32 calcIndex(const LiveActor*) const;
    void updateCometStatus();

    
    LiveActorGroup* _8C;
    bool _90;
    MiniatureGalaxy* _94;
    s32 _98;
    s32 _9C;
};

class MiniatureGalaxyFunction {
    public:
    static void registerMiniatureGalaxyToHolder(LiveActor*, const JMapInfoIter&); 
    static s32 getMiniatureGalaxyNum();
    static void calcMiniatureGalaxyIndex(const LiveActor*);
    static void updateCometStatus();
    static MiniatureGalaxy* getCometLandMiniatureGalaxy();
    static s32 getCometNameId();
    static MiniatureGalaxy* getPointingMiniatureGalaxy();
    
    inline static MiniatureGalaxyHolder* getMiniGalaxyHolder() {    // No proves, no doubts
        SceneObjHolder* mHolder = MR::getSceneObjHolder();
        MiniatureGalaxyHolder* mMiniatureGalaxyHolder = (MiniatureGalaxyHolder*)mHolder->getObj(115);
        return mMiniatureGalaxyHolder;
    }
};
