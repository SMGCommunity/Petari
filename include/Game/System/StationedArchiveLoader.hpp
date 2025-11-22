#pragma once

class JKRHeap;

namespace MR {
    struct StationedFileInfo;
};

class StationedArchiveLoader {
public:
    class Condition {
    public:
        virtual ~Condition();
        virtual bool isExecute(const MR::StationedFileInfo*) const;
        virtual JKRHeap* getProperHeap(const MR::StationedFileInfo*) const;
    };

    static JKRHeap* getProperHeap(const MR::StationedFileInfo*);
    static void loadResourcesFromTable(const StationedArchiveLoader::Condition&);
    static void createAndAddResourcesFromTable(const StationedArchiveLoader::Condition&);
    static void loadScenarioData(JKRHeap*);
};
