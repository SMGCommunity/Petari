#pragma once

#include "Game/System/StationedFileInfo.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <revolution.h>

class StationedArchiveLoader {
public:
    class Condition {
    public:
        virtual ~Condition();
        virtual bool isExecute(const MR::StationedFileInfo *) const;
        virtual JKRHeap* getProperHeap(const MR::StationedFileInfo *) const;
    };

    static JKRHeap* getProperHeap(const MR::StationedFileInfo *);
    static void loadResourcesFromTable(const StationedArchiveLoader::Condition &);
    static void createAndAddResourcesFromTable(const StationedArchiveLoader::Condition &);
    static void loadScenarioData(JKRHeap *);
};