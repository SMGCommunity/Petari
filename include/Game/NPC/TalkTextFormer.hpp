#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/CustomTagProcessor.h"

class TalkTextFormer {
public:
    TalkTextFormer(LayoutActor *, const char *);

    void formMessage(const wchar_t *, s32);
    void setArg(const CustomTagArg &, s32);
    bool nextPage();
    bool hasNextPage() const;
    void updateTalking();
    bool isTextAppearedAll() const;

    LayoutActor* mHostActor;    // 0x0
    const wchar_t* mMsg;        // 0x4
    u32 _8;
    const char* mPaneName;      // 0xC
};