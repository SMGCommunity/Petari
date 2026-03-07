#pragma once

#include <revolution.h>
#include <JSystem/JAudio2/JASGlobal.hpp>

class JKRArchive;

struct AudScaleData {
    void initScaleData(u32 base);

    const u8* up;   // 0x00
    const u8* down; // 0x04
};

class AudChordTable {
public:
    AudChordTable();

    bool setChordTable(s32 id, JKRArchive* pArchive);
    bool setChordTableResource(void* pRes);

    u8 mLoaded;         // 0x00
    u8 mPad[3];
    u32 mChordCount;    // 0x04
    u32 mScaleCount;    // 0x08
    u32* mChordPtr;     // 0x0C
    u32* mScalePtr;     // 0x10
};

class AudChordInfo : JASGlobalInstance<AudChordInfo> {
public:
    AudChordInfo();

    void init(JKRArchive* pArchive);
    bool loadChordInfo(s16 id, bool keepCurrent);
    void invalidiate();
    bool setCurChord(s32 index);
    bool setCurScale(s32 index);

    u8 getBassNote() const;
    u8 getRoot() const;
    u8 getChordTone(int idx) const;
    u8 getAddNote(int idx) const;
    u8 getScaleNoteUp(s32 idx);
    u8 getScaleNoteDown(s32 idx);

    u8 getThird();
    u8 getFifth();
    u8 getSeventh();
    u8 getSixth();
    u8 getNinth();
    u8 getEleventh();

    bool isOnChord(u8 note, bool includeAdd, bool includeBass);
    u8 getNearestChordNoteDir(u8 note, bool up);
    u8 getNearestChordNote(u8 note);
    u8 getUpperNoteOnChord(u8 note, s32 steps);
    u8 getLowerNoteOnChord(u8 note, s32 steps);

    s32 getOnScaleType(u8 note);
    u8 getNearestScaleNoteDir(u8 note, bool up);
    u8 getNearestScaleNote(u8 note);
    u8 getUpperNoteOnScale(u8 note, s32 steps);
    u8 getLowerNoteOnScale(u8 note, s32 steps);

    void expandChordNote();
    s32 getChordNoteIndex(u8 pitchClass);
    s32 addIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount);
    s32 subIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount);
    s32 getScaleNoteIndex(u8 pitchClass, bool up);
    s32 addIndexInScaleNoteList(s32 idx, s32 count, s32& wrapCount);
    s32 subIndexInScaleNoteList(s32 idx, s32 count, s32& wrapCount);

    void initParams();
    bool isAvailable() const;

    JKRArchive* mArchive;       // 0x00
    AudChordTable mTable;       // 0x04
    void* mCurChord;            // 0x18
    AudScaleData* mCurScale;    // 0x1C
    u8 mChordNoteList[8];       // 0x20
    u8 mFlags;                  // 0x28
    u8 mPad29[3];
    u32 mUnknown2C;             // 0x2C
    s32 mCurChordIndex;         // 0x30
    s32 mCurScaleIndex;         // 0x34
    s16 mTableId;               // 0x38
};
