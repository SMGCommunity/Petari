#pragma once

#include <revolution.h>

class NPCActorItem;
class TalkMessageCtrl;

namespace MR {
    bool getNPCItemData(NPCActorItem *, s32);

    bool isNPCItemFileExist(const char *);

    void startNPCTalkCamera(const TalkMessageCtrl *, MtxPtr, f32, s32);
};