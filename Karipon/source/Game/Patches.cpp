#include "Game/NPC/TalkMessageInfo.hpp"
#include "Kamek.hpp"
#include "Macros.hpp"
#include "revolution/types.h"
#include <cstdlib>
#include <cstring>

// 360 first-person camera
#ifdef PATCH_360_FP_CAM
extern kmSymbol calc__16CameraSubjectiveFv;
kmWrite32(&calc__16CameraSubjectiveFv + 0x1AC, PPC_NOP);
#endif

// Remove Korean regional diffs
#ifdef PATCH_KOR_DIFFS
namespace {
    static const char cTxtGalaxyNameU[] = "TxtGalaxyNameU";
    static const char cTxtGalaxyName[] = "TxtGalaxyName";

    // clang-format off
#ifdef __MWERKS__
    asm void getTxtGalaxyNameU() {
        lis  r4, cTxtGalaxyNameU@ha
        addi r4, r4, cTxtGalaxyNameU@l
        blr
    }
#else
    static void getTxtGalaxyNameU() {}
#endif

#ifdef __MWERKS__
    asm void getTxtGalaxyName() {
        lis  r4, cTxtGalaxyName@ha
        addi r4, r4, cTxtGalaxyName@l
        blr
    }
#else
    static void getTxtGalaxyName() {}
#endif
    // clang-format on
} // namespace

kmCall(0x80360810, getTxtGalaxyNameU);
kmCall(0x80360818, getTxtGalaxyName);

// Load USA panes
namespace {
    static const char cLanguagePrefix[] = "UsEn";

    static const char* getCurrentLanguagePrefix() {
        return cLanguagePrefix;
    }
}

kmCall(0x80369F48, getCurrentLanguagePrefix);

// Remove GalaxyMap's A button
kmWrite32(0x8035962C, PPC_B(0x5C));
kmWrite32(0x8035999C, PPC_B(0x14));
kmWrite32(0x80359B98, PPC_B(0x14));
kmWrite32(0x80359D48, PPC_B(0x14));
kmWrite32(0x80359F34, PPC_B(0x14));
kmWrite32(0x8035A8AC, PPC_B(0x14));
kmWrite32(0x8035AA18, PPC_BLR);

// Use normal text colors
kmWrite32(0x803509B0, PPC_B(0x10));
#endif

// Missing text patch
#ifdef PATCH_MISSING_MSG
namespace {
    static wchar_t sErrorMessageBuffer[128];

    // clang-format off
#ifdef __MWERKS__
    static asm void saveRegister() {
        mr r12, r5 // const char* pLabel
        mr r4, r5
        blr
    }
#else
    static void saveRegister() {}
#endif
    // clang-format on

    static const bool getErrorMessage() {
        register const char* pLabel;
        register TalkMessageInfo* pTalkMsgInfo;

        GET_REGISTER(pLabel, r12);
        GET_REGISTER(pTalkMsgInfo, r31);

        u32 len = strlen(pLabel) + 1;
        if (len > 127) {
            len = 127;
        }

        mbstowcs(sErrorMessageBuffer, pLabel, len);
        sErrorMessageBuffer[ARRAY_LEN(sErrorMessageBuffer) - 1] = '\0';
        pTalkMsgInfo->_0 = reinterpret_cast<u8*>(sErrorMessageBuffer);
        return false;
    }
} // namespace

extern kmSymbol getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc;
kmCall(&getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc + 0x14, saveRegister);
kmCall(&getMessageDirect__11MessageDataCFP15TalkMessageInfoPCc + 0x5C, getErrorMessage);
#endif

// Remove the comet retry button
#ifdef PATCH_NO_COMET_RETRY
extern kmSymbol init__9GameSceneFv;
kmWrite32(&init__9GameSceneFv + 0x170, PPC_LI(3, 0)); // li r3, 0

extern kmSymbol requestGalaxyMove__20GameSequenceProgressFRC18GalaxyMoveArgument;
kmWrite32(&requestGalaxyMove__20GameSequenceProgressFRC18GalaxyMoveArgument + 0x1EC, PPC_LI(3, 0)); // li r3, 0
#endif

// Instant pause menus
#ifdef PATCH_QUICK_PAUSE
extern kmSymbol isPermitToPlusPause__24PauseButtonCheckerInGameCFv;
kmWrite16(&isPermitToPlusPause__24PauseButtonCheckerInGameCFv + 0x2E, 0x1);

extern kmSymbol isPermitToMinusPause__24PauseButtonCheckerInGameCFv;
kmWrite16(&isPermitToMinusPause__24PauseButtonCheckerInGameCFv + 0x2E, 0x1);
#endif
