#pragma once

#include "revolution/types.h"

class NPCActorItem;
namespace NPCFunction {
    void createNPCData();
    void deleteNPCData();
    bool getNPCItemData(NPCActorItem*, s32);
};  // namespace NPCFunction
