#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"

class StarPieceGroup : public LiveActor {
    public:
        StarPieceGroup(const char *);

        virtual void init(const JMapInfoIter &);
        virtual void appear();
        virtual void makeActorAppeared();
        virtual void makeActorDead();
        virtual void startClipped();
        virtual void endClipped();
        virtual void control();

        bool isExistAnyStarPiece();
        void forceReplaceStarPieceAll();
        void forceKillStarPieceAll(bool);
        void placementAllPiece();
        void placementPieceOnCircle();
        void placementPieceOnRail();
        void placementPieceOnRailPoint();
        void onSwitchA();
        void offSwitchA();
        void onSwitchB();
        void exeFlow();

};
