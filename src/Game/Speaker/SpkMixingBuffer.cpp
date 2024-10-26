#include "Game/Speaker/SpkMixingBuffer.hpp"
#include "Game/Speaker/SpkSound.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"

SpkMixingBuffer::SpkMixingBuffer(JKRHeap *pHeap) {
    for (int i = 0; i < 4; i++) {
        mSamples[i] = new(pHeap, 0) s16[0x28];
        JASCalc::bzero((void*)mSamples[i], 0x50);
    }
}

void SpkMixingBuffer::mix(s32 a1, s16 *a2, s32 a3, f32 a4, s32 a5) {
    //for (int i = 0; i < )
}

const s16* SpkMixingBuffer::getSamples(s32 idx) const {
    return mSamples[idx];
}

bool SpkMixingBuffer::update(s32 idx) {
    JASCalc::bzero((void*)mSamples[idx], 0x50);
    return JASGlobalInstance<SpkSoundHolder>::sInstance->update(idx);
}