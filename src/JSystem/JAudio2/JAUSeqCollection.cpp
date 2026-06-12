#include "JSystem/JAudio2/JAUSeqCollection.hpp"

JAUSeqCollection::JAUSeqCollection() {
    mCollectionData = nullptr;
}

void JAUSeqCollection::init(void const* param_0) {
    mCollectionData = (const JAUSeqCollectionData*)param_0;
    if (mCollectionData->field_0x0 != 'S' || mCollectionData->field_0x1 != 'C') {
        mCollectionData = nullptr;
        return;
    }

    field_0x0 = mCollectionData->field_0x2;
    field_0xc = mCollectionData->field_0x4;
    field_0x4 = &mCollectionData->field_0x8;
}

bool JAUSeqCollection::getSeqData(int param_0, int param_1, JAISeqData* param_2) {
    if (param_0 >= field_0x0) {
        return false;
    }
    u32 r29 = field_0x4[param_0];
    u32* puVar2 = (u32*)((u8*)mCollectionData + r29);
    if (param_1 >= puVar2[0]) {
        return false;
    }
    param_2->set((void*)mCollectionData, *(puVar2 + param_1 + 1));
    return true;
}

bool JAUSeqCollection::getSeqDataRegion(JAISeqDataRegion* param_0) {
    if (isValid()) {
        param_0->addr = (u8*)mCollectionData;
        param_0->size = field_0xc;
        return true;
    }
    return false;
}

JAUSeqDataMgr_SeqCollection::JAUSeqDataMgr_SeqCollection() {
    user_ = nullptr;
}

bool JAUSeqDataMgr_SeqCollection::setSeqDataUser(JAISeqDataUser* param_0) {
    user_ = param_0;
    return true;
}

int JAUSeqDataMgr_SeqCollection::releaseSeqData() {
    if (user_) {
        JAISeqDataRegion region;
        getSeqDataRegion(&region);
        int result = user_->releaseSeqData(region);
        return result;
    }
    return 2;
}

JAUSeqDataMgr_SeqCollection::SeqDataReturnValue JAUSeqDataMgr_SeqCollection::getSeqData(JAISoundID param_0, JAISeqData* param_1) {
    if (!isValid()) {
        return SeqDataReturnValue_0;
    }
    if (JAUSeqCollection::getSeqData(param_0.getGroupID(), param_0.getWaveID(), param_1)) {
        return SeqDataReturnValue_2;
    }
    return SeqDataReturnValue_0;
}

JAUSeqDataMgr_SeqCollection::~JAUSeqDataMgr_SeqCollection() {
}
