#pragma once

#include <revolution/types.h>

struct SubtitleMessageInfo {
    /* 0x00 */ const char* mMessageID;
    /* 0x04 */ u32 mStartStep;
    /* 0x08 */ u32 mAppearTime;
};

struct SubtitleInfo {
    /* 0x00 */ const char* mMovieName;
    /* 0x04 */ SubtitleMessageInfo mMessageInfo[5];
};

namespace MovieSubtitlesUtil {
    const SubtitleMessageInfo* getSubtitlesMessageInfo(const char *, s32);
    bool isExistSubtitles(const char *, s32);
    const char* getSubtitlesMessageId(const char *, s32);
    s32 getSubtitlesStartStep(const char *, s32);
    s32 getSubtitlesAppearTime(const char *, s32);
    s32 getSubtitlesMessageNum(const char *);
};
