#pragma once

#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "revolution/types.h"

class GalaxySelectInfo : public LayoutActor {
    public:
        GalaxySelectInfo(bool);
        ~GalaxySelectInfo();
        virtual void init(const JMapInfoIter&);
        virtual void draw() const;
        virtual void appear();
        virtual void kill();
        
        void show(const char*, u32, s32);
        void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
        void control();
        void show(const char*, u32, bool, s32);
        void exeWait();
        void exeDisappear();
        // ...
};
