#pragma once

#include "Game/Screen/LayoutActor.h"
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

    LayoutActor* mHostActor;    // _0
    const wchar_t* mMsg;        // _4
    u32 _8;
    const char* mPaneName;      // _C
};