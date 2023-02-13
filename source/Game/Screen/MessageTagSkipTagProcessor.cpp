#include "Game/Screen/MessageTagSkipTagProcessor.h"

MessageTagSkipTagProcessor::MessageTagSkipTagProcessor() : nw4r::ut::TagProcessorBase<wchar_t>() {

}

nw4r::ut::TagProcessorBase<wchar_t>::Operation MessageTagSkipTagProcessor::CalcRect(nw4r::ut::Rect *pRect, u16 code, ContextType *pPrintContext) {
    if (code != 0x1A) {
        return nw4r::ut::TagProcessorBase<wchar_t>::CalcRect(pRect, code, pPrintContext);
    }
    else {
        return skipTag(pRect, pPrintContext, false);
    }
}

nw4r::ut::TagProcessorBase<wchar_t>::Operation MessageTagSkipTagProcessor::Process(u16 code, ContextType *pPrintContext) {
    if (code != 0x1A) {
        return nw4r::ut::TagProcessorBase<wchar_t>::Process(code, pPrintContext);
    }
    else {
        return skipTag(nullptr, pPrintContext, false);
    }
}

/*
nw4r::ut::TagProcessorBase<wchar_t>::Operation MessageTagSkipTagProcessor::skipTag(nw4r::ut::Rect *pRect, ContextType *pPrintContext, bool a3) {
    pPrintContext->str += (*pPrintContext->str - 2) & 0xFFFFFFFE;
    return (nw4r::ut::TagProcessorBase<wchar_t>::Operation)0;
}
*/
