#include "JSystem/JAudio2/JASDrumSet.hpp"
#include "JSystem/JAudio2/JASCalc.hpp"
#include "JSystem/JAudio2/JASInstEffect.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

JASDrumSet::JASDrumSet() {
    JASCalc::bzero(&mPercArray, PERC_MAX * sizeof(TPerc*));
}

JASDrumSet::~JASDrumSet() {
}

bool JASDrumSet::getParam(int key, int velocity, JASInstParam* pParams) const {
    if ((u32)key >= PERC_MAX) {
        return false;
    }

    TPerc* perc = mPercArray[key];
    if (perc == nullptr) {
        return false;
    }

    pParams->mChannelType = JASChannel::CH_WAVE;
    pParams->_24 = true;
    pParams->mVolume = perc->mVolume;
    pParams->mPitch = perc->mPitch;
    pParams->mPan = perc->mPan;
    pParams->mDirectRelease = u16(perc->mRelease);

    static JASOscillator::Data osc;

    osc.mTarget = 0;
    osc._04 = 1.0f;
    osc.mTable = nullptr;
    osc.rel_table = nullptr;
    osc.mScale = 1.0f;
    osc._14 = 0.0f;

    static JASOscillator::Data* oscp = &osc;

    pParams->mOscillatorData = &oscp;
    pParams->mNumOscillators = 1;

    for (JASPtrListAdaptor< JASInstEffect >* it = perc->mEffects.begin(); it != nullptr; it = it->getNext()) {
        it->get()->effect(key, velocity, pParams);
    }

    for (int i = 0; i < perc->getVeloRegionCount(); i++) {
        const TVelomap* veloMap = &perc->mVelomap[i];
        if (velocity <= veloMap->_0) {
            pParams->mVolume *= veloMap->mVolume;
            pParams->mPitch *= veloMap->mPitch;
            pParams->_18 = veloMap->_4;
            return true;
        }
    }

    return false;
}

void JASDrumSet::setPerc(int index, JASDrumSet::TPerc* pPerc) {
    mPercArray[index] = pPerc;
}

JASDrumSet::TPerc::TPerc() : mVolume(1.0f), mPitch(1.0f), mPan(0.5f), mRelease(1000), mEffects(), mVelomapCount(0), mVelomap(nullptr) {
}

void JASDrumSet::TPerc::setVeloRegionCount(u32 count, JKRHeap* pHeap) {
    delete[] mVelomap;
    mVelomap = new (pHeap, 0) TVelomap[count];
    mVelomapCount = count;
}

JASDrumSet::TVelomap* JASDrumSet::TPerc::getVeloRegion(int index) {
    return &mVelomap[index];
}

void JASDrumSet::TPerc::appendEffect(JASPtrListAdaptor< JASInstEffect >* p) {
    if (mEffects.isEmpty()) {
        mEffects.mList = p;
        return;
    }

    JASPtrListAdaptor< JASInstEffect >* it = mEffects.begin();
    for (; it->getNext() != nullptr; it = it->getNext()) {
    }
    it->mNext = p;
}

void JASDrumSet::TPerc::setRelease(u32 release) {
    mRelease = release;
}
