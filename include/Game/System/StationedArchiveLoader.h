#pragma once

#include "Game/System/StationedFileInfo.h"
#include <JSystem/JKernel/JKRHeap.h>
#include <revolution.h>

class StationedArchiveLoader {
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