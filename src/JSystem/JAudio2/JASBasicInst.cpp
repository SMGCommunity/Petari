#include "JSystem/JAudio2/JASBasicInst.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASGadget.hpp"
#include "JSystem/JAudio2/JASInstEffect.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"


JASBasicInst::JASBasicInst() : mVolume(1.0f), mPitch(1.0f), mEffects(nullptr), mKeymapCount(0), mKeymap(nullptr) {
    JASCalc::bzero(mOscillatorData, sizeof(mOscillatorData));
}

JASBasicInst::~JASBasicInst() {
    delete[] mKeymap;
    // delete mEffects;
}

bool JASBasicInst::getParam(int pitch, int velocity, JASInstParam* pParams) const {
    pParams->_14 = 0;
    pParams->_24 = false;
    pParams->mOscillatorData = (JASOscillator::Data**)&mOscillatorData;
    pParams->mNumOscillators = 2;
    pParams->mVolume = mVolume;
    pParams->mPitch = mPitch;

    for (JASPtrListAdaptor< JASInstEffect >* it = mEffects.begin(); it != nullptr; it = it->getNext()) {
        it->get()->effect(pitch, velocity, pParams);
    }

    const TKeymap* keyMap = nullptr;
    for (int i = 0; i < mKeymapCount; i++) {
        if (pitch <= mKeymap[i].mHighKey) {
            keyMap = &mKeymap[i];
            break;
        }
    }

    if (keyMap == nullptr) {
        return false;
    }

    for (int i = 0; i < keyMap->getVeloRegionCount(); i++) {
        const TVelomap* veloMap = keyMap->getVeloRegion(i);
        if (velocity <= veloMap->_0) {
            pParams->mVolume *= veloMap->mVolume;
            pParams->mPitch *= veloMap->mPitch;
            pParams->_18 = veloMap->_4;
            return true;
        }
    }

    return false;
}

void JASBasicInst::setKeyRegionCount(u32 count, JKRHeap* pHeap) {
    delete[] mKeymap;
    mKeymap = new (pHeap, 0) TKeymap[count];
    mKeymapCount = count;
}

void JASBasicInst::appendEffect(JASPtrListAdaptor< JASInstEffect >* p) {
    if (mEffects.isEmpty()) {
        mEffects.mList = p;
        return;
    }

    JASPtrListAdaptor< JASInstEffect >* it = mEffects.begin();
    for (; it->getNext() != nullptr; it = it->getNext()) {
    }
    it->mNext = p;
}

void JASBasicInst::setOsc(int index, JASOscillator::Data const* pData) {
    mOscillatorData[index] = pData;
}

JASBasicInst::TKeymap* JASBasicInst::getKeyRegion(int index) {
    if (index >= mKeymapCount) {
        return nullptr;
    }

    return &mKeymap[index];
}

JASBasicInst::TKeymap* JASBasicInst::getKeyRegion(int index) const {
    if (index >= mKeymapCount) {
        return nullptr;
    }

    return &mKeymap[index];
}

JASBasicInst::TKeymap::~TKeymap() {
    delete[] mVelomap;
}

void JASBasicInst::TKeymap::setVeloRegionCount(u32 count, JKRHeap* pHeap) {
    delete[] mVelomap;
    mVelomap = new (pHeap, 0) TVelomap[count];
    mVelomapCount = count;
}

JASBasicInst::TVelomap* JASBasicInst::TKeymap::getVeloRegion(int index) {
    if (index >= mVelomapCount) {
        return nullptr;
    }
    return &mVelomap[index];
}

const JASBasicInst::TVelomap* JASBasicInst::TKeymap::getVeloRegion(int index) const {
    if (index >= mVelomapCount) {
        return nullptr;
    }
    return &mVelomap[index];
}
