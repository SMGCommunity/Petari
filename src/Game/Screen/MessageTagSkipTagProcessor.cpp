#include "Game/Screen/MessageTagSkipTagProcessor.hpp"

MessageEditorMessageTag::MessageEditorMessageTag(const nw4r::ut::PrintContext< wchar_t >* pContext) : mMessage(pContext->str) {
}

MessageEditorMessageTag::MessageEditorMessageTag(const wchar_t* pMessage) : mMessage(pMessage) {
}

u32 MessageEditorMessageTag::getTagLength() const {
    return reinterpret_cast< const u8* >(mMessage)[0] - 2;
}

u32 MessageEditorMessageTag::getSkipLength() const {
    return getTagLength() / 2;
}

s32 MessageEditorMessageTag::getParamLength() const {
    return reinterpret_cast< const u8* >(mMessage)[0] - 6;
}

u8 MessageEditorMessageTag::getParam8(int index) const {
    const u8* pParam = reinterpret_cast< const u8* >(mMessage) + index;
    return pParam[4];
}

u16 MessageEditorMessageTag::getParam16(int index) const {
    const u16* pParam = reinterpret_cast< const u16* >(mMessage) + index;
    return pParam[2];
}

u32 MessageEditorMessageTag::getParam32(int index) const {
    const u32* pParam = reinterpret_cast< const u32* >(mMessage) + index;
    return pParam[1];
}

wchar_t* MessageEditorMessageTag::getParamPtr(int index) const {
    const u8* pParam = reinterpret_cast< const u8* >(mMessage) + index + 4;
    return const_cast< wchar_t* >(reinterpret_cast< const wchar_t* >(pParam));
}

MessageTagSkipTagProcessor::MessageTagSkipTagProcessor() : nw4r::ut::TagProcessorBase< wchar_t >() {
}

nw4r::ut::TagProcessorBase< wchar_t >::Operation MessageTagSkipTagProcessor::CalcRect(nw4r::ut::Rect* pRect, u16 code, ContextType* pPrintContext) {
    if (code != 0x1A) {
        return nw4r::ut::TagProcessorBase< wchar_t >::CalcRect(pRect, code, pPrintContext);
    } else {
        return skipTag(pRect, pPrintContext, false);
    }
}

nw4r::ut::TagProcessorBase< wchar_t >::Operation MessageTagSkipTagProcessor::Process(u16 code, ContextType* pPrintContext) {
    if (code != 0x1A) {
        return nw4r::ut::TagProcessorBase< wchar_t >::Process(code, pPrintContext);
    } else {
        return skipTag(nullptr, pPrintContext, false);
    }
}

nw4r::ut::TagProcessorBase< wchar_t >::Operation MessageTagSkipTagProcessor::skipTag(nw4r::ut::Rect* pRect, ContextType* pPrintContext, bool param3) {
    MessageEditorMessageTag tag(pPrintContext);
    pPrintContext->str += tag.getSkipLength();
    return OPERATION_DEFAULT;
}
