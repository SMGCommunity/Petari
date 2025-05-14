#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class HeavensDoorDemoObj : public MapObjActor {
    public:
        HeavensDoorDemoObj(const char *);

        virtual ~HeavensDoorDemoObj();
        virtual void init(const JMapInfoIter &);
        virtual void kill();
        virtual void control();
        virtual void connectToScene(const MapObjActorInitInfo &);

        void exeAppear();
        void exeVanish();
        void startInsideCageDemo();
        inline void exeWait() {
            if (MapObjActor::isObjectName("HeavensDoorInsidePlanetPartsA")) {
                MR::startLevelSound(this, "SE_OJ_LV_HD_INSIDE_SUN_WORK", -1, -1, -1);
            }
        }

        bool _C4;
};

namespace NrvHeavensDoorDemoObj {
    NERVE(HeavensDoorDemoObjNrvAppear);
    NERVE(HeavensDoorDemoObjNrvWait);
    NERVE(HeavensDoorDemoObjNrvVanish);
};
