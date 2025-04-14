#include "Game/Screen/MovieSubtitlesDataTable.hpp"
#include "Game/Util.hpp"

#define ENTRY(name, step, appear) { name, step, appear }
#define NULL_ENTRY { nullptr, 0xFFFFFFFF, 0xFFFFFFFF }

namespace {
    static SubtitleInfo sSubtitlesInfo[7] = {
        { "/MovieData/PrologueA.thp", 
            {
                ENTRY("Layout_StoryDemoKoopaTalk000", 0xF2F, 0x84),
                ENTRY("Layout_StoryDemoKoopaTalk001", 0xFB4, 0x9A),
                ENTRY("Layout_StoryDemoKoopaTalk002", 0x104F, 0x78),
                ENTRY("Layout_StoryDemoKoopaTalk003", 0x1248, 0xB3),
                ENTRY("Layout_StoryDemoKoopaTalk004", 0x11D0, 0x78),
            }
        },
        { "/MovieData/PrologueB.thp", 
            {
                ENTRY("Layout_StoryDemoPeachTalk000", 0x1338, 0x78),
                ENTRY("Layout_StoryDemoKameckTalk000", 0x1554, 0xB4),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
        { "/MovieData/FinalBattle.thp", 
            {
                ENTRY("Layout_StoryDemoRosettaTalk000", 0xB9A, 0x9F),
                ENTRY("Layout_StoryDemoRosettaTalk006", 0xC3A, 0xB4),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
        { "/MovieData/EpilogueA.thp", 
            {
                ENTRY("Layout_StoryDemoKoopaTalk005", 0x11FF, 0xBB),
                ENTRY("Layout_StoryDemoKoopaTalk006", 0x12BB, 0xE2),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
        { "/MovieData/EpilogueB.thp", 
            {
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
        { "/MovieData/EndingA.thp", 
            {
                ENTRY("Layout_StoryDemoRosettaTalk001", 0x1374, 0x77),
                ENTRY("Layout_StoryDemoRosettaTalk002", 0x13EC, 0x95),
                ENTRY("Layout_StoryDemoRosettaTalk003", 0x1482, 0x78),
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
        { "/MovieData/EndingB.thp", 
            {
                ENTRY("Layout_StoryDemoRosettaTalk004", 0x744, 0x86),
                ENTRY("Layout_StoryDemoRosettaTalk005", 0x7CB, 0x113),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY
            }
        },
    };
};

namespace MovieSubtitlesUtil {
    inline SubtitleInfo* getSubtitleInfo(const SubtitleInfo* pInfos, const char *pTag) {
        for (u32 i = 0; i < 7; i++) {
            if (MR::isEqualString(pTag, pInfos[i].mFile)) {
                return &sSubtitlesInfo[i];
            }
        }

        return nullptr;
    }

    SubtitleMessageInfo* getSubtitlesMessageInfo(const char *pTag, s32 idx) {
        return &getSubtitleInfo(sSubtitlesInfo, pTag)->mMessageInfos[idx];
    }

    bool isExistSubtitles(const char *pTag, s32 idx) {
        return getSubtitlesMessageInfo(pTag, idx)->mMessageID != 0;
    }

    const char* getSubtitlesMessageId(const char *pTag, s32 idx) {
        SubtitleMessageInfo* inf = getSubtitlesMessageInfo(pTag, idx);
        if (inf != nullptr) {
            return inf->mMessageID;
        }

        return nullptr;
    }

    u32 getSubtitlesStartStep(const char *pTag, s32 idx) {
        SubtitleMessageInfo* inf = getSubtitlesMessageInfo(pTag, idx);
        if (inf != nullptr) {
            return inf->mStartStep;
        }

        return 0;
    }

    u32 getSubtitlesAppearTime(const char *pTag, s32 idx) {
        SubtitleMessageInfo* inf = getSubtitlesMessageInfo(pTag, idx);
        if (inf != nullptr) {
            return inf->mAppearTime;
        }

        return 0;
    }

    u32 getSubtitlesMessageNum(const char *pTag) {
        for (s32 i = 0; i < 5; i++) {
            bool exist = getSubtitlesMessageInfo(pTag, i)->mMessageID != nullptr;
            if (!exist) {
                return i;
            }
        }
        
        return 5;
    }
};
