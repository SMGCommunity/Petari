#pragma once

#include <JSystem/JAudio2/JASGlobal.hpp>
#include <revolution.h>

class JKRArchive;

struct AudChordData {
    /* 0x00 */ u8 mBass;
    /* 0x01 */ u8 mChordTone[4];
    /* 0x05 */ u8 mAddNote[3];
};

struct AudScaleData {
    void initScaleData(u32 base) NO_INLINE;

    /* 0x00 */ u8* up;
    /* 0x04 */ u8* down;
};

class AudChordTable {
public:
    AudChordTable();

    bool setChordTable(s32 id, JKRArchive* pArchive);
    bool setChordTableResource(void* pRes);

    /* 0x00 */ bool mLoaded;
    /* 0x04 */ int mChordCount;
    /* 0x08 */ int mScaleCount;
    /* 0x0C */ AudChordData** mChordPtr;
    /* 0x10 */ AudScaleData** mScalePtr;
};

class AudChordInfo : JASGlobalInstance< AudChordInfo > {
public:
    enum ScaleType {
        ScaleType_None = 0,
        ScaleType_Up = 1,
        ScaleType_Down = 2,
        ScaleType_UpDown = 3,
    };

    static const int OCTAVE = 12;
    static const int NULL_NOTE = 0x7F;
    static const int NUM_CHORD_NOTES = 8;

    AudChordInfo();

    void init(JKRArchive* pArchive);
    bool loadChordInfo(s16 id, bool keepCurrent);
    void invalidiate();
    bool setCurChord(s32 index);
    bool setCurScale(s32 index);

    u8 getBassNote() const;
    u8 getRoot() const NO_INLINE {
        return mCurChord->mChordTone[0];
    }
    u8 getChordTone(int idx) const NO_INLINE {
        return mCurChord->mChordTone[idx];
    }
    u8 getAddNote(int idx) const NO_INLINE {
        return mCurChord->mAddNote[idx];
    }
    s32 getScaleNoteUp(s32 idx) NO_INLINE {
        return mCurScale->up[idx];
    }
    s32 getScaleNoteDown(s32 idx) NO_INLINE {
        return mCurScale->down[idx];
    }

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

    /* 0x00 */ JKRArchive* mArchive;
    /* 0x04 */ AudChordTable mTable;
    /* 0x18 */ AudChordData* mCurChord;
    /* 0x1C */ AudScaleData* mCurScale;
    /* 0x20 */ u8 mChordNoteList[NUM_CHORD_NOTES];
    /* 0x28 */ u8 mFlags;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ s32 mCurChordIndex;
    /* 0x34 */ s32 mCurScaleIndex;
    /* 0x38 */ s16 mTableId;
};
