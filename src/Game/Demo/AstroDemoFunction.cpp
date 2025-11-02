#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const char* cGrandStarReturnDemoTable[] = {
        "グランドスター１帰還",
        "グランドスター２帰還",
        "グランドスター３帰還",
        "グランドスター４帰還",
        "グランドスター５帰還",
        "グランドスター６帰還",
    };
};

namespace AstroDemoFunction {
    int getOpenedAstroDomeNum() {
        return MR::calcOpenedAstroDomeNum();
    }

    const char* getGrandStarReturnDemoName(int index) {
        return cGrandStarReturnDemoTable[index];
    }

    int getActiveGrandStarReturnDemoIndex() {
        for (int i = 0; i < sizeof(cGrandStarReturnDemoTable) / sizeof(*cGrandStarReturnDemoTable); i++) {
            if (MR::isDemoActive(cGrandStarReturnDemoTable[i])) {
                return i;
            }
        }

        return -1;
    }

    void tryRegisterDemo(LiveActor* pParam1, const char* pParam2, const JMapInfoIter& rIter) {
        if (MR::isDemoExist(pParam2) && !MR::isDemoCast(pParam1, pParam2)) {
            MR::tryRegisterDemoCast(pParam1, pParam2, rIter);
        }
    }

    void tryRegisterAstroDemoAll(LiveActor* pParam1, const JMapInfoIter& rIter) {
        AstroDemoFunction::tryRegisterGrandStarReturn(pParam1, rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "グランドスター帰還[２回目以降]", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "パワースター帰還", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタ状況説明デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタ最終決戦デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタコメット説明デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタトーチの炎説明デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタトーチの炎進捗デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタ天文台機能回復デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタキノピオ探検隊デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ルイージ失踪デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタカウントダウン開始デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタノーマルエンディング後デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタ銀河の中心説明デモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタルイージデモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "バトラーグリーンドライバ説明", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "バトラーマップレクチャー", rIter);
    }

    void tryRegisterGrandStarReturn(LiveActor* pParam1, const JMapInfoIter& rIter) {
        for (int i = 0; i < sizeof(cGrandStarReturnDemoTable) / sizeof(*cGrandStarReturnDemoTable); i++) {
            AstroDemoFunction::tryRegisterDemo(pParam1, cGrandStarReturnDemoTable[i], rIter);
        }
    }

    void tryRegisterGrandStarReturnWithFunction(LiveActor* pParam1, const JMapInfoIter& rIter, const MR::FunctorBase& rFunctor) {
        const char* pDemoName;

        for (int i = 0; i < sizeof(cGrandStarReturnDemoTable) / sizeof(*cGrandStarReturnDemoTable); i++) {
            pDemoName = cGrandStarReturnDemoTable[i];

            if (MR::isDemoExist(pDemoName) && MR::tryRegisterDemoCast(pParam1, pDemoName, rIter)) {
                MR::tryRegisterDemoActionFunctorDirect(pParam1, rFunctor, pDemoName, nullptr);
            }
        }
    }

    void tryRegisterGrandStarReturnAndSimpleCast(LiveActor* pParam1, const JMapInfoIter& rIter) {
        AstroDemoFunction::tryRegisterGrandStarReturn(pParam1, rIter);
        AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(pParam1);
    }

    void tryRegisterGrandStarReturnWithFunctionAndSimpleCast(LiveActor* pParam1, const JMapInfoIter& rIter, const MR::FunctorBase& rFunctor) {
        AstroDemoFunction::tryRegisterGrandStarReturnWithFunction(pParam1, rIter, rFunctor);
        AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(pParam1);
    }

    bool tryRegisterSimpleCastIfAstroGalaxy(LiveActor* pParam1) {
        if (MR::isEqualStageName("AstroGalaxy")) {
            MR::registerDemoSimpleCastAll(pParam1);

            return true;
        }

        return false;
    }

    void tryRegisterDemoForTico(LiveActor* pParam1, const JMapInfoIter& rIter) {
        if (tryRegisterSimpleCastIfAstroGalaxy(pParam1)) {
            s32 demoCastID = MR::getDemoCastID(rIter);
            const char* pDemoName = nullptr;

            switch (demoCastID) {
            case 0:
                pDemoName = cGrandStarReturnDemoTable[0];
                break;
            case 1:
                pDemoName = cGrandStarReturnDemoTable[2];
                break;
            case 2:
                pDemoName = cGrandStarReturnDemoTable[4];
                break;
            }

            if (pDemoName != nullptr) {
                AstroDemoFunction::tryRegisterDemo(pParam1, pDemoName, rIter);
            }
        }
    }

    void tryRegisterDemoForLuigiAndKinopio(LiveActor* pParam1, const JMapInfoIter& rIter) {
        AstroDemoFunction::tryRegisterGrandStarReturn(pParam1, rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタルイージデモ", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "バトラーグリーンドライバ説明", rIter);
        AstroDemoFunction::tryRegisterDemo(pParam1, "ロゼッタ最終決戦デモ", rIter);
    }
};
