#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    void declareNoteNumMaxAndMelodyToCounter(LiveActor *, s32, s32, f32);
    void addNoteToCounter();
    void failureNoteCollection();
};