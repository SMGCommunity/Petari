#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudEffector.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudSoundObjHolder.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <JSystem/JAudio2/JAISound.hpp>
#include <JSystem/JGadget/hashcode.hpp>

void FORCE_MATCH_SDATA2() {
    f32 f1 = 1.0f;
}

AudSoundObject::AudSoundObject(TVec3f* pPos, u8 numHandles, JKRHeap* pHeap)
    : JAUSoundObject(pPos, numHandles, pHeap), JKRDisposer(), _30(0), mMapCode(0), mMapCodeExtra(0), mHashDatas(nullptr), mNumSounds(0) {
    mNumHandles = numHandles;
    if (mNumHandles > 0) {
        mHashDatas = new AudSoundObjHashData[numHandles];

        for (s32 idx = 0; idx < mNumHandles; idx++) {
            mHashDatas[idx].init();
        }
    }
}

AudSoundObject::~AudSoundObject() {
    AudWrap::getSoundObjHolder()->remove(this);
}

void AudSoundObject::addToSoundObjHolder() {
    AudWrap::getSoundObjHolder()->add(this);
}

void AudSoundObject::setTrans(TVec3f* pTrans) {
    mPos = pTrans;
}

bool AudSoundObject::writePort(JAISoundHandle* pHandle, u32 port, u16 value) {
    if (pHandle == nullptr) {
        return false;
    }

    if (!pHandle->isSoundAttached()) {
        return false;
    }

    BOOL en = OSDisableInterrupts();

    JASTrack* track = pHandle->getSound()->getTrack();
    if (track != nullptr) {
        track->writePort(port, value);
        OSRestoreInterrupts(en);
        return true;
    }
    OSRestoreInterrupts(en);
    return false;
}

bool AudSoundObject::isLimitedSound(JAISoundID soundID) {
    if (soundID == -1) {
        return true;
    }

    if (AudWrap::getSystem()->isRegisteredLimitedSound(soundID)) {
        return true;
    }

    if (modifyLimitedSound(soundID)) {
        return true;
    }

    if (!isPlayingID(soundID)) {
        AudSoundInfo* info = AudWrap::getSoundInfo();
        if (info != nullptr) {
            if (info->isLimitCountPlaying(soundID)) {
                return true;
            }

            if (info->isLimitNumPlaying(soundID)) {
                return true;
            }
        }
    }

    return false;
}

JAISoundHandle* AudSoundObject::startSound(JAISoundID soundID) {
    if (!isEnableStartSound(soundID)) {
        return nullptr;
    }

    if (soundID == -1) {
        return nullptr;
    }

    if (AudWrap::getSystem()->_82B && soundID.getGroupID() != SE_HOME_BUTTON_MENU && soundID.getGroupID() != SE_SYSTEM) {
        return nullptr;
    }

    if (isLimitedSound(soundID)) {
        return nullptr;
    }

    limitVoiceOne(soundID);
    JAISoundHandle* handle = JAUSoundObject::startSound(soundID);
    setMapCodeToPort(handle, soundID);
    setCutoffToPort(handle, soundID);
    return handle;
}

JAISoundHandle* AudSoundObject::startLevelSound(JAISoundID soundID) {
    if (!isEnableStartSound(soundID)) {
        return nullptr;
    }

    if (soundID == -1) {
        return nullptr;
    }

    if (AudWrap::getSystem()->_82C && soundID.getGroupID() != SE_HOME_BUTTON_MENU && soundID.getGroupID() != SE_SYSTEM) {
        return nullptr;
    }

    if (isLimitedSound(soundID)) {
        return nullptr;
    }

    JAISoundHandle* handle = JAUSoundObject::startLevelSound(soundID);
    setMapCodeToPort(handle, soundID);
    setCutoffToPort(handle, soundID);
    return handle;
}

JAISoundHandle* AudSoundObject::startLevelSound(const char* pName) {
    JAISoundID id = convertNameToLevelSEID(pName);
    return startLevelSound(id);
}

JAISoundHandle* AudSoundObject::startSoundParam(JAISoundID soundID, s32 volume, s32 a2) {
    JAISoundHandle* handle = startSound(soundID);
    if (handle != nullptr && handle->isSoundAttached()) {
        // TODO: param names!
        modifySe_Kawamura(handle, volume);
        modifySe_Takezawa(handle, volume, a2);
        modifySe_Gohara(handle, volume, a2);
    }

    return handle;
}

