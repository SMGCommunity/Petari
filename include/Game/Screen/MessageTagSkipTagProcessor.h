#pragma once

#include <nw4r/ut/TagProcessorBase.h>

class MessageTagSkipTagProcessor : public nw4r::ut::TagProcessorBase<wchar_t> {
public:
    MessageTagSkipTagProcessor();

    virtual ~MessageTagSkipTagProcessor() {

    }

    virtual nw4r::ut::TagProcessorBase<wchar_t>::Operation Process(u16, ContextType *);
    virtual nw4r::ut::TagProcessorBase<wchar_t>::Operation CalcRect(nw4r::ut::Rect *, u16, ContextType *);

    nw4r::ut::TagProcessorBase<wchar_t>::Operation skipTag(nw4r::ut::Rect *, ContextType *, bool);
};

class MessageEditorMessageTag {
public:
    MessageEditorMessageTag(const wchar_t *);

    const wchar_t* mMessage;    // _0
};