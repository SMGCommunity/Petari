#include "Game/RhythmLib/AudChordInfo.hpp"
#include <JSystem/JKernel/JKRArchive.hpp>

template<>
AudChordInfo* JASGlobalInstance<AudChordInfo>::sInstance = nullptr;

void AudScaleData::initScaleData(u32 base) {
    up = (const u8*)((u32)up + base);
    down = (const u8*)((u32)down + base);
}

AudChordTable::AudChordTable() {
    mLoaded = 0;
    mChordCount = 0;
    mScaleCount = 0;
    mChordPtr = nullptr;
    mScalePtr = nullptr;
}

bool AudChordTable::setChordTable(s32 id, JKRArchive* pArchive) {
    mLoaded = 0;

    if (id < 0) {
        return false;
    }

    void* pRes = pArchive->getResource((u16)id);
    bool result = setChordTableResource(pRes);
    mLoaded = result;
    return result;
}

bool AudChordTable::setChordTableResource(void* pRes) {
    u32 relocFlag = *(u32*)pRes;
    bool alreadyRelocated = false;
    
    if (relocFlag != 0) {
        alreadyRelocated = true;
    }

    // Check for 'CITS' signature
    if (((u8*)pRes)[4] != 'C' ||
        ((u8*)pRes)[5] != 'I' ||
        ((u8*)pRes)[6] != 'T' ||
        ((u8*)pRes)[7] != 'S') {
        return false;
    }

    u8* base = (u8*)pRes + 0x0C;
    mChordCount = *(u16*)base;
    mScaleCount = *(u16*)(base + 2);
    mChordPtr = (u32*)(base + 4);
    mScalePtr = (u32*)(base + 4 + mChordCount * 4);

    if (!alreadyRelocated) {
        // Relocate chord pointers
        for (s32 i = 0; i < (s32)mChordCount; i++) {
            mChordPtr[i] = (u32)pRes + mChordPtr[i];
        }

        // Relocate scale pointers and init scale data
        for (s32 i = 0; i < (s32)mScaleCount; i++) {
            mScalePtr[i] = (u32)pRes + mScalePtr[i];
            ((AudScaleData*)mScalePtr[i])->initScaleData((u32)pRes);
        }

        *(u32*)pRes = 1;
    }

    return true;
}

AudChordInfo::AudChordInfo() : JASGlobalInstance<AudChordInfo>(this), mArchive(nullptr), mTable() {
    mCurChord = nullptr;
    mCurScale = nullptr;
    mFlags = 0;
    mUnknown2C = 0;
    mCurChordIndex = 0;
    mCurScaleIndex = 0;
    mTableId = -1;

    for (s32 i = 0; i < 8; i++) {
        mChordNoteList[i] = 0x7F;
    }
}

void AudChordInfo::init(JKRArchive* pArchive) {
    mArchive = pArchive;
}

bool AudChordInfo::loadChordInfo(s16 id, bool keepCurrent) {
    initParams();

    BOOL inter = OSDisableInterrupts();
    bool result = mTable.setChordTable(id, mArchive);

    if (result) {
        if (keepCurrent) {
            setCurChord(mCurChordIndex);
            setCurScale(mCurScaleIndex);
        }
        else {
            setCurChord(0);
            setCurScale(0);
        }
        mTableId = id;
    }

    OSRestoreInterrupts(inter);
    return result;
}

void AudChordInfo::invalidiate() {
    initParams();
}

bool AudChordInfo::setCurChord(s32 index) {
    if (!mTable.mLoaded) {
        return false;
    }

    if (index >= (s32)mTable.mChordCount) {
        return false;
    }

    mCurChord = (void*)mTable.mChordPtr[index];
    expandChordNote();
    mCurChordIndex = index;
    return true;
}

bool AudChordInfo::setCurScale(s32 index) {
    if (!mTable.mLoaded) {
        return false;
    }

    if (index >= (s32)mTable.mScaleCount) {
        return false;
    }

    mCurScale = (AudScaleData*)mTable.mScalePtr[index];
    mCurScaleIndex = index;
    return true;
}

#pragma dont_inline on
u8 AudChordInfo::getBassNote() const {
    u8* chord = (u8*)mCurChord;
    u8 bass = chord[0];

    if (bass == 0x7F) {
        return chord[1];
    }

    return bass;
}

u8 AudChordInfo::getRoot() const {
    return ((u8*)mCurChord)[1];
}

u8 AudChordInfo::getChordTone(int idx) const {
    return ((u8*)mCurChord)[1 + idx];
}

u8 AudChordInfo::getAddNote(int idx) const {
    return ((u8*)mCurChord)[5 + idx];
}

u8 AudChordInfo::getScaleNoteUp(s32 idx) {
    return mCurScale->up[idx];
}

u8 AudChordInfo::getScaleNoteDown(s32 idx) {
    return mCurScale->down[idx];
}
#pragma dont_inline off

u8 AudChordInfo::getThird() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 3);
    u8 hi = (u8)(root + 5);

    for (s32 i = 0; i < 4; i++) {
        u8 tone = getChordTone(i);

        if (tone == 0x7F) {
            continue;
        }

        if (tone < root) {
            tone = (u8)(tone + 12);
        }

        if (tone >= lo && tone <= hi) {
            return (u8)(tone % 12);
        }
    }

    return 0x7F;
}

u8 AudChordInfo::getFifth() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 6);
    u8 hi = (u8)(root + 8);

    for (s32 i = 0; i < 4; i++) {
        u8 tone = getChordTone(i);

        if (tone == 0x7F) {
            continue;
        }

        if (tone < root) {
            tone = (u8)(tone + 12);
        }

        if (tone >= lo && tone <= hi) {
            return (u8)(tone % 12);
        }
    }

    return 0x7F;
}

u8 AudChordInfo::getSeventh() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 9);
    u8 hi = (u8)(root + 11);

    for (s32 i = 0; i < 4; i++) {
        u8 tone = getChordTone(i);

        if (tone == 0x7F) {
            continue;
        }

        if (tone < root) {
            tone = (u8)(tone + 12);
        }

        if (tone >= lo && tone <= hi) {
            return (u8)(tone % 12);
        }
    }

    return 0x7F;
}

u8 AudChordInfo::getSixth() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 8);
    u8 hi = (u8)(root + 9);

    for (s32 i = 0; i < 8; i++) {
        u8 note = mChordNoteList[i];

        if (note == 0x7F) {
            continue;
        }

        if (note < root) {
            note = (u8)(note + 12);
        }

        if (note >= lo && note <= hi) {
            return (u8)(note % 12);
        }
    }

    return 0x7F;
}

u8 AudChordInfo::getNinth() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 1);
    u8 hi = (u8)(root + 3);

    for (s32 i = 0; i < 8; i++) {
        u8 note = mChordNoteList[i];

        if (note == 0x7F) {
            continue;
        }

        if (note < root) {
            note = (u8)(note + 12);
        }

        if (note >= lo && note <= hi) {
            u8 third = getThird();
            if (note == third) {
                continue;
            }
            return (u8)(note % 12);
        }
    }

    return 0x7F;
}

u8 AudChordInfo::getEleventh() {
    if (!isAvailable()) {
        return 0x7F;
    }

    u8 root = getRoot();
    u8 lo = (u8)(root + 5);
    u8 hi = (u8)(root + 6);

    for (s32 i = 0; i < 8; i++) {
        u8 note = mChordNoteList[i];

        if (note == 0x7F) {
            continue;
        }

        if (note < root) {
            note = (u8)(note + 12);
        }

        if (note >= lo && note <= hi) {
            return (u8)(note % 12);
        }
    }

    return 0x7F;
}

