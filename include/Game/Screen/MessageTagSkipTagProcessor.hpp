#pragma once

#include "nw4r/ut/TagProcessorBase.h"

class MessageTagSkipTagProcessor : public nw4r::ut::TagProcessorBase< wchar_t > {
public:
    MessageTagSkipTagProcessor();

    virtual ~MessageTagSkipTagProcessor() {}

    virtual nw4r::ut::TagProcessorBase< wchar_t >::Operation Process(u16, ContextType*);
    virtual nw4r::ut::TagProcessorBase< wchar_t >::Operation CalcRect(nw4r::ut::Rect*, u16, ContextType*);

    nw4r::ut::TagProcessorBase< wchar_t >::Operation skipTag(nw4r::ut::Rect*, ContextType*, bool);
};

class MessageEditorMessageTag {
public:
    MessageEditorMessageTag(const wchar_t*);

    u32 getTagLength() const;
    u32 getSkipLength() const;
    u32 getParamLength() const;
    u8 getParam8(int) const;
    u16 getParam16(int) const;
    u32 getParam32(int) const;
    wchar_t* getParamPtr(int) const;

    const wchar_t* mMessage;  // 0x0
};
