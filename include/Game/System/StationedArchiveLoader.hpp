#pragma once

class JKRHeap;

namespace MR {
    struct StationedFileInfo;
};  // namespace MR

class StationedArchiveLoader {
public:
    class Condition {
    public:
        /* 0x08 */ virtual ~Condition();
        /* 0x0C */ virtual bool isExecute(const MR::StationedFileInfo*) const;
        /* 0x10 */ virtual JKRHeap* getProperHeap(const MR::StationedFileInfo*) const;
    };

    static JKRHeap* getProperHeap(const MR::StationedFileInfo*);
    static void loadResourcesFromTable(const StationedArchiveLoader::Condition&);
    static void createAndAddResourcesFromTable(const StationedArchiveLoader::Condition&);
    static void loadScenarioData(JKRHeap*);
};
