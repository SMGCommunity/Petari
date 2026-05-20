#pragma once

#include "Game/MapObj/MiniatureGalaxy.hpp"

class MiniatureGalaxyHolder : LiveActor {
public:
    MiniatureGalaxyHolder();
    ~MiniatureGalaxyHolder();
    void registerActor(LiveActor*, const JMapInfoIter&);
    virtual void init(const JMapInfoIter&);
    bool isRegisteredActor(const LiveActor*);
    MiniatureGalaxy* findMiniatureGalaxy(const char*) const;  // MiniatureGalaxy* ?
    void killAllMiniatureGalaxy();
    s32 calcIndex(const LiveActor*) const;
    void updateCometStatus();

    /* 0x8C */ LiveActorGroup* _8C;
    /* 0x90 */ bool _90;
    /* 0x94 */ MiniatureGalaxy* mCometGalaxy;
    /* 0x98 */ s32 mCometID;
    /* 0x9C */ s32 _9C;
};

class MiniatureGalaxyFunction {
public:
    static void registerMiniatureGalaxyToHolder(LiveActor*, const JMapInfoIter&);
    static s32 getMiniatureGalaxyNum();
    static s32 calcMiniatureGalaxyIndex(const LiveActor*);
    static void updateCometStatus();
    static MiniatureGalaxy* getCometLandMiniatureGalaxy();
    static s32 getCometNameId();
    static MiniatureGalaxy* getPointingMiniatureGalaxy();

    inline static MiniatureGalaxyHolder* getMiniGalaxyHolder() {  // No proves, no doubts
        SceneObjHolder* mHolder = MR::getSceneObjHolder();
        MiniatureGalaxyHolder* mMiniatureGalaxyHolder = (MiniatureGalaxyHolder*)mHolder->getObj(115);
        return mMiniatureGalaxyHolder;
    }
};
