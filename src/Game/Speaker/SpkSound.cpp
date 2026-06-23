#include "Game/Speaker/SpkSound.hpp"
#include "Game/Speaker/SpkSpeakerCtrl.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>

void SpkSoundHandle::releaseSound() {
    if (!mSound) {
        return;
    }

    mSound->mSoundHandle = nullptr;
    mSound = nullptr;
}

SpkSoundVolume::SpkSoundVolume() {
    mReleaseVolume = 1.0f;
    mReleaseRate = 0.0f;
    mFadeOutVolume = 1.0f;
    mFadeOutRate = 0.0f;
    mFadeVolume = 1.0f;
    mFadeTarget = 1.0f;
    mFadeRate = 0.0f;
    mBaseVolume = 1.0f;
}

void SpkSoundVolume::setRelease(s32 release) {
    if (release > 0) {
        mReleaseRate = 1.0f / release;
    } else {
        mReleaseRate = 1.0f;
    }

    mReleaseVolume = 1.0f;
}

void SpkSoundVolume::setFadeOut(s32 fadeOut) {
    JASCriticalSection crit;

    if (fadeOut > 0) {
        mFadeOutRate = 1.0f / fadeOut;
    } else {
        mFadeOutRate = 1.0f;
    }

    mFadeOutVolume = 1.0f;
}

f32 SpkSoundVolume::calc(bool& kill) {
    kill = false;
    if (mReleaseRate != 0.0f) {
        mReleaseVolume -= mReleaseRate;
        if (mReleaseVolume <= 0.0f) {
            mReleaseVolume = 0.0f;
            mReleaseRate = 0.0f;
            kill = true;
        }
    }

    if (mFadeOutRate != 0.0f) {
        mFadeOutVolume -= mFadeOutRate;
        if (mFadeOutVolume <= 0.0f) {
            mFadeOutVolume = 0.0f;
            mFadeOutRate = 0.0f;
            kill = true;
        }
    }

    if (mFadeRate != 0.0f) {
        mFadeVolume += mFadeRate;
        if (mFadeRate >= 0.0f) {
            if (mFadeVolume >= mFadeTarget) {
                mFadeVolume = mFadeTarget;
                mFadeRate = 0.0f;
            }
        } else {
            if (mFadeVolume <= mFadeTarget) {
                mFadeVolume = mFadeTarget;
                mFadeRate = 0.0f;
            }
        }
    }

    return mReleaseVolume * mReleaseVolume * mFadeOutVolume * mFadeVolume * mBaseVolume;
}

SpkSound::SpkSound()
    : JSULink(this), mSoundHandle(nullptr), mWaveID(-1), mWave(nullptr), mWavePtr(0), mPadChannel(WPAD_CHAN0), _24(100), mReleaseTime(0),
      mWaveSize(0), mLoopStartPos(-1), mLoopEndPos(-1), mState(0), _3C(-1) {
}

SpkSound::~SpkSound() {
    releaseHandle();
}

void SpkSound::update(f32 baseVolume) {
    if (mState != State_UNLOCK && mState != State_STOP) {
        return;
    }

    SpkMixingBuffer* mix = JASGlobalInstance< SpkSystem >::getInstance()->mMixBuffer;

    bool kill = false;
    f32 volume = mVolume.calc(kill);
    if (kill) {
        mState = State_DEAD;
    }
    volume *= baseVolume;

    if (!isLoopValid()) {
        s32 readSize = 40;
        if (mWavePtr + readSize >= mWaveSize) {
            readSize = mWaveSize - mWavePtr;
        }
        mix->mix(mPadChannel, mWave + mWavePtr, readSize, volume, 0);
        mWavePtr += 40;
        if (mWavePtr >= mWaveSize) {
            mState = State_DEAD;
        }
        return;
    }

    s32 readAmount;
    s32 readOffset = 0;
    s32 readSize = 40;

    if (mLoopEndPos - mWavePtr >= readSize) {
        mix->mix(mPadChannel, mWave + mWavePtr, readSize, volume, readOffset);
        mWavePtr += 40;
        if (mWavePtr >= mLoopEndPos) {
            mWavePtr = mLoopStartPos;
        }
        return;
    }

    while (readSize > 0) {
        readAmount = mLoopEndPos - mWavePtr;
        if (readAmount >= readSize) {
            readAmount = readSize;
        }
        readSize -= readAmount;
        mix->mix(mPadChannel, mWave + mWavePtr, readAmount, volume, readOffset);
        readOffset += readAmount;
        mWavePtr += readAmount;
        if (mWavePtr >= mLoopEndPos) {
            mWavePtr = mLoopStartPos;
        }
    }
}

void SpkSound::stop(s32 stopTime) {
    mState = State_STOP;
    s32 fade = (6 * stopTime) / 40;

    if (fade != 0) {
        mVolume.setFadeOut(fade);
    } else {
        mVolume.setRelease(mReleaseTime);
    }
}

