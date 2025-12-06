#include "Game/Screen/MovieSubtitlesDataTable.hpp"
#include "Game/Util/StringUtil.hpp"

#define ENTRY(name, step, appear)                                                                                                                    \
    { name, step, appear }
#define NULL_ENTRY                                                                                                                                   \
    { nullptr, -1, -1 }

namespace {
    static const SubtitlesInfo sSubtitlesInfo[] = {
        {
            "/MovieData/PrologueA.thp",
            {
                ENTRY("Layout_StoryDemoKoopaTalk000", 3887, 132),
                ENTRY("Layout_StoryDemoKoopaTalk001", 4020, 154),
                ENTRY("Layout_StoryDemoKoopaTalk002", 4175, 120),
                ENTRY("Layout_StoryDemoKoopaTalk003", 4680, 179),
                ENTRY("Layout_StoryDemoKoopaTalk004", 4560, 120),
            },
        },
        {
            "/MovieData/PrologueB.thp",
            {
                ENTRY("Layout_StoryDemoPeachTalk000", 4920, 120),
                ENTRY("Layout_StoryDemoKameckTalk000", 5460, 180),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
        {
            "/MovieData/FinalBattle.thp",
            {
                ENTRY("Layout_StoryDemoRosettaTalk000", 2970, 159),
                ENTRY("Layout_StoryDemoRosettaTalk006", 3130, 180),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
        {
            "/MovieData/EpilogueA.thp",
            {
                ENTRY("Layout_StoryDemoKoopaTalk005", 4607, 187),
                ENTRY("Layout_StoryDemoKoopaTalk006", 4795, 226),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
        {
            "/MovieData/EpilogueB.thp",
            {
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
        {
            "/MovieData/EndingA.thp",
            {
                ENTRY("Layout_StoryDemoRosettaTalk001", 4980, 119),
                ENTRY("Layout_StoryDemoRosettaTalk002", 5100, 149),
                ENTRY("Layout_StoryDemoRosettaTalk003", 5250, 120),
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
        {
            "/MovieData/EndingB.thp",
            {
                ENTRY("Layout_StoryDemoRosettaTalk004", 1860, 134),
                ENTRY("Layout_StoryDemoRosettaTalk005", 1995, 275),
                NULL_ENTRY,
                NULL_ENTRY,
                NULL_ENTRY,
            },
        },
    };

    const SubtitlesInfo* getSubtitlesInfo(const char* pMovieName) {
        for (s32 i = 0; i < sizeof(sSubtitlesInfo) / sizeof(*sSubtitlesInfo); i++) {
            if (MR::isEqualString(pMovieName, sSubtitlesInfo[i].mMovieName)) {
                return &sSubtitlesInfo[i];
            }
        }

        return nullptr;
    }
};  // namespace

namespace MovieSubtitlesUtil {
    const SubtitlesMessageInfo* getSubtitlesMessageInfo(const char* pMovieName, s32 idx) { return &getSubtitlesInfo(pMovieName)->mMessageInfo[idx]; }

    bool isExistSubtitles(const char* pMovieName, s32 idx) { return getSubtitlesMessageInfo(pMovieName, idx)->mMessageID != nullptr; }

    const char* getSubtitlesMessageId(const char* pMovieName, s32 idx) {
        const SubtitlesMessageInfo* pSubtitles = getSubtitlesMessageInfo(pMovieName, idx);

        if (pSubtitles != nullptr) {
            return pSubtitles->mMessageID;
        }

        return nullptr;
    }

    s32 getSubtitlesStartStep(const char* pMovieName, s32 idx) {
        const SubtitlesMessageInfo* pSubtitles = getSubtitlesMessageInfo(pMovieName, idx);

        if (pSubtitles != nullptr) {
            return pSubtitles->mStartStep;
        }

        return 0;
    }

    s32 getSubtitlesAppearTime(const char* pMovieName, s32 idx) {
        const SubtitlesMessageInfo* pSubtitles = getSubtitlesMessageInfo(pMovieName, idx);

        if (pSubtitles != nullptr) {
            return pSubtitles->mAppearTime;
        }

        return 0;
    }

    s32 getSubtitlesMessageNum(const char* pMovieName) {
        s32 size = sizeof(sSubtitlesInfo->mMessageInfo) / sizeof(*sSubtitlesInfo->mMessageInfo);

        for (s32 i = 0; i < size; i++) {
            if (isExistSubtitles(pMovieName, i)) {
                continue;
            }

            return i;
        }

        return size;
    }
};  // namespace MovieSubtitlesUtil
