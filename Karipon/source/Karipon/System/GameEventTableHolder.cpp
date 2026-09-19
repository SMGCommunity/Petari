#include "Karipon/System/GameEventTableHolder.hpp"
#include "Game/System/GameEventFlag.hpp"
#include "Game/System/GameEventValueChecker.hpp"
#include "Game/Util/Array.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Karipon/System/ByamlFile.hpp"
#include "Karipon/System/ByamlIter.hpp"

namespace {
    static ByamlFile sFile;
    static MR::AssignableArray<GameEventFlag> sEventFlagTable;
    static MR::AssignableArray<GameEventValue> sEventValueTable;
    static ByamlIter sStoryEventsIt;

    static void readGameEventFlag(GameEventFlag& rFlag, const ByamlIter& rIter) {
        
    }

    static void readGameEventValue(GameEventValue& rValue, const ByamlIter& rIter) {
        
    }
}

void GameEventTableHolder::init() {
    void* pData = MR::receiveArchive("/Debug/GameEventTable.byaml");
    sFile.attach(static_cast<u8*>(pData));

    ByamlIter rootIt = sFile.getRootIter();
    ByamlIter eventFlagsIt = rootIt.getIterByKey("EventFlags");
    ByamlIter eventValuesIt = rootIt.getIterByKey("EventValues");

    sStoryEventsIt = rootIt.getIterByKey("StoryEvents");
}
