#include "Game/RhythmLib/AudChordInfo.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>

void AudScaleData::initScaleData(u32 base) {
    up += base;
    down += base;
}

AudChordTable::AudChordTable() : mLoaded(false), mChordCount(0), mScaleCount(0), mChordPtr(nullptr), mScalePtr(nullptr) {
}

bool AudChordTable::setChordTable(s32 id, JKRArchive* pArchive) {
    mLoaded = false;

    if (id < 0) {
        return false;
    }

    bool loaded = setChordTableResource(pArchive->getResource((u16)id));
    mLoaded = loaded;
    return loaded;
}

bool AudChordTable::setChordTableResource(void* pRes) {
    s32* cursor = (s32*)pRes;

    bool alreadyRelocated = false;
    s32* initialized = cursor;
    if (*cursor++) {
        alreadyRelocated = true;
    }

    char* sig = (char*)cursor;
    // Check for 'CITS' signature
    if (sig[0] != 'C' || sig[1] != 'I' || sig[2] != 'T' || sig[3] != 'S') {
        return false;
    }

    cursor++;
    cursor++;

    u8* base = (u8*)cursor;
    mChordCount = *(u16*)(base + 0);
    mScaleCount = *(u16*)(base + 2);
    mChordPtr = (AudChordData**)(base + 4);
    mScalePtr = (AudScaleData**)(base + 4 + mChordCount * 4);

    if (!alreadyRelocated) {
        // Relocate chord pointers
        for (s32 i = 0; i < mChordCount; i++) {
            mChordPtr[i] = (AudChordData*)((u32)pRes + (u32)mChordPtr[i]);
        }

        // Relocate scale pointers and init scale data
        for (s32 i = 0; i < mScaleCount; i++) {
            mScalePtr[i] = (AudScaleData*)((u32)pRes + (u32)mScalePtr[i]);
            mScalePtr[i]->initScaleData((u32)pRes);
        }

        *initialized = true;
    }

    return true;
}

AudChordInfo::AudChordInfo()
    : JASGlobalInstance< AudChordInfo >(true), mArchive(nullptr), mCurChord(nullptr), mCurScale(nullptr), mFlags(0), _2C(0), mCurChordIndex(0),
      mCurScaleIndex(0), mTableId(-1) {
    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        mChordNoteList[i] = NULL_NOTE;
    }
}

void AudChordInfo::init(JKRArchive* pArchive) {
    mArchive = pArchive;
}

bool AudChordInfo::loadChordInfo(s16 id, bool keepCurrent) {
    initParams();

    JASCriticalSection crit;
    bool success = mTable.setChordTable(id, mArchive);

    if (success) {
        if (keepCurrent) {
            setCurChord(mCurChordIndex);
            setCurScale(mCurScaleIndex);
        } else {
            setCurChord(0);
            setCurScale(0);
        }
        mTableId = id;
    }

    return success;
}

void AudChordInfo::invalidiate() {
    initParams();
}

bool AudChordInfo::setCurChord(s32 index) {
    if (!mTable.mLoaded) {
        return false;
    }

    if (index >= mTable.mChordCount) {
        return false;
    }

    mCurChord = mTable.mChordPtr[index];
    expandChordNote();
    mCurChordIndex = index;
    return true;
}

bool AudChordInfo::setCurScale(s32 index) {
    if (!mTable.mLoaded) {
        return false;
    }

    if (index >= mTable.mScaleCount) {
        return false;
    }

    mCurScale = mTable.mScalePtr[index];
    mCurScaleIndex = index;
    return true;
}

u8 AudChordInfo::getBassNote() const {
    if (mCurChord->mBass == NULL_NOTE) {
        return mCurChord->mChordTone[0];
    }

    return mCurChord->mBass;
}

u8 AudChordInfo::getThird() {
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = root + 3;
    u8 hi = root + 5;

    for (s32 i = 0; i < 4; i++) {
        u8 note = getChordTone(i);

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        if (note >= lo && note <= hi) {
            return note % OCTAVE;
        }
    }

    return NULL_NOTE;
}

u8 AudChordInfo::getFifth() {
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = root + 6;
    u8 hi = root + 8;

    for (s32 i = 0; i < 4; i++) {
        u8 note = getChordTone(i);

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        if (note >= lo && note <= hi) {
            return note % OCTAVE;
        }
    }

    return NULL_NOTE;
}

