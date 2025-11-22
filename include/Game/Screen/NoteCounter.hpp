#pragma once

#include "Game/Screen/LayoutActor.hpp"

class LiveActor;

class NoteCounter : public LayoutActor {
public:
    /// @brief Creates a new `NoteCounter`.
    /// @param pName The pointer to the null-terminated name of the object.
    NoteCounter(const char* pName);

    virtual void init(const JMapInfoIter& rIter);

    void declareNoteNumMaxAndMelody(LiveActor* pHost, s32 noteNum, s32 melodyNo, f32 railLength);
    void add();
    void exeShow();
    void exeShowWait();
    void exeShowToHide();
    void exeHideToShow();
    void exeHide();
    void tryEndDisp();

    /// @brief The total number of notes in the song.
    /* 0x20 */ s32 mNoteNum;

    /// @brief The cumulative number of collected notes.
    /* 0x24 */ s32 mNoteAddNum;

    /// @brief The total number of missed notes.
    /* 0x28 */ s32 mNoteMissNum;

    /// @brief The numeric identifier for the selected song.
    /* 0x2C */ s32 mMelodyNo;

    /// @brief The length of the path that notes are distributed along.
    /* 0x30 */ f32 mRailLength;

    /// @brief The pointer to the owning actor instance.
    /* 0x34 */ LiveActor* mHost;
};

namespace MR {
    void declareNoteNumMaxAndMelodyToCounter(LiveActor* pHost, s32 noteNum, s32 melodyNo, f32 railLength);
    void addNoteToCounter();
    void failureNoteCollection();
};  // namespace MR
