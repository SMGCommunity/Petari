#pragma once

#include <revolution.h>

struct SubtitleMessageInfo {
    const char* mMessageID;         // 0x00
    u32 mStartStep;                 // 0x04
    u32 mAppearTime;                // 0x08
};

struct SubtitleInfo {
    const char* mFile;                          // 0x00
    SubtitleMessageInfo mMessageInfos[5];       // 0x04
};

namespace MovieSubtitlesUtil {
    SubtitleMessageInfo* getSubtitlesMessageInfo(const char *, s32);
    bool isExistSubtitles(const char *, s32);
    const char* getSubtitlesMessageId(const char *, s32);
    u32 getSubtitleStartStep(const char *, s32);
    u32 getSubtitlesAppearTime(const char *, s32);
    u32 getSubtitlesMessageNum(const char *);
};
