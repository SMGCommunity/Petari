#pragma once

#include "Game/Screen/LayoutActor.hpp"

struct CustomTagArg;

class TalkTextFormer {
public:
    TalkTextFormer(LayoutActor*, const char*);

    void formMessage(const wchar_t*, s32);
    void setArg(const CustomTagArg&, s32);
    bool nextPage();
    bool hasNextPage() const;
    void updateTalking();
    bool isTextAppearedAll() const;

    /* 0x0 */ LayoutActor* mHostActor;
    /* 0x4 */ const wchar_t* mMsg;
    s32 _8;
    /* 0xC */ const char* mPaneName;
};

namespace MR {
    void nextStepTagProcessorRecursive(LayoutActor*, const char*);
    bool isEndStepTagProcessorRecursive(const LayoutActor*, const char*, bool);
    void initTagProcessorRecursive(LayoutActor*, const char*, s32);
}  // namespace MR
