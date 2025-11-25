#pragma once

#include "Game/Screen/CustomTagProcessor.hpp"
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

    LayoutActor* mHostActor;  // 0x0
    const wchar_t* mMsg;      // 0x4
    s32 _8;
    const char* mPaneName;  // 0xC
};

namespace MR {
    void nextStepTagProcessorRecursive(LayoutActor*, const char*);
    bool isEndStepTagProcessorRecursive(const LayoutActor*, const char*, bool);
    void initTagProcessorRecursive(LayoutActor*, const char*, long);
}  // namespace MR