JAISoundHandle* AudSoundObject::startLevelSoundParam(JAISoundID soundID, s32 volume, s32 a2) {
    JAISoundHandle* handle = startLevelSound(soundID);
    if (handle != nullptr && handle->isSoundAttached()) {
        // TODO: param names!
        modifySe_Kawamura(handle, volume);
        modifySe_Takezawa(handle, volume, a2);
        modifySe_Gohara(handle, volume, a2);
    }

    return handle;
}

JAISoundHandle* AudSoundObject::startLevelSoundParam(const char* pName, s32 volume, s32 a2) {
    JAISoundID id = convertNameToLevelSEID(pName);
    return startLevelSoundParam(id, volume, a2);
}

bool AudSoundObject::modifyLimitedSound(JAISoundID soundID) {
    if (modifyLimitedSound_Kawamura(soundID)) {
        return true;
    }

    if (modifyLimitedSound_Takezawa(soundID)) {
        return true;
    }

    return false;
}

bool AudSoundObject::isPlayingID(JAISoundID soundID) {
    return getHandleSoundID(soundID) != nullptr;
}

void AudSoundObject::releaseHandle(JAISoundID soundID) {
    JAISoundHandle* handle = getHandleSoundID(soundID);
    if (handle != nullptr) {
        handle->getSound()->releaseHandle();
    }
}

void AudSoundObject::setMapCode(s32 code) {
    if (code < 0) {
        mMapCode = 0;
    } else {
        mMapCode = code;
    }
}

void AudSoundObject::setMapCodeExtra(s32 code) {
    if (code == 0) {
        code = 1;
    }

    if (code < 0) {
        mMapCodeExtra = 0;
    } else {
        mMapCodeExtra = code;
    }
}

s32 AudSoundObject::getMapCode() const {
    if (mMapCodeExtra > 0) {
        return mMapCodeExtra;
    } else {
        return mMapCode;
    }
}

void AudSoundObject::clearMapCode() {
    mMapCode = 0;
    mMapCodeExtra = 0;
}

JAISoundID AudSoundObject::convertNameToLevelSEID(const char* pName) {
    u32 hash = 0;
    bool found = false;

    for (s32 idx = 0; idx < mNumSounds; idx++) {
        if (mHashDatas[idx].mName == pName) {
            hash = mHashDatas[idx].mHash;
            found = true;
        }
    }

    if (!found) {
        hash = JGadget::getHashCode(pName);

        for (s32 idx = 0; idx < mNumSounds; idx++) {
            mHashDatas[idx].mID = (u32)mHashDatas[idx].mID + 1;
        }

        if (mNumSounds < mNumHandles) {
            AudSoundObjHashData* data = &mHashDatas[mNumSounds];
            data->mName = pName;
            data->mHash = hash;
            data->mID = 0;
            mNumSounds++;
        } else {
            s32 i = 0;
            for (s32 idx = 0; idx < mNumSounds; idx++) {
                if (mHashDatas[i].mID < mHashDatas[idx].mID) {
                    i = idx;
                }
            }
            AudSoundObjHashData* data = &mHashDatas[i];
            data->mName = pName;
            data->mHash = hash;
            data->mID = 0;
        }
    }

    return AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName, hash);
}

void AudSoundObject::limitVoiceOne(JAISoundID soundID) {
    u8 group = soundID.getGroupID();

    if (group == SE_ENEMY_VOICE) {
        stopCategorySound(SE_ENEMY_VOICE, 0);
        return;
    }

    if (group == SE_BOSS_VOICE) {
        stopCategorySound(SE_BOSS_VOICE, 0);
        return;
    }

    if (group == SE_PLAYER_VOICE) {
        stopCategorySound(SE_PLAYER_VOICE, 0);
        return;
    }

    if (group == SE_SUPPORTER_VOICE) {
        stopCategorySound(SE_SUPPORTER_VOICE, 0);
        return;
    }
}

void AudSoundObject::stopCategorySound(u32 groupID, u32 fadeTime) {
    for (s32 idx = 0; idx < JAUSoundObject::mNumHandles; idx++) {
        JAISound* sound = mHandles[idx].getSound();
        if (sound != nullptr) {
            JAISoundID id = sound->mSoundID;
            u32 group = id.getGroupID();
            if (group == groupID) {
                stopSound(id, fadeTime);
            }
        }
    }
}