u8 AudChordInfo::getSeventh() {
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = root + 9;
    u8 hi = root + 11;

    for (s32 i = 0; i < 4; i++) {
        u8 note = getChordTone(i);

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        if (note >= lo && note <= hi) {
            return note % OCTAVE;
        }
    }

    return NULL_NOTE;
}

u8 AudChordInfo::getSixth() {
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = root + 8;
    u8 hi = root + 9;

    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        u8 note = mChordNoteList[i];

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        note &= 0xFF;

        if (note >= lo && note <= hi) {
            return note % OCTAVE;
        }
    }

    return NULL_NOTE;
}

u8 AudChordInfo::getNinth() {
    // FIXME: missing clrlwi
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = root + 1;
    u8 hi = root + 3;

    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        u8 note = mChordNoteList[i];

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        note &= 0xFF;

        if (note >= lo && note <= hi) {
            if (note != getThird()) {
                return note % OCTAVE;
            }
        }
    }

    return NULL_NOTE;
}

u8 AudChordInfo::getEleventh() {
    if (!isAvailable()) {
        return NULL_NOTE;
    }

    u8 root = getRoot();
    u8 lo = (root + 5);
    u8 hi = (root + 6);

    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        u8 note = mChordNoteList[i];

        if (note == NULL_NOTE) {
            continue;
        }

        if (note < root) {
            note += OCTAVE;
        }

        note &= 0xFF;

        if (note >= lo && note <= hi) {
            return note % OCTAVE;
        }
    }

    return NULL_NOTE;
}

bool AudChordInfo::isOnChord(u8 note, bool includeAdd, bool includeBass) {
    s32 pitchClass = note % OCTAVE;

    // Check chord tones
    for (s32 i = 0; i < 4; i++) {
        if (pitchClass == getChordTone(i)) {
            return true;
        }
    }

    // Check add notes
    if (includeAdd) {
        for (s32 i = 0; i < 3; i++) {
            if (pitchClass == getAddNote(i)) {
                return true;
            }
        }
    }

    // Check bass
    if (includeBass) {
        if (pitchClass == getBassNote()) {
            return true;
        }
    }

    return false;
}

u8 AudChordInfo::getNearestChordNoteDir(u8 note, bool up) {
    if (isOnChord(note, true, true)) {
        return note;
    }

    s32 octave = note / OCTAVE;
    s32 pitchClass = note % OCTAVE;
    s32 minDist = 0x7FFF;
    s32 nearest = -1;

    if (up) {
        for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
            s32 n = mChordNoteList[i];

            if (n == NULL_NOTE) {
                continue;
            }

            if (n < pitchClass) {
                n += OCTAVE;
            }

            s32 dist = n - pitchClass;

            if (dist < minDist) {
                minDist = dist;
                nearest = n;
            }
        }
    } else {
        for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
            s32 note = mChordNoteList[i];

            if (note == NULL_NOTE) {
                continue;
            }

            s32 orig = note;

            if (note > pitchClass) {
                note -= OCTAVE;
            }

            s32 dist = pitchClass - note;

            if (dist < minDist) {
                minDist = dist;
                nearest = orig;
            }
        }
    }

    if (nearest < 0) {
        nearest = pitchClass;
    }

    nearest += octave * OCTAVE;
    return nearest;
}

u8 AudChordInfo::getNearestChordNote(u8 note) {
    if (isOnChord(note, true, true)) {
        return note;
    }

    u8 upNote = getNearestChordNoteDir(note, true);
    u8 downNote = getNearestChordNoteDir(note, false);

    if (upNote < downNote) {
        return upNote;
    }

    return downNote;
}

u8 AudChordInfo::getUpperNoteOnChord(u8 note, s32 steps) {
    // FIXME: missing clrlwi
    if (!isOnChord(note, true, true)) {
        note = getNearestChordNoteDir(note, true) & 0xFF;
        steps--;
    }

    if (steps <= 0) {
        return note;
    }

    s32 pitchClass = note % OCTAVE;
    s32 octave = note / OCTAVE;

    s32 idx = getChordNoteIndex(pitchClass);
    s32 wrapCount;
    idx = addIndexInChordNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    return mChordNoteList[idx] + wrapCount * OCTAVE;
}

