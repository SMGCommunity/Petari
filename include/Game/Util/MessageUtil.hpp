#pragma once

#include <revolution/types.h>

namespace MR {
    const wchar_t* getSystemMessageDirect(const char*);
    const wchar_t* getGameMessageDirect(const char*);
    const wchar_t* getLayoutMessageDirect(const char*);
    const wchar_t* getCurrentGalaxyNameOnCurrentLanguage();
    const wchar_t* getCurrentGalaxyNameShortOnCurrentLanguage();
    const wchar_t* getCurrentScenarioNameOnCurrentLanguage();
    const wchar_t* getRaceNameOnCurrentLanguage(int);
    bool isExistGameMessage(const char*);
    const wchar_t* getMessageLine(wchar_t*, u32, const wchar_t*, u32);
    s32 countMessageLine(const wchar_t*);
    s32 countMessageChar(const wchar_t*);
    s32 countMessageFigure(s32);
    const wchar_t* getNextMessagePage(const wchar_t*);
    const wchar_t* getGalaxyNameOnCurrentLanguage(const char*);
    const wchar_t* getGalaxyNameShortOnCurrentLanguage(const char*);
    const wchar_t* getScenarioNameOnCurrentLanguage(const char*, s32);
    void getLayoutMessageID(char*, const char*, const char*);
    void makeCometMessageID(char*, u32, const char*);
};  // namespace MR
