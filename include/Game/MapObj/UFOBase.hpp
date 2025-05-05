#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class UFOBase : public LiveActor {
    public:
        UFOBase(const char *);

        virtual void init(const JMapInfoIter &);
        virtual void kill();
        virtual void makeActorDead();
        virtual void control();
        virtual void calcAndSetBaseMtx();

        void exeWaitForPlayerOn();
        void exeMove();
        void initSensorType();
        void initSubModel(const JMapInfoIter &, const char *) NO_INLINE;
        void inline exeWait();

        CollisionParts* mCollisionParts; // _84
        LodCtrl* mLODCtrl; // _90
        MapPartsRailMover* mRailMover2; // _94
        MapPartsRailMover* mRailMover1; // _98
        TVec3f _9C;
        f32 _A8;
        s32 _AC;
        bool _B0;
        const char* _B4;
}; 

namespace NrvUFOBase {
    NERVE(UFOBaseNrvWait);   
    NERVE(UFOBaseNrvWaitForPlayerOn);   
    NERVE(UFOBaseNrvMove);   
    NERVE(UFOBaseNrvBreak);   
};
