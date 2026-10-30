#include "Game/Util/MessageUtil.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/Screen/MessageTagSkipTagProcessor.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

#define MESSAGE_ID_BUFFER_SIZE 256

const wchar_t* MR::getSystemMessageDirect(const char* pMessageId) {
    TalkMessageInfo messageInfo = TalkMessageInfo();

    MessageSystem::getSystemMessageDirect(&messageInfo, pMessageId);

    return reinterpret_cast< wchar_t* >(messageInfo._0);
}

const wchar_t* MR::getGameMessageDirect(const char* pMessageId) {
    TalkMessageInfo messageInfo = TalkMessageInfo();

    MessageSystem::getGameMessageDirect(&messageInfo, pMessageId);

    return reinterpret_cast< wchar_t* >(messageInfo._0);
}

const wchar_t* MR::getLayoutMessageDirect(const char* pMessageId) {
    TalkMessageInfo messageInfo = TalkMessageInfo();

    MessageSystem::getLayoutMessageDirect(&messageInfo, pMessageId);

    return reinterpret_cast< wchar_t* >(messageInfo._0);
}

const wchar_t* MR::getCurrentGalaxyNameOnCurrentLanguage() {
    return getGalaxyNameOnCurrentLanguage(getCurrentStageName());
}

const wchar_t* MR::getCurrentGalaxyNameShortOnCurrentLanguage() {
    return getGalaxyNameShortOnCurrentLanguage(getCurrentStageName());
}

const wchar_t* MR::getCurrentScenarioNameOnCurrentLanguage() {
    s32 selectedScenarioNo = getCurrentSelectedScenarioNo();
    s32 scenarioNo = selectedScenarioNo != -1 ? selectedScenarioNo : getCurrentScenarioNo();

    return getScenarioNameOnCurrentLanguage(getCurrentStageName(), scenarioNo);
}

const wchar_t* MR::getRaceNameOnCurrentLanguage(int raceId) {
    return getGameMessageDirect(RaceManagerFunction::getRaceMessageId(raceId));
}

bool MR::isExistGameMessage(const char* pMessageId) {
    TalkMessageInfo messageInfo = TalkMessageInfo();

    return MessageSystem::getGameMessageDirect(&messageInfo, pMessageId) &&
           getStringLengthWithMessageTag(reinterpret_cast< wchar_t* >(messageInfo._0)) != 0;
}

const wchar_t* MR::getMessageLine(wchar_t* pDst, u32 capacity, const wchar_t* pMessage, u32 line) {
    u32 currentLine = 0;

    while (*pMessage != 0) {
        if (currentLine == line) {
            break;
        }

        if (*pMessage == 0x1A) {
            pMessage++;
            MessageEditorMessageTag tag(pMessage);
            pMessage += tag.getSkipLength();

            if (tag.isGroupTagId(1, 1)) {
                break;
            }
        } else {
            if (*pMessage == L'\n') {
                currentLine++;
            }

            pMessage++;
        }
    }

    for (u32 count = 0; count < capacity; count++) {
        if (*pMessage == 0x1A) {
            *pDst++ = *pMessage++;
            MessageEditorMessageTag tag(pMessage);

            for (s32 i = 0; i < static_cast< s32 >(tag.getSkipLength()); i++) {
                *pDst = *pMessage;
                pMessage++;
                pDst++;
            }
        }

        if (*pMessage == L'\n' || *pMessage == 0) {
            break;
        }

        *pDst = *pMessage;

        if (count < capacity - 1) {
            pDst++;
            pMessage++;
        }
    }

    *pDst = 0;
    return pDst;
}

s32 MR::countMessageLine(const wchar_t* pMessage) {
    s32 count = 1;

    while (*pMessage != 0) {
        if (*pMessage == 0x1A) {
            pMessage++;
            MessageEditorMessageTag tag(pMessage);
            pMessage += tag.getSkipLength();

            if (tag.isGroupTagId(1, 1)) {
                break;
            }
        } else {
            if (*pMessage == L'\n') {
                count++;
            }

            pMessage++;
        }
    }

    return count;
}

s32 MR::countMessageChar(const wchar_t* pMessage) {
    if (pMessage == nullptr) {
        return 0;
    }

    s32 count = 0;

    while (*pMessage != 0) {
        if (*pMessage == 0x1A) {
            pMessage++;
            MessageEditorMessageTag tag(pMessage);
            pMessage += tag.getSkipLength();

            if (tag.getGroup() == 3) {
                count++;
            } else if (tag.getGroup() == 6) {
                count += countMessageFigure(*reinterpret_cast< const s32* >(tag.getParamPtr(0)));
            } else if (tag.getGroup() == 5) {
                count += 3;
            }
#if (VERSION == RMGK01)
            else if (tag.getGroup() == 11) {
                count += 2;
            }
#endif
            else if (tag.getGroup() == 7) {
                count += countMessageChar(*reinterpret_cast< const wchar_t* const* >(tag.getParamPtr(0)));
            } else if (tag.isGroupTagId(1, 1)) {
                break;
            }
        } else {
            pMessage++;
            count++;
        }
    }

    return count;
}

s32 MR::countMessageFigure(s32 value) {
    u32 magnitude = MR::abs(value);
    s32 count = 1;

    while ((magnitude /= 10) != 0) {
        count++;
    }

    return count;
}

const wchar_t* MR::getNextMessagePage(const wchar_t* pMessage) {
    while (*pMessage != 0) {
        if (*pMessage == 0x1A) {
            pMessage++;
            MessageEditorMessageTag tag(pMessage);
            pMessage += tag.getSkipLength();

            if (tag.isGroupTagId(1, 1)) {
                if (*pMessage == L'\n') {
                    pMessage++;
                }

                return pMessage;
            }
        } else {
            pMessage++;
        }
    }

    return nullptr;
}

const wchar_t* MR::getGalaxyNameOnCurrentLanguage(const char* pGalaxyName) {
    char messageId[MESSAGE_ID_BUFFER_SIZE];
    snprintf(messageId, sizeof(messageId), "GalaxyName_%s", pGalaxyName);

    return getGameMessageDirect(messageId);
}

const wchar_t* MR::getGalaxyNameShortOnCurrentLanguage(const char* pGalaxyName) {
    char messageId[MESSAGE_ID_BUFFER_SIZE];
    snprintf(messageId, sizeof(messageId), "GalaxyNameShort_%s", pGalaxyName);

    return getGameMessageDirect(messageId);
}

const wchar_t* MR::getScenarioNameOnCurrentLanguage(const char* pGalaxyName, s32 scenarioNo) {
    char messageId[MESSAGE_ID_BUFFER_SIZE];
    snprintf(messageId, sizeof(messageId), "ScenarioName_%s%d", pGalaxyName, scenarioNo);

    return getGameMessageDirect(messageId);
}

void MR::getLayoutMessageID(char* pDst, const char* pSuperMessageId, const char* pSubMessageId) {
    snprintf(pDst, MESSAGE_ID_BUFFER_SIZE, "Layout_%s%s", pSuperMessageId, pSubMessageId);
}

void MR::makeCometMessageID(char* pDst, u32 bufferSize, const char* pCometName) {
    snprintf(pDst, bufferSize, "CometName_%s", pCometName);
}