void SpkSound::attachHandle(SpkSoundHandle* pHandle) {
    if (mSoundHandle) {
        releaseHandle();
    }

    mSoundHandle = pHandle;
    pHandle->mSound = this;
}

void SpkSound::releaseHandle() {
    if (!mSoundHandle) {
        return;
    }

    mSoundHandle->mSound = nullptr;
    mSoundHandle = nullptr;
}

bool SpkSound::start(s32 padChannel, s32 waveID) {
    mPadChannel = padChannel;
    bool res = setWaveData(waveID);

    if (!res) {
        return false;
    }

    mState = State_LOCK;
    return true;
}

void SpkSound::unlock() {
    if (mState != State_LOCK) {
        return;
    }

    mState = State_UNLOCK;
}

bool SpkSound::setWaveData(s32 waveID) {
    SpkData* data = JASGlobalInstance< SpkSystem >::getInstance()->mData;

    if (!data->isValid()) {
        return false;
    }

    mWaveID = waveID;

    SpkTable* table = data->getSpkTable();

    u16 ID = table->getParameters(waveID)->mWaveID;
    _24 = table->getParameters(waveID)->_2;
    mVolume.mBaseVolume = (table->getParameters(waveID)->mVolume) / 127.0f;
    mReleaseTime = (table->getParameters(waveID)->mReleaseTime * 6) / 40;

    SpkWave* wave = data->getWave();
    mWave = wave->getWave(ID);
    mWavePtr = 0;
    mWaveSize = wave->getWaveSize(ID) / 2;
    mLoopStartPos = wave->getLoopStartPos(ID);
    mLoopEndPos = wave->getLoopEndPos(ID);

    return true;
}

SpkSoundHolder::SpkSoundHolder() : JASGlobalInstance(true) {
    _44 = 10;
    mBaseVolume = 1.0f;

    for (int i = 0; i < 4; i++) {
        mVolume[i] = 1.0f;
    }
}

bool SpkSoundHolder::startSound(s32 padChannel, s32 waveID, SpkSoundHandle* pHandle) {
    if (!SpkSpeakerCtrl::isEnable(padChannel)) {
        return false;
    }

    if (waveID < 0) {
        return false;
    }

    if (pHandle != nullptr) {
        SpkSound* sound = pHandle->getSound();
        if (sound != nullptr) {
            if (sound->mState == SpkSound::State_LOCK) {
                sound->mState = SpkSound::State_DEAD;
            } else {
                sound->stop(0);
            }
        }
    }

    SpkSound* sound = new SpkSound();
    if (sound == nullptr) {
        return false;
    }

    if (pHandle != nullptr) {
        sound->attachHandle(pHandle);
    }

    if (!sound->start(padChannel, waveID)) {
        delete sound;
        return false;
    }

    JASCriticalSection crit;
    mSoundList[padChannel].append(sound);
    return true;
}

bool SpkSoundHolder::update(s32 padChannel) {
    JASCriticalSection crit;
    bool ret = updateEachSound(padChannel);
    return ret;
}

bool SpkSoundHolder::updateEachSound(s32 padChannel) {
    bool updated = false;

    JSULink< SpkSound >* sound = mSoundList[padChannel].getFirst();
    f32 volume = getBaseVolume() * get_44();

    if (sound != nullptr) {
        updated = true;
        for (; sound != mSoundList[padChannel].getEnd(); sound = sound->getNext()) {
            sound->getObject()->update(volume * mVolume[padChannel]);
        }
    }
    return updated;
}

void SpkSoundHolder::freeDeadSound(s32 padChannel) {
    for (JSULink< SpkSound >* sound = mSoundList[padChannel].getFirst(); sound != mSoundList[padChannel].getEnd();) {
        JSULink< SpkSound >* next = sound->getNext();
        if (sound->getObject()->mState == SpkSound::State_DEAD) {
            mSoundList[padChannel].remove(sound);
            delete (SpkSound*)sound->mData;
        }
        sound = next;
    }
}

void SpkSoundHolder::framework() {
    JASCriticalSection crit;

    for (int i = 0; i < 4; i++) {
        for (JSULink< SpkSound >* sound = mSoundList[i].getFirst(); sound != mSoundList[i].getEnd(); sound = sound->getNext()) {
            if (sound->getObject() == nullptr) {
                continue;
            }

            if (sound->getObject()->mState == SpkSound::State_LOCK) {
                sound->getObject()->unlock();
                continue;
            }

            if (sound->getObject()->_3C < 0) {
                continue;
            }

            sound->getObject()->_3C--;

            if (sound->getObject()->_3C <= 0) {
                sound->getObject()->stop(0);
                sound->getObject()->_3C = -1;
            }
        }
        freeDeadSound(i);
    }
}