bool AudChordInfo::isOnChord(u8 note, bool includeAdd, bool includeBass) {
    s32 pitchClass = note % 12;

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

    s32 octave = note / 12;
    u8 pitchClass = note % 12;
    s32 minDist = 0x7FFF;
    s32 nearest = -1;

    if (up) {
        for (s32 i = 0; i < 8; i++) {
            s32 n = mChordNoteList[i];

            if (n == 0x7F) {
                continue;
            }

            if (n < pitchClass) {
                n += 12;
            }

            s32 dist = n - pitchClass;

            if (dist < minDist) {
                minDist = dist;
                nearest = n;
            }
        }
    }
    else {
        for (s32 i = 0; i < 8; i++) {
            s32 n = mChordNoteList[i];

            if (n == 0x7F) {
                continue;
            }

            s32 orig = n;

            if (n > pitchClass) {
                n -= 12;
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

    nearest += octave * 12;
    return (u8)nearest;
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
    if (!isOnChord(note, true, true)) {
        note = (u8)getNearestChordNoteDir(note, true);
        steps--;
    }

    if (steps <= 0) {
        return (u8)note;
    }

    u8 pitchClass = (u8)(note % 12);
    s32 octave = note / 12;

    s32 idx = getChordNoteIndex(pitchClass);
    s32 wrapCount;
    idx = addIndexInChordNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    return (u8)(mChordNoteList[idx] + wrapCount * 12);
}

u8 AudChordInfo::getLowerNoteOnChord(u8 note, s32 steps) {
    if (!isOnChord(note, true, true)) {
        note = (u8)getNearestChordNoteDir(note, false);
        steps--;
    }

    if (steps <= 0) {
        return (u8)note;
    }

    u8 pitchClass = (u8)(note % 12);
    s32 octave = note / 12;

    s32 idx = getChordNoteIndex(pitchClass);
    s32 wrapCount;
    idx = subIndexInChordNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    if (wrapCount < 0) {
        wrapCount = 0;
    }

    return (u8)(mChordNoteList[idx] + wrapCount * 12);
}

s32 AudChordInfo::getOnScaleType(u8 note) {
    s32 pitchClass = note % 12;
    s32 onUp = 0;
    s32 onDown = 0;

    for (s32 i = 0; i < 12; i++) {
        s32 upNote = getScaleNoteUp(i);
        if (pitchClass == upNote) {
            onUp = 1;
            break;
        }
    }

    for (s32 i = 0; i < 12; i++) {
        s32 downNote = getScaleNoteDown(i);
        if (pitchClass == downNote) {
            onDown = 1;
            break;
        }
    }

    if (onUp != 0) {
        if (onDown != 0) {
            return 3;
        }
        return 1;
    }

    s32 result = 0;
    if (onDown != 0) {
        result = 2;
    }
    return result;
}

u8 AudChordInfo::getNearestScaleNoteDir(u8 note, bool up) {
    if (up) {
        if (getOnScaleType(note) == 1) {
            return note;
        }
    }
    else {
        if (getOnScaleType(note) == 2) {
            return note;
        }
    }

    s32 octave = note / 12;
    u8 pitchClass = note % 12;
    s32 minDist = 0x7FFF;
    s32 nearest = -1;

    if (up) {
        for (s32 i = 0; i < 12; i++) {
            s32 n = getScaleNoteUp(i);

            if (n == 0x7F) {
                continue;
            }

            if (n < pitchClass) {
                n += 12;
            }

            s32 dist = n - pitchClass;

            if (dist < minDist) {
                minDist = dist;
                nearest = n;
            }
        }
    }
    else {
        for (s32 i = 0; i < 12; i++) {
            s32 n = getScaleNoteDown(i);

            if (n == 0x7F) {
                continue;
            }

            s32 orig = n;

            if (n > pitchClass) {
                n -= 12;
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

    nearest += octave * 12;
    return (u8)nearest;
}

u8 AudChordInfo::getNearestScaleNote(u8 note) {
    if (getOnScaleType(note) == 3) {
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
    if (!(getOnScaleType(note) & 1)) {
        note = (u8)getNearestScaleNoteDir(note, true);
        steps--;
    }

    if (steps <= 0) {
        return (u8)note;
    }

    u8 pitchClass = (u8)(note % 12);
    s32 octave = note / 12;

    s32 idx = getScaleNoteIndex(pitchClass, true);
    s32 wrapCount;
    idx = addIndexInScaleNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    return (u8)(getScaleNoteUp(idx) + wrapCount * 12);
}

u8 AudChordInfo::getLowerNoteOnScale(u8 note, s32 steps) {
    if (!(getOnScaleType(note) & 2)) {
        note = (u8)getNearestScaleNoteDir(note, false);
        steps--;
    }

    if (steps <= 0) {
        return (u8)note;
    }

    u8 pitchClass = (u8)(note % 12);
    s32 octave = note / 12;

    s32 idx = getScaleNoteIndex(pitchClass, false);
    s32 wrapCount;
    idx = subIndexInScaleNoteList(idx, steps, wrapCount);

    wrapCount += octave;

    if (wrapCount < 0) {
        wrapCount = 0;
    }

    return (u8)(getScaleNoteDown(idx) + wrapCount * 12);
}

void AudChordInfo::expandChordNote() {
    // Reset note list
    for (s32 i = 0; i < 8; i++) {
        mChordNoteList[i] = 0x7F;
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
    s32 lastSwap = 7;
    while (lastSwap >= 1) {
        s32 newLastSwap = -1;

        for (s32 i = 1; i <= lastSwap; i++) {
            if (mChordNoteList[i] < mChordNoteList[i - 1]) {
                u8 tmp = mChordNoteList[i - 1];
                mChordNoteList[i - 1] = mChordNoteList[i];
                mChordNoteList[i] = tmp;
                newLastSwap = i - 1;
            }
        }

        lastSwap = newLastSwap;
    }

    // Remove duplicates
    for (s32 i = 0; i < 7; i++) {
        if (mChordNoteList[i] == mChordNoteList[i + 1]) {
            for (s32 j = i + 1; j < 7; j++) {
                mChordNoteList[j] = mChordNoteList[j + 1];
            }
            mChordNoteList[7] = 0x7F;
            i--;
        }
    }
}

s32 AudChordInfo::getChordNoteIndex(u8 pitchClass) {
    for (s32 i = 0; i < 8; i++) {
        if (mChordNoteList[i] == pitchClass) {
            return i;
        }
    }

    return -1;
}

s32 AudChordInfo::addIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount) {
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx++;

        if (idx >= 8) {
            wrapCount++;
            idx = 0;
        }

        s32 skip = 0;

        while (mChordNoteList[idx] == 0x7F) {
            skip++;
            idx++;

            if (skip >= 8) {
                return -1;
            }

            if (idx >= 8) {
                wrapCount++;
                idx = 0;
            }
        }
    }

    return idx;
}

s32 AudChordInfo::subIndexInChordNoteList(s32 idx, s32 count, s32& wrapCount) {
    wrapCount = 0;

    for (s32 c = 0; c < count; c++) {
        idx--;

        if (idx < 0) {
            wrapCount--;
            idx = 7;
        }

        s32 skip = 0;

        while (mChordNoteList[idx] == 0x7F) {
            skip++;
            idx--;

            if (skip >= 8) {
                return -1;
            }

            if (idx < 0) {
                wrapCount--;
                idx = 7;
            }
        }
    }

    return idx;
}

s32 AudChordInfo::getScaleNoteIndex(u8 pClass, bool up) {
    s32 pitchClass = pClass;
    if (up) {
        for (s32 i = 0; i < 12; i++) {
            if (pitchClass == (s32)getScaleNoteUp(i)) {
                return i;
            }
        }
    }
    else {
        for (s32 i = 0; i < 12; i++) {
            if (pitchClass == (s32)getScaleNoteDown(i)) {
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

        if (idx >= 12) {
            wrapCount++;
            idx = 0;
        }

        s32 skip = 0;

        while ((s32)getScaleNoteUp(idx) == 127) {
            skip++;
            idx++;

            if (skip >= 12) {
                return -1;
            }

            if (idx >= 12) {
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
            idx = 11;
        }

        s32 skip = 0;

        while ((s32)getScaleNoteDown(idx) == 127) {
            skip++;
            idx--;

            if (skip >= 12) {
                return -1;
            }

            if (idx < 0) {
                wrapCount--;
                idx = 11;
            }
        }
    }

    return idx;
}

void AudChordInfo::initParams() {
    BOOL inter = OSDisableInterrupts();

    mFlags = 0;
    mCurChord = nullptr;
    mCurScale = nullptr;
    mTableId = -1;
    mUnknown2C = 0;

    for (s32 i = 0; i < 8; i++) {
        mChordNoteList[i] = 0x7F;
    }

    OSRestoreInterrupts(inter);
}