void AudSoundObject::setMapCodeToPort(JAISoundHandle* pHandle, JAISoundID soundID) {
    if (pHandle == nullptr) {
        return;
    }

    if (!pHandle->isSoundAttached()) {
        return;
    }

    s32 mapCode = getMapCode();
    if (mapCode <= 0) {
        return;
    }

    // only specific soundIDs can modify based on mapcode
    switch (soundID) {
    case SE_PM_HIPDROP:
    case SE_PM_FALLDOWN_S:
    case SE_PM_FALLDOWN_M:
    case SE_PM_FOOTNOTE_L:
    case SE_PM_FOOTNOTE_R:
    case SE_PM_FOOTNOTE_SUB_L:
    case SE_PM_FOOTNOTE_SUB_R:
    case SE_PM_JUMP:
    case SE_PM_LAND:
    case SE_PM_HAND:
    case SE_PM_LAND_HEAVY:
        writePort(pHandle, 12, (mapCode - 1) / 7);
        writePort(pHandle, 11, ((mapCode - 1) % 7) * 12);
        break;
    }
}

void AudSoundObject::setCutoffToPort(JAISoundHandle* pHandle, JAISoundID soundID) {
    if (pHandle == nullptr) {
        return;
    }

    if (!pHandle->isSoundAttached()) {
        return;
    }

    AudSoundInfo* info = AudWrap::getSoundInfo();
    if (info == nullptr) {
        return;
    }

    if (info->isUseLpf(soundID)) {
        AudEffector* effector = AudWrap::getSystem()->mAudEffector;
        s32 value = effector->mCutoff;
        if (value < 0 || value > 0x7F) {
            value = 0x7F;
        }

        switch (soundID) {
        case SE_AT_LV_WATERFALL_S:
        case SE_AT_LV_WATERFALL_L:
        case SE_AT_LV_WATERFALL_UPPER_S:
        case SE_AT_LV_WATERFALL_UPPER_L:
        case SE_OJ_SEAGULL_CHIRP:
            if (effector->mSpecialFx == 2) {
                // TODO: magic value
                value = 10;
                pHandle->getSound()->mParams.mMove.moveVolume(0.5f, 0);
            }
            break;
        case SE_OJ_LV_FOUNTAIN:
        case SE_AT_LV_WIND_RABBIT_MAZE:
            if (effector->mSpecialFx == 1) {
                // TODO: magic value
                value = 20;
                pHandle->getSound()->mParams.mMove.moveVolume(0.5f, 5);
            } else {
                value = 0x7F;
                pHandle->getSound()->mParams.mMove.moveVolume(1.0f, 5);
            }
            break;
        }
        // TODO: port magic value
        writePort(pHandle, 10, value);
    } else {
        writePort(pHandle, 10, 0x7F);
        return;
    }
}

JAISoundID AudSoundObject::convertSoundIdFromSeVersion(JAISoundID soundID) const {
    if (_30 == 1) {
        switch (soundID) {
        case SE_EM_KURIBO_LAND:
            return SE_EM_KURIBO_LAND_2D;
        case SE_EM_KURIBO_STEP1:
            return SE_EM_KURIBO_STEP1_2D;
        case SE_EM_KURIBO_STEP2:
            return SE_EM_KURIBO_STEP2_2D;
        case SE_EM_KURIBO_FIND:
            return SE_EM_KURIBO_FIND_2D;
        case SE_EM_LV_KURIBO_DASH1:
            return SE_EM_LV_KURIBO_DASH1_2D;
        case SE_EM_KURIBO_JUMP:
            return SE_EM_KURIBO_JUMP_2D;
        case SE_EM_KURIBO_SWOON_RECOVER:
            return SE_EM_KURIBO_SWOON_RECOVER_2D;
        case SE_EM_METBO_TRAMPLE:
            return SE_EM_METBO_TRAMPLE_2D;
        case SE_EM_METBO_SPIN_HIT:
            return SE_EM_METBO_SPIN_HIT_2D;
        case SE_EM_METBO_FIND:
            return SE_EM_METBO_FIND_2D;
        case SE_EM_LV_METBO_STAGGER:
            return SE_EM_LV_METBO_STAGGER_2D;
        case SE_EM_METBO_SWOON_RECOVER:
            return SE_EM_METBO_SWOON_RECOVER_2D;
        case SE_EM_LV_METBO_SWOON:
            return SE_EM_LV_METBO_SWOON_2D;
        case SE_EM_EXPLODE_S:
            return SE_EM_EXPLODE_S_2D;
        case SE_EM_LV_SWOON_S:
            return SE_EM_LV_SWOON_S_2D;
        }
    }

    return soundID;
}

AudSoundObjHashData::AudSoundObjHashData() {
    init();
}

void AudSoundObjHashData::init() {
    mName = nullptr;
    mHash = 0;
    mID = 0;
}
