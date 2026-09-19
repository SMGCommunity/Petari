#include "Game/System/StationedFileInfo.hpp"
#include "Kamek.hpp"

namespace {
    // clang-format off
    static const MR::StationedFileInfo cStationedFileInfo[] = {
        {
            MR::StationedFileInfo::HEAP_TYPE_GDDR,
            MR::StationedFileInfo::LOAD_TYPE_MOUNT,
            "/Debug/GameEventTable.byaml"
        },
        {
            static_cast<MR::StationedFileInfo::HeapType>(0),
            static_cast<MR::StationedFileInfo::LoadType>(255), 
            nullptr
        }
    };
    // clang-format on

    // clang-format off
#ifdef __MWERKS__
    static asm bool loadNewTable() {
        nofralloc
        lwz   r0, 8(r31)
        cmpwi r0, 0
        bne   loc_28
        lwz   r0, 4(r31)
        cmpwi r0, 255
        beq   loc_28
        lis   r31, cStationedFileInfo@ha
        addi  r31, r31, cStationedFileInfo@l
    loc_28:
        lwz   r3, 8(r31)
        addic r0, r3, -1
        subfe r3, r0, r3
        blr
    }
#else
    static void loadNewTable() {}
#endif
    // clang-format on
} // namespace

extern kmSymbol loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition;
kmCall(&loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0xB4, loadNewTable);
kmWrite32(&loadResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0xB8, PPC_CMPWI(3, 0));

extern kmSymbol createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition;
kmCall(&createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0x80, loadNewTable);
kmWrite32(&createAndAddResourcesFromTable__22StationedArchiveLoaderFRCQ222StationedArchiveLoader9Condition + 0x84, PPC_CMPWI(3, 0));
