#include "JSystem/JAudio2/JAUAudioArcLoader.hpp"
#include "JSystem/JAudio2/JAISeMgr.hpp"
#include "JSystem/JAudio2/JASReport.hpp"
#include "JSystem/JAudio2/JAUSectionHeap.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"

JAUAudioArcLoader::JAUAudioArcLoader(JAUSection* pSection) {
    mSection = pSection;
}

bool JAUAudioArcLoader::load(void const* pData) {
    return parse(pData);
}

bool JAUAudioArcLoader::load(char const* pFileName) {
    void const* data = mSection->loadDVDFile(pFileName, true, EXPAND_SWITCH_UNKNOWN0);
    return data != nullptr && load(data);
}

void JAUAudioArcLoader::readWS(u32 param_0, void const* pData, u32 param_2) {
    mSection->newWaveBank(param_0, pData);
    if (param_2 != 0) {
        mSection->loadWaveArc(param_0, param_2);
    }
}

void JAUAudioArcLoader::readBNK(u32 param_0, void const* pData) {
    mSection->newBank(pData, param_0);
}

void JAUAudioArcLoader::readBSC(void const* pData, u32 param_1) {
    mSection->newSeSeqCollection(pData, param_1);
}

void JAUAudioArcLoader::readBST(void const* pData, u32 param_1) {
    mSection->newSoundTable(pData, param_1, true);
}

void JAUAudioArcLoader::readBSTN(void const* pData, u32 param_1) {
    mSection->newSoundNameTable(pData, param_1, true);
}

void JAUAudioArcLoader::readBMS(u32 param_0, void const* pData, u32 param_2) {
    mSection->newStaticSeqData(param_0, pData, param_2);
}

void JAUAudioArcLoader::readBMS_fromArchive(u32 param_0) {
    mSection->newStaticSeqData(param_0);
}

void JAUAudioArcLoader::newVoiceBank(u32 param_0, u32 param_1) {
    mSection->newVoiceBank(param_1, param_0);
}

void JAUAudioArcLoader::newDynamicSeqBlock(u32 param_0) {
    JAUSectionHeap* sectionHeap = mSection->asSectionHeap();
    sectionHeap->newDynamicSeqBlock(param_0);
}

void JAUAudioArcLoader::readBSFT(void const* pData) {
    mSection->newStreamFileTable(pData, true);
}

void JAUAudioArcLoader::beginBNKList(u32 param_0, u32 param_1) {
    mSection->beginNewBankTable(param_0, param_1);
}

void JAUAudioArcLoader::endBNKList() {
    mSection->endNewBankTable();
}

void JAUAudioArcLoader::readMaxSeCategory(int category, int maxActiveSe, int maxInactiveSe) {
    {
        JAISeMgr* seMgr = JASGlobalInstance< JAISeMgr >::getInstance();
        if (seMgr != nullptr) {
            seMgr->getCategory(category)->setMaxActiveSe(maxActiveSe);
            seMgr->getCategory(category)->setMaxInactiveSe(maxInactiveSe);
        }
    }
}

void JAUAudioArcLoader_withoutCopy::readWS(u32 param_0, void const* param_1, u32 param_2) {
    mSection->newWaveBank(param_0, param_1);
    if (param_2) {
        mSection->loadWaveArc(param_0, param_2);
    }
    JASReport(".ws resource remains at Heap Head\n");
}

void JAUAudioArcLoader_withoutCopy::readBNK(u32 param_0, void const* pData) {
    mSection->newBank(pData, param_0);
    JASReport(".bnk resource remains at Heap Head\n");
}

void JAUAudioArcLoader_withoutCopy::readBSC(void const* pData, u32 param_1) {
    mSection->newSeSeqCollection(pData, 0);
}

void JAUAudioArcLoader_withoutCopy::readBST(void const* pData, u32 param_1) {
    mSection->newSoundTable(pData, 0, true);
}

void JAUAudioArcLoader_withoutCopy::readBSTN(void const* pData, u32 param_1) {
    mSection->newSoundNameTable(pData, 0, true);
}

void JAUAudioArcLoader_withoutCopy::readBMS(u32 param_0, void const* pData, u32 param_2) {
    mSection->newStaticSeqData(param_0, pData, 0);
}

void JAUAudioArcLoader_withoutCopy::readBMS_fromArchive(u32 param_0) {
    mSection->newStaticSeqData(param_0);
}

void JAUAudioArcLoader_withoutCopy::newVoiceBank(u32 param_0, u32 param_1) {
    mSection->newVoiceBank(param_1, param_0);
}

void JAUAudioArcLoader_withoutCopy::newDynamicSeqBlock(u32 param_0) {
    JAUSectionHeap* sectionHeap = mSection->asSectionHeap();
    sectionHeap->newDynamicSeqBlock(param_0);
}

void JAUAudioArcLoader_withoutCopy::readBSFT(void const* pData) {
    mSection->newStreamFileTable(pData, false);
}

void JAUAudioArcLoader_withoutCopy::beginBNKList(u32 param_0, u32 param_1) {
    mSection->beginNewBankTable(param_0, param_1);
}

void JAUAudioArcLoader_withoutCopy::endBNKList() {
    mSection->endNewBankTable();
}

void JAUAudioArcLoader_withoutCopy::readMaxSeCategory(int category, int maxActiveSe, int maxInactiveSe) {
    JAISeMgr* seMgr = JASGlobalInstance< JAISeMgr >::getInstance();
    if (seMgr != nullptr) {
        seMgr->getCategory(category)->setMaxActiveSe(maxActiveSe);
        seMgr->getCategory(category)->setMaxInactiveSe(maxInactiveSe);
    }
}