u8 AudChordInfo::getLowerNoteOnChord(u8 note, s32 steps) {
    // FIXME: missing clrlwi
    if (!isOnChord(note, true, true)) {
        note = getNearestChordNoteDir(note, false) & 0xFF;
        steps--;
    }

    if (steps <= 0) {
        return note;
    }

    s32 pitchClass = note % OCTAVE;
    s32 octave = note / OCTAVE;

    s32 idx = getChordNoteIndex(pitchClass);
    s32 wrapCount;
    idx = subIndexInChordNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    if (wrapCount < 0) {
        wrapCount = 0;
    }

    return mChordNoteList[idx] + wrapCount * OCTAVE;
}

s32 AudChordInfo::getOnScaleType(u8 note) {
    bool onUp = false;
    bool onDown = false;
    s32 pitchClass = note % OCTAVE;

    for (s32 i = 0; i < OCTAVE; i++) {
        if (pitchClass == getScaleNoteUp(i)) {
            onUp = true;
            break;
        }
    }

    for (s32 i = 0; i < OCTAVE; i++) {
        if (pitchClass == getScaleNoteDown(i)) {
            onDown = true;
            break;
        }
    }

    if (onUp) {
        return onDown ? ScaleType_UpDown : ScaleType_Up;
    } else {
        return onDown ? ScaleType_Down : ScaleType_None;
    }
}

u8 AudChordInfo::getNearestScaleNoteDir(u8 note, bool up) {
    if (up) {
        if (getOnScaleType(note) == ScaleType_Up) {
            return note;
        }
    } else {
        if (getOnScaleType(note) == ScaleType_Down) {
            return note;
        }
    }

    s32 octave = note / OCTAVE;
    s32 pitchClass = note % OCTAVE;
    s32 minDist = 0x7FFF;
    s32 nearest = -1;

    if (up) {
        for (s32 i = 0; i < OCTAVE; i++) {
            s32 n = getScaleNoteUp(i);

            if (n == NULL_NOTE) {
                continue;
            }

            if (n < pitchClass) {
                n += OCTAVE;
            }

            s32 dist = n - pitchClass;

            if (dist < minDist) {
                minDist = dist;
                nearest = n;
            }
        }
    } else {
        for (s32 i = 0; i < OCTAVE; i++) {
            s32 n = getScaleNoteDown(i);

            if (n == NULL_NOTE) {
                continue;
            }

            s32 orig = n;

            if (n > pitchClass) {
                n -= OCTAVE;
            }

            s32 dist = pitchClass - n;

            if (dist < minDist) {
                minDist = dist;
                nearest = orig;
            }
        }
    }

    if (nearest < 0) {
        nearest = pitchClass;
    }

    nearest += octave * OCTAVE;
    return nearest;
}

u8 AudChordInfo::getNearestScaleNote(u8 note) {
    if (getOnScaleType(note) == ScaleType_UpDown) {
        return note;
    }

    u8 upNote = getNearestScaleNoteDir(note, true);
    u8 downNote = getNearestScaleNoteDir(note, false);

    if (upNote < downNote) {
        return upNote;
    }

    return downNote;
}

u8 AudChordInfo::getUpperNoteOnScale(u8 note, s32 steps) {
    // FIXME: missing clrlwi
    if ((getOnScaleType(note) & 1) == 0) {  // not up
        note = getNearestScaleNoteDir(note, true) & 0xFF;
        steps--;
    }

    if (steps <= 0) {
        return note;
    }

    s32 pitchClass = note % OCTAVE;
    s32 octave = note / OCTAVE;

    s32 idx = getScaleNoteIndex(pitchClass, true);
    s32 wrapCount;
    idx = addIndexInScaleNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    return getScaleNoteUp(idx) + wrapCount * OCTAVE;
}

u8 AudChordInfo::getLowerNoteOnScale(u8 note, s32 steps) {
    // FIXME: missing clrlwi
    if ((getOnScaleType(note) & 2) == 0) {  // not down
        note = getNearestScaleNoteDir(note, false) & 0xFF;
        steps--;
    }

    if (steps <= 0) {
        return note;
    }

    s32 pitchClass = note % OCTAVE;
    s32 octave = note / OCTAVE;

    s32 idx = getScaleNoteIndex(pitchClass, false);
    s32 wrapCount;
    idx = subIndexInScaleNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    if (wrapCount < 0) {
        wrapCount = 0;
    }

    return getScaleNoteDown(idx) + wrapCount * OCTAVE;
}

