#include "Game/AudioLib/ExAudSceneMgr.hpp"
#include "Game/Util/FileUtil.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JAudio2/JASWaveInfo.hpp"
#include "JSystem/JAudio2/JAUSectionHeap.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "Kamek.hpp"
#include "Karipon/System/ByamlIter.hpp"
#include <cstddef>
#include <cstdio>

ExAudSceneMgr::ExAudSceneMgr(JAUSectionHeap* pSectionHeap) : AudSceneMgr(pSectionHeap), mWaveTable(), mWaveSetStageIt(), mWaveSetScenarioIt() {
    void* pData = MR::receiveFile("/Debug/GameAudioWaveTable.byaml");
    mWaveTable.attach(static_cast<u8*>(pData));
}

void ExAudSceneMgr::loadStaticResource() {
    ByamlIter staticResourceIt = getRootIter("StaticResource");
    loadWaveSet(staticResourceIt);
}

bool ExAudSceneMgr::isLoadDoneStaticResource() const {
    ByamlIter staticResourceIt = getRootIter("StaticResource");
    return isLoadDoneWaveSet(staticResourceIt);
}

void ExAudSceneMgr::loadStageResource(const char* pStageName) {
    mIsNewPlayerMode = mPlayerMode != mPrevPlayerMode;

    if (mIsNewPlayerMode) {
        mSectionHeap->eraseWaveArc(34, 2);
        mSectionHeap->eraseWaveArc(34, 4);
        loadPlayerResource();
        mPrevPlayerMode = mPlayerMode;
    }

    ByamlIter waveSetStageIt = getRootIter("StageResource").getIterByKey(pStageName).getIterByKey("Common");

    if (waveSetStageIt != mWaveSetStageIt) {
        eraseWaveSet(mWaveSetStageIt);
        loadWaveSet(waveSetStageIt);
        mWaveSetStageIt = waveSetStageIt;
    }
}

bool ExAudSceneMgr::isLoadDoneStageResource() {
    return isLoadDoneWaveSet(mWaveSetStageIt) && isPlayerResourceLoaded();
}

void ExAudSceneMgr::loadScenarioResource(const char* pStageName, s32 scenarioNo) {
    char key[16];
    snprintf(key, sizeof(key), "Scenario%d", scenarioNo);

    ByamlIter waveSetScenarioIt = getRootIter("StageResource").getIterByKey(pStageName).getIterByKey(key);

    if (waveSetScenarioIt != mWaveSetScenarioIt) {
        eraseWaveSet(mWaveSetScenarioIt);
        loadWaveSet(waveSetScenarioIt);
        mWaveSetScenarioIt = waveSetScenarioIt;
    }
}

bool ExAudSceneMgr::isLoadDoneScenarioResource() const {
    return isLoadDoneWaveSet(mWaveSetScenarioIt);
}

void ExAudSceneMgr::loadWaveSet(const ByamlIter& rIter) {
    if (!rIter.isValid()) {
        return;
    }

    s32 size = rIter.getSize();

    for (s32 i = 0; i < size; i++) {
        const char* pName = nullptr;
        rIter.tryGetValueByIndex(&pName, i);

        if (pName == nullptr || pName[0] == '\0') {
            continue;
        }

        s32 bankNo = findWaveBankNo(pName);
        if (bankNo != -1) {
            mSectionHeap->loadWaveArc(bankNo);
            OSReport("[%s] Loaded %s\n", __FILE__, pName);
        } else {
            OSReport("[%s] Failed loading %s\n", __FILE__, pName);
        }
    }
}

void ExAudSceneMgr::eraseWaveSet(const ByamlIter& rIter) {
    if (!rIter.isValid()) {
        return;
    }

    s32 size = rIter.getSize();

    for (s32 i = 0; i < size; i++) {
        const char* pName = nullptr;
        rIter.tryGetValueByIndex(&pName, i);

        if (pName == nullptr || pName[0] == '\0') {
            continue;
        }

        s32 bankNo = findWaveBankNo(pName);
        if (bankNo != -1) {
            mSectionHeap->eraseWaveArc(bankNo);
            OSReport("[%s] Erased %s\n", __FILE__, pName);
        } else {
            OSReport("[%s] Failed erasing %s\n", __FILE__, pName);
        }
    }
}

bool ExAudSceneMgr::isLoadDoneWaveSet(const ByamlIter& rIter) const {
    if (!rIter.isValid()) {
        return true;
    }

    s32 size = rIter.getSize();

    for (s32 i = 0; i < size; i++) {
        const char* pName = nullptr;
        rIter.tryGetValueByIndex(&pName, i);

        if (pName == nullptr || pName[0] == '\0') {
            continue;
        }

        s32 bankNo = findWaveBankNo(pName);
        if (bankNo != -1 && !mSectionHeap->isWaveLoaded(bankNo, 0)) {
            return false;
        }
    }

    return true;
}

