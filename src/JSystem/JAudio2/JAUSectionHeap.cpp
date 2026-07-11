#include "JSystem/JAudio2/JAUSectionHeap.hpp"
#include "JSystem/JAudio2/JASBNKParser.hpp"
#include "JSystem/JAudio2/JASBankTable.hpp"
#include "JSystem/JAudio2/JASBasicWaveBank.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"
#include "JSystem/JAudio2/JASHeapCtrl.hpp"
#include "JSystem/JAudio2/JASResArcLoader.hpp"
#include "JSystem/JAudio2/JASVoiceBank.hpp"
#include "JSystem/JAudio2/JASWSParser.hpp"
#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JAudio2/JAUSeqCollection.hpp"
#include "JSystem/JAudio2/JAUSoundInfo.hpp"
#include "JSystem/JAudio2/JAUSoundTable.hpp"
#include "JSystem/JAudio2/JAUStreamFileTable.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include <cstring>
#include <revolution/dvd.h>

namespace {
    class TPushCurrentHeap {
    public:
        TPushCurrentHeap(JKRHeap* heap) {
            mHeap = JKRSetCurrentHeap(heap);
        }
        ~TPushCurrentHeap() {
            JKRSetCurrentHeap(mHeap);
        }

    private:
        JKRHeap* mHeap;
    };

    class TStreamDataMgr : public JAIStreamDataMgr {
    public:
        TStreamDataMgr(const void* param_0) {
            JAUStreamFileTable stack_14;
            stack_14.init(param_0);
            if (!stack_14.isValid()) {
                field_0x4 = 0;
                field_0x8 = nullptr;
                return;
            }
            field_0x4 = stack_14.getNumFiles();
            field_0x8 = new s32[field_0x4];
            if (!field_0x8) {
                field_0x4 = 0;
                return;
            }
            for (u32 i = 0; i < field_0x4; i++) {
                field_0x8[i] = DVDConvertPathToEntrynum(stack_14.getFilePath(i));
            }
        }
        virtual s32 getStreamFileEntry(JAISoundID id) {
            u32 short_id = id.getWaveID();
            if (short_id >= field_0x4) {
                return -1;
            }
            return field_0x8[short_id];
        }

        bool isValid() {
            return field_0x4 != 0;
        }

        u32 field_0x4;
        s32* field_0x8;
    };
}  // namespace

namespace {
    bool JKRSolidHeap_isEmpty(JKRSolidHeap* pHeap) {
        u32 beforeSize = pHeap->getFreeSize();
        pHeap->freeAll();
        return pHeap->getFreeSize() - beforeSize == 0;
    }
}  // namespace

JAUSection::TSectionData::TSectionData() {
    resetRegisteredWaveBankTables();
    resetRegisteredBankTables();
    mBstDst = nullptr;
    mBstnDst = nullptr;
    field_0x80 = nullptr;
    field_0xa0 = 0;
    field_0x9c = 0;
}

void JAUSection::TSectionData::resetRegisteredBankTables() {
    for (u32 i = 0; i < 255; i++) {
        registeredBankTables.reset(i);
    }
}

void JAUSection::TSectionData::resetRegisteredWaveBankTables() {
    for (u32 i = 0; i < 255; i++) {
        registeredWaveBankTables.reset(i);
    }
}

JAUSection::JAUSection(JAUSectionHeap* param_0, u32 param_1, s32 param_2) : JSULink< JAUSection >(this), field_0x28(param_1), sectionHeap_(param_0) {
    buildingBankTable_ = nullptr;
    field_0x2c = 1;
    data_.field_0x98 = param_2;
    if (this != sectionHeap_) {
        data_.field_0x00.setSeqDataArchive(sectionHeap_->sectionHeapData_.seqDataBlocks.getSeqDataArchive());
    }
}

void JAUSection::finishBuild() {
    { TPushCurrentHeap push(getHeap_()); }
    data_.field_0x98 -= getHeap_()->getFreeSize();
    getHeap_()->freeTail();
    field_0x2c = 0;
}

void JAUSection::dispose() {
    if (data_.mBstDst) {
        sectionHeap_->sectionHeapData_.soundTable->~JAUSoundTable();
        sectionHeap_->sectionHeapData_.soundTable = nullptr;
    }
    if ((data_.mBstnDst)) {
        sectionHeap_->sectionHeapData_.soundNameTable->~JAUSoundNameTable();
        sectionHeap_->sectionHeapData_.soundNameTable = nullptr;
    }
}

JAUSoundTable* JAUSection::newSoundTable(void const* bst, u32 param_1, bool param_2) {
    {
        TPushCurrentHeap push(getHeap_());
        void const* bstDst = bst;
        if (param_1) {
            bstDst = newCopy(bst, param_1, 4);
        }
        JAUSoundTable* soundTable = new JAUSoundTable(param_2);
        soundTable->init(bstDst);
        sectionHeap_->sectionHeapData_.soundTable = soundTable;
        data_.mBstDst = bstDst;
    }
    return sectionHeap_->sectionHeapData_.soundTable;
}

JAUSoundNameTable* JAUSection::newSoundNameTable(void const* bstn, u32 param_1, bool param_2) {
    {
        TPushCurrentHeap push(getHeap_());
        void const* bstnDst = bstn;
        if (param_1) {
            bstnDst = newCopy(bstn, param_1, 4);
        }
        JAUSoundNameTable* soundNameTable = new JAUSoundNameTable(param_2);
        soundNameTable->init(bstnDst);
        sectionHeap_->sectionHeapData_.soundNameTable = soundNameTable;
        data_.mBstnDst = bstnDst;
    }
    return sectionHeap_->sectionHeapData_.soundNameTable;
}

JAIStreamDataMgr* JAUSection::newStreamFileTable(void const* param_0, bool param_1) {
    {
        TPushCurrentHeap push(getHeap_());
        JAIStreamDataMgr* r28 = nullptr;
        if (param_1) {
            TStreamDataMgr* r26 = new TStreamDataMgr(param_0);
            if (r26->isValid()) {
                r28 = r26;
            }
        } else {
            JAUStreamDataMgr_StreamFileTable* r25 = new JAUStreamDataMgr_StreamFileTable();
            r25->init(param_0);
            if (r25->isValid()) {
                r28 = r25;
            }
        }
        sectionHeap_->sectionHeapData_.streamDataMgr_ = r28;
    }
    return sectionHeap_->sectionHeapData_.streamDataMgr_;
}

JAISeqDataMgr* JAUSection::newSeSeqCollection(void const* bsc, u32 param_1) {
    if (param_1) {
        bsc = newCopy(bsc, param_1, 4);
    }

    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataMgr_SeqCollection* seSeqDataMgr = new JAUSeqDataMgr_SeqCollection();
        seSeqDataMgr->init(bsc);
        sectionHeap_->sectionHeapData_.seSeqDataMgr_ = seSeqDataMgr;
        data_.field_0x80 = seSeqDataMgr;
    }
    return sectionHeap_->sectionHeapData_.seSeqDataMgr_;
}

u8* JAUSection::newStaticSeqDataBlock_(JAISoundID param_0, u32 size) {
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock* seqDataBlock = new JAUSeqDataBlock();
        if (!seqDataBlock) {
            return nullptr;
        }
        JSULink< JAUSeqDataBlock >* link = new JSULink< JAUSeqDataBlock >(seqDataBlock);
        if (!link) {
            return nullptr;
        }
        u8* r28 = new (0x20) u8[size];
        if (!r28) {
            return nullptr;
        }
        seqDataBlock->region.addr = r28;
        seqDataBlock->region.size = size;
        seqDataBlock->mSoundID = param_0;
        JASCriticalSection cs;
        if (data_.field_0x00.appendDynamicSeqDataBlock(seqDataBlock)) {
            data_.field_0x28.append(link);
            return r28;
        }
    }
    return nullptr;
}

bool JAUSection::newStaticSeqData(JAISoundID param_0, void const* param_1, u32 param_2) {
    {
        u8* r30 = newStaticSeqDataBlock_(param_0, param_2);
        if (r30) {
            memcpy(r30, param_1, param_2);
            return true;
        }
    }
    return false;
}

bool JAUSection::newStaticSeqData(JAISoundID param_0) {
    JKRArchive* seqArchive = data_.field_0x00.getSeqDataArchive();
    JAUSoundInfo* soundInfo = JASGlobalInstance< JAUSoundInfo >::getInstance();
    if (!soundInfo) {
        return false;
    }
    u16 r26 = soundInfo->getBgmSeqResourceID(param_0);
    u32 r25 = JASResArcLoader::getResSize(seqArchive, r26);
    u8* r24 = newStaticSeqDataBlock_(0xffffffff, r25);
    if (r24) {
        data_.field_0x00.loadDynamicSeq(param_0, true, sectionHeap_->sectionHeapData_.seqDataUser);
        return true;
    }
    return false;
}

void* JAUSection::loadDVDFile(const char* pFileName, bool direction, JKRExpandSwitch expandSwitch) {
    {
        TPushCurrentHeap push(getHeap_());
        return JKRDvdToMainRam(pFileName, nullptr, expandSwitch, 0, getHeap_(),
                               direction ? JKRDvdRipper::ALLOC_DIRECTION_BACKWARD : JKRDvdRipper::ALLOC_DIRECTION_FORWARD, 0, nullptr, nullptr);
    }
}

void* JAUSection::newCopy(void const* param_0, u32 param_1, s32 param_2) {
    u8* r31 = new (getHeap_(), param_2) u8[param_1];
    if (r31) {
        memcpy(r31, param_0, param_1);
    }
    return r31;
}

JASWaveBank* JAUSection::newWaveBank(u32 bank_no, void const* param_1) {
    {
        TPushCurrentHeap push(getHeap_());
        s32 r27 = getHeap_()->getFreeSize();
        JASWaveBank* waveBank = JASWSParser::createWaveBank(param_1, getHeap_());
        if (waveBank) {
            sectionHeap_->getWaveBankTable().registWaveBank(bank_no, waveBank);
            data_.registeredWaveBankTables.set(bank_no, true);
            data_.field_0xa0 += r27 - getHeap_()->getFreeSize();
            return waveBank;
        }
    }
    return nullptr;
}

bool JAUSection::loadWaveArc(u32 bankNo, u32 param_1) {
    if (data_.registeredWaveBankTables.test(bankNo)) {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bankNo);
        if (waveBank) {
            for (u32 i = 0; i < waveBank->getArcCount(); i++) {
                if (param_1 & 1 << i) {
                    JASWaveArc* waveArc = waveBank->getWaveArc(i);
                    waveArc->load(nullptr);
                }
            }
            return true;
        }
    }
    return false;
}

bool JAUSection::loadWaveArc(u32 bankNo) {
    if (data_.registeredWaveBankTables.test(bankNo)) {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bankNo);
        if (waveBank) {
            for (u32 i = 0; i < waveBank->getArcCount(); i++) {
                JASWaveArc* waveArc = waveBank->getWaveArc(i);
                waveArc->load(nullptr);
            }
            return true;
        }
    }
    return false;
}

bool JAUSection::eraseWaveArc(u32 bankNo, u32 param_1) {
    if (data_.registeredWaveBankTables.test(bankNo)) {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bankNo);
        if (waveBank) {
            for (u32 i = 0; i < waveBank->getArcCount(); i++) {
                if (param_1 & 1 << i) {
                    JASWaveArc* waveArc = waveBank->getWaveArc(i);
                    waveArc->erase();
                }
            }
            return true;
        }
    }
    return false;
}

bool JAUSection::eraseWaveArc(u32 bankNo) {
    if (data_.registeredWaveBankTables.test(bankNo)) {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(bankNo);
        if (waveBank) {
            for (u32 i = 0; i < waveBank->getArcCount(); i++) {
                JASWaveArc* waveArc = waveBank->getWaveArc(i);
                waveArc->erase();
            }
            return true;
        }
    }
    return false;
}

JASBank* JAUSection::newBank(void const* param_0, u32 param_1) {
    JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(param_1);
    {
        TPushCurrentHeap push(getHeap_());
        u32 bank_no = JASBNKParser::getBankNumber(param_0);
        s32 r25 = getHeap_()->getFreeSize();
        JASBank* bank = JASBNKParser::createBank(param_0, getHeap_());
        if (bank) {
            if (buildingBankTable_) {
                buildingBankTable_->registBank(bank_no, bank);
            } else {
                JASDefaultBankTable::getInstance()->registBank(bank_no, bank);
                data_.registeredBankTables.set(bank_no, true);
            }
            bank->assignWaveBank(waveBank);
            data_.field_0x9c += r25 - getHeap_()->getFreeSize();
            return bank;
        }
    }
    return 0;
}

JASVoiceBank* JAUSection::newVoiceBank(u32 bank_no, u32 param_1) {
    {
        JASWaveBank* waveBank = sectionHeap_->getWaveBankTable().getWaveBank(param_1);
        TPushCurrentHeap push(getHeap_());
        JASBank* voiceBank = new JASVoiceBank();
        if (voiceBank) {
            if (buildingBankTable_) {
                buildingBankTable_->registBank(bank_no, voiceBank);
            } else {
                JASDefaultBankTable::getInstance()->registBank(bank_no, voiceBank);
                data_.registeredBankTables.set(bank_no, true);
            }
            voiceBank->assignWaveBank(waveBank);
            return (JASVoiceBank*)voiceBank;
        }
    }
    return nullptr;
}

bool JAUSection::beginNewBankTable(u32 param_0, u32 param_1) {
    JAUBankTableLink* bankTableLink = nullptr;
    {
        TPushCurrentHeap push(getHeap_());
        JASBank** r26 = new JASBank*[param_1];
        if (r26 != nullptr) {
            bankTableLink = new JAUBankTableLink(param_0, r26, param_1);
            if (bankTableLink != nullptr) {
                buildingBankTable_ = bankTableLink;
            }
        }
    }
    return bankTableLink;
}

JAUBankTable* JAUSection::endNewBankTable() {
    JAUBankTable* r29 = nullptr;
    if (buildingBankTable_) {
        data_.field_0x74.appendBankTable(buildingBankTable_);
        buildingBankTable_ = nullptr;
        r29 = buildingBankTable_;
    }
    return r29;
}

JAUSectionHeap::TSectionHeapData::TSectionHeapData() {
    seSeqDataMgr_ = nullptr;
    streamDataMgr_ = nullptr;
    soundTable = nullptr;
    soundNameTable = nullptr;
    seqDataUser = nullptr;
}

void JAUSectionHeap::setSeqDataArchive(JKRArchive* param_0) {
    sectionHeapData_.seqDataBlocks.setSeqDataArchive(param_0);
    JSULink< JAUSection >* link;
    for (link = mSectionList.getFirst(); link; link = link->getNext()) {
        link->getObject()->data_.field_0x00.setSeqDataArchive(param_0);
    }
}

bool JAUSectionHeap::loadDynamicSeq(JAISoundID param_0, bool param_1) {
    return sectionHeapData_.seqDataBlocks.loadDynamicSeq(param_0, param_1, sectionHeapData_.seqDataUser);
}

u32 JAUSectionHeap::releaseIdleDynamicSeqDataBlock() {
    return sectionHeapData_.seqDataBlocks.releaseIdleDynamicSeqDataBlock(sectionHeapData_.seqDataUser);
}

static JAUSectionHeap* JAUNewSectionHeap(JKRSolidHeap* heap, bool param_1) NO_INLINE {
    TPushCurrentHeap push(heap);
    s32 r29 = heap->getFreeSize();
    JAUSectionHeap* sectionHeap = new JAUSectionHeap(heap, param_1, r29);
    return sectionHeap;
}

JAUSectionHeap* JAUNewSectionHeap(bool param_0) {
    s32 freeSize = JASDram->getFreeSize();
    JKRSolidHeap* sectionHeap = JKRCreateSolidHeap(freeSize, JASDram, true);
    return JAUNewSectionHeap(sectionHeap, param_0);
}

JAUSectionHeap::JAUSectionHeap(JKRSolidHeap* param_0, bool param_1, s32 param_2)
    : JAUSection(this, 0, param_2), JASGlobalInstance< JAUSectionHeap >(param_1), mHeap(param_0) {
    field_0xe4 = 0;
    mSectionList.append(this);
}

JAUSection* JAUSectionHeap::getOpenSection() {
    return mSectionList.getLast()->getObject();
}

JAUSection* JAUSectionHeap::getSection(int param_0) {
    JSULink< JAUSection >* link = mSectionList.getFirst();
    while (param_0 > 0) {
        JSULink< JAUSection >* link2 = link->getNext();
        if (link2 == nullptr) {
            return nullptr;
        }
        link = link2;
        param_0--;
    }
    return link->getObject();
}

bool JAUSectionHeap::setSeqDataUser(JAISeqDataUser* param_0) {
    sectionHeapData_.seqDataUser = param_0;
    return true;
}

bool JAUSectionHeap::newDynamicSeqBlock(u32 size) {
    {
        TPushCurrentHeap push(getHeap_());
        JAUSeqDataBlock* seqDataBlock = new JAUSeqDataBlock();
        if (!seqDataBlock) {
            return false;
        }
        JSULink< JAUSeqDataBlock >* link = new JSULink< JAUSeqDataBlock >(seqDataBlock);
        if (!link) {
            return false;
        }
        u8* r25 = new (0x20) u8[size];
        if (!r25) {
            return false;
        }
        seqDataBlock->region.addr = r25;
        seqDataBlock->region.size = size;
        seqDataBlock->mSoundID.setAnonymous();
        JASCriticalSection cs;
        if (sectionHeap_->sectionHeapData_.seqDataBlocks.appendDynamicSeqDataBlock(seqDataBlock)) {
            return true;
        }
    }
    return false;
}

int JAUSectionHeap::newDynamicSeqBlock(u32 size, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!newDynamicSeqBlock(size)) {
            break;
        }
    }
    return i;
}

JAISeqDataMgr::SeqDataReturnValue JAUSectionHeap::getSeqData(JAISoundID param_0, JAISeqData* param_1) {
    const int JAI_ASYNC_RESULT_RETRY = 1;
    for (JSULink< JAUSection >* link = mSectionList.getFirst(); link; link = link->getNext()) {
        s32 result = link->getObject()->data_.field_0x00.getSeqData(param_0, sectionHeapData_.seqDataUser, param_1, false);
        if (result == 2) {
            return JAISeqDataMgr::SeqDataReturnValue_2;
        }
    }
    return (SeqDataReturnValue)sectionHeapData_.seqDataBlocks.getSeqData(param_0, sectionHeapData_.seqDataUser, param_1, true);
}

int JAUSectionHeap::releaseSeqData() {
    return 0;
}

bool JAUSectionHeap::isWaveLoaded(u32 param_0, u32 param_1) {
    JASWaveBank* waveBank = sectionHeapData_.waveBankTable.getWaveBank(param_0);
    if (waveBank != nullptr) {
        JASWaveArc* waveArc = waveBank->getWaveArc(param_1);
        if (waveArc != nullptr) {
            if (waveArc->mStatus == 2) {
                return true;
            }
        }
    }
    return false;
}