void AudChordInfo::expandChordNote() {
    // Reset note list
    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        mChordNoteList[i] = NULL_NOTE;
    }

    // Copy chord tones
    s32 idx = 0;
    for (s32 i = 0; i < 4; i++) {
        mChordNoteList[idx++] = getChordTone(i);
    }

    // Copy add notes
    for (s32 i = 0; i < 3; i++) {
        mChordNoteList[idx++] = getAddNote(i);
    }

    // Add bass note
    mChordNoteList[idx] = getBassNote();

    // Bubble sort ascending
    s32 lastSwap = NUM_CHORD_NOTES - 1;
    while (lastSwap >= 1) {
        s32 newLastSwap = -1;

        for (s32 i = 1; i <= lastSwap; i++) {
            if (mChordNoteList[i] < mChordNoteList[i - 1]) {
                u8 tmp = mChordNoteList[i];
                mChordNoteList[i] = mChordNoteList[i - 1];
                mChordNoteList[i - 1] = tmp;
                newLastSwap = i - 1;
            }
        }

        lastSwap = newLastSwap;
    }

    // Remove duplicates
    for (s32 i = 0; i < NUM_CHORD_NOTES - 1; i++) {
        if (mChordNoteList[i] == mChordNoteList[i + 1]) {
            for (s32 j = i + 1; j < NUM_CHORD_NOTES - 1; j++) {
                mChordNoteList[j] = mChordNoteList[j + 1];
            }
            mChordNoteList[NUM_CHORD_NOTES - 1] = NULL_NOTE;
        }
    }
}

s32 AudChordInfo::getChordNoteIndex(u8 pitchClass) {
    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        if (mChordNoteList[i] == pitchClass) {
            return i;
        }
    }

    return -1;
}

s32 AudChordInfo::addIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount) {
    // FIXME: regswaps
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx++;

        if (idx >= NUM_CHORD_NOTES) {
            wrapCount++;
            idx = 0;
        }

        s32 skip = 0;

        while (mChordNoteList[idx] == NULL_NOTE) {
            skip++;
            idx++;

            if (skip >= NUM_CHORD_NOTES) {
                return -1;
            }

            if (idx >= NUM_CHORD_NOTES) {
                wrapCount++;
                idx = 0;
            }
        }
    }

    return idx;
}

s32 AudChordInfo::subIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount) {
    // FIXME: regswaps
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx--;

        if (idx < 0) {
            wrapCount--;
            idx = NUM_CHORD_NOTES - 1;
        }

        s32 skip = 0;

        while (mChordNoteList[idx] == NULL_NOTE) {
            skip++;
            idx--;

            if (skip >= NUM_CHORD_NOTES) {
                return -1;
            }

            if (idx < 0) {
                wrapCount--;
                idx = NUM_CHORD_NOTES - 1;
            }
        }
    }

    return idx;
}

s32 AudChordInfo::getScaleNoteIndex(u8 pClass, bool up) {
    s32 pitchClass = pClass;
    if (up) {
        for (s32 i = 0; i < OCTAVE; i++) {
            if (pitchClass == getScaleNoteUp(i)) {
                return i;
            }
        }
    } else {
        for (s32 i = 0; i < OCTAVE; i++) {
            if (pitchClass == getScaleNoteDown(i)) {
                return i;
            }
        }
    }

    return -1;
}

s32 AudChordInfo::addIndexInScaleNoteList(s32 idx, s32 count, s32& wrapCount) {
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx++;

        if (idx >= OCTAVE) {
            wrapCount++;
            idx = 0;
        }

        s32 skip = 0;

        while (getScaleNoteUp(idx) == NULL_NOTE) {
            skip++;
            idx++;

            if (skip >= OCTAVE) {
                return -1;
            }

            if (idx >= OCTAVE) {
                wrapCount++;
                idx = 0;
            }
        }
    }

    return idx;
}

s32 AudChordInfo::subIndexInScaleNoteList(s32 idx, s32 count, s32& wrapCount) {
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx--;

        if (idx < 0) {
            wrapCount--;
            idx = OCTAVE - 1;
        }

        s32 skip = 0;

        while (getScaleNoteDown(idx) == NULL_NOTE) {
            skip++;
            idx--;

            if (skip >= OCTAVE) {
                return -1;
            }

            if (idx < 0) {
                wrapCount--;
                idx = OCTAVE - 1;
            }
        }
    }

    return idx;
}

void AudChordInfo::initParams() {
    JASCriticalSection crit;

    mFlags = 0;
    mCurChord = nullptr;
    mCurScale = nullptr;
    mTableId = -1;
    _2C = 0;

    for (s32 i = 0; i < NUM_CHORD_NOTES; i++) {
        mChordNoteList[i] = NULL_NOTE;
    }
}