s32 ExAudSceneMgr::findWaveBankNo(const char* pWaveArcName) const {
    char filePath[0x200];
    snprintf(filePath, sizeof(filePath), "/AudioRes/Waves/%s", pWaveArcName);

    char filePathLang[0x200];
    MR::makeFileNameConsideringLanguage(filePathLang, sizeof(filePathLang), filePath);

    s32 entryNum = DVDConvertPathToEntrynum(filePathLang);
    if (entryNum == -1) {
        return -1;
    }

    for (u8 bankNo = 0; bankNo < 0xFF; bankNo++) {
        if (!mSectionHeap->getSectionData().registeredWaveBankTables.test(bankNo)) {
            continue;
        }

        JASWaveBank* pWaveBank = mSectionHeap->getWaveBankTable().getWaveBank(bankNo);
        if (pWaveBank == nullptr) {
            continue;
        }

        for (u32 i = 0; i < pWaveBank->getArcCount(); i++) {
            JASWaveArc* pWaveArc = pWaveBank->getWaveArc(i);

            if (pWaveArc->mEntryNum == entryNum) {
                return bankNo;
            }
        }
    }

    return -1;
}

namespace {
    static ExAudSceneMgr* createExAudSceneMgr(JAUSectionHeap* pSectionHeap, JKRHeap* pHeap) {
        return new (pHeap, 4) ExAudSceneMgr(pSectionHeap);
    }

    static void loadStaticWaveData(ExAudSceneMgr* pAudSceneMgr) {
        pAudSceneMgr->loadStaticResource();
    }

    static bool isLoadDoneStaticWaveData(ExAudSceneMgr* pAudSceneMgr) {
        return pAudSceneMgr->isLoadDoneStaticResource();
    }

    static void loadStageResource(ExAudSceneMgr* pAudSceneMgr, const char* pSceneName, const char* pStageName) {
        pAudSceneMgr->loadStageResource(pStageName);
    }

    static bool isLoadDoneStageWaveData(ExAudSceneMgr* pAudSceneMgr) {
        return pAudSceneMgr->isLoadDoneStageResource();
    }

    static void loadScenarioResource(ExAudSceneMgr* pAudSceneMgr, const char* pSceneName, const char* pStageName, s32 scenarioNo) {
        pAudSceneMgr->loadScenarioResource(pStageName, scenarioNo);
    }

    static bool isLoadDoneScenarioWaveData(ExAudSceneMgr* pAudSceneMgr) {
        return pAudSceneMgr->isLoadDoneScenarioResource();
    }
} // namespace

extern kmSymbol __ct__9AudSystemFP14JAUSectionHeapP10JKRArchiveP10JKRArchiveP10JKRArchive;
kmWrite32(&__ct__9AudSystemFP14JAUSectionHeapP10JKRArchiveP10JKRArchiveP10JKRArchive + 0x4B0, PPC_LWZ(3, 0x838, 24));
kmCall(&__ct__9AudSystemFP14JAUSectionHeapP10JKRArchiveP10JKRArchiveP10JKRArchive + 0x4B4, createExAudSceneMgr);
kmWrite32(&__ct__9AudSystemFP14JAUSectionHeapP10JKRArchiveP10JKRArchiveP10JKRArchive + 0x4B8, PPC_B(0x14));

extern kmSymbol loadStaticWaveData__16AudSystemWrapperFv;
kmBranch(&loadStaticWaveData__16AudSystemWrapperFv + 0x18, loadStaticWaveData);

extern kmSymbol isLoadDoneStaticWaveData__16AudSystemWrapperCFv;
kmBranch(&isLoadDoneStaticWaveData__16AudSystemWrapperCFv + 0x28, isLoadDoneStaticWaveData);

extern kmSymbol loadStageWaveData__16AudSystemWrapperFPCcPCcb;
kmCall(&loadStageWaveData__16AudSystemWrapperFPCcPCcb + 0x5C, loadStageResource);

extern kmSymbol isLoadDoneStageWaveData__16AudSystemWrapperCFv;
kmBranch(&isLoadDoneStageWaveData__16AudSystemWrapperCFv + 0x28, isLoadDoneStageWaveData);

extern kmSymbol loadScenarioWaveData__16AudSystemWrapperFPCcPCcl;
kmBranch(&loadScenarioWaveData__16AudSystemWrapperFPCcPCcl + 0x18, loadScenarioResource);

extern kmSymbol isLoadDoneScenarioWaveData__16AudSystemWrapperCFv;
kmBranch(&isLoadDoneScenarioWaveData__16AudSystemWrapperCFv + 0x28, isLoadDoneScenarioWaveData);
