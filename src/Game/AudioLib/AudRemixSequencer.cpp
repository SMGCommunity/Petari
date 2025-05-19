#include "Game/AudioLib/AudRemixSequencer.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/GameAudio/AudTalkSoundData.hpp"
#include "Game/SingletonHolder.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"

namespace
{
    static const char *cRemixNoteTrackSeId[] = {
        "SE_RS_REMIX_NOTE_GET_TRK0",
        "SE_RS_REMIX_NOTE_GET_TRK1",
        "SE_RS_REMIX_NOTE_GET_TRK2",
        "SE_RS_REMIX_NOTE_GET_TRK3",
        "SE_RS_REMIX_NOTE_GET_TRK4",
        "SE_RS_REMIX_NOTE_GET_TRK5",
        "SE_RS_REMIX_NOTE_GET_TRK6",
        "SE_RS_REMIX_NOTE_GET_TRK7",
        "SE_RS_REMIX_NOTE_GET_TRK8",
        "SE_RS_REMIX_NOTE_GET_TRK9",
        "SE_RS_REMIX_NOTE_GET_TRK10",
        "SE_RS_REMIX_NOTE_GET_TRK11",
        "SE_RS_REMIX_NOTE_GET_TRK12",
        "SE_RS_REMIX_NOTE_GET_TRK13",
        "SE_RS_REMIX_NOTE_GET_TRK14",
        "SE_RS_REMIX_NOTE_GET_TRK15",
    };
}

AudRmxSeqNoteOnTimer::AudRmxSeqNoteOnTimer()
{
    _8 = nullptr;
    _0 = 0.0f;
    _4 = 0.0f;
    _C = nullptr;
    initData();
}

void AudRmxSeqNoteOnTimer::initData()
{
    _8 = nullptr;
    _4 = 0.0f;
    _0 = 0.0f;
    _C = nullptr;
}

void AudRmxSeqNoteOnTimer::setData(const RemixNoteTrackData *trackData, const RemixNoteData *data) {
    _C = trackData;
    _8 = data;
    _0 = data->_C;
    _4 = _0 + data->_8;
}

bool AudRmxSeqNoteOnTimer::update(f32 f1)
{
    if (_4 <= 0.0f)
    {
        initData();
        return false;
    }

    _4 -= f1;

    if (_0 <= 0.0f)
    {
        if (_C != nullptr && _8 != nullptr)
        {
            AudRemixSequencer *pSeq = AudWrap::getRemixSequencer();
            JAISoundID id = getFreeSeID();
            JAISoundHandle *pHandle = AudWrap::getRemixSeqObject()->startSound(id);
            AudWrap::getSystemSeObject()->writePort(pHandle, 0xc, _C->_0);
            AudWrap::getSystemSeObject()->writePort(pHandle, 0xb, _8->_0);
            AudWrap::getSystemSeObject()->writePort(pHandle, 0xf, _8->_4);

            f32 x200 = pSeq->_200;
            u16 length = (_8->_8 * 120.0f) / x200;
            if (length < 1)
            {
                length = 1;
            }
            else if (length > 1440) {
                length = 1440;
            }
            AudWrap::getSystemSeObject()->writePort(pHandle, 0xe, length);
            _C = nullptr;
            _8 = nullptr;
        }
        return true;
    }
    else {
        _0 -= f1;
        return false;
    }
}

JAISoundID AudRmxSeqNoteOnTimer::getFreeSeID()
{
    
    for (u32 i = 0; i < 0x10; i++)
    {
        JAISoundID id = AudSingletonHolder<AudSoundNameConverter>::sInstance->getSoundID(cRemixNoteTrackSeId[i]);
        u32 *handleID = AudWrap::getRemixSeqObject()->getHandleSoundID(id);
        if (handleID == nullptr)
        {
            return id;
        }
    }
    JAISoundID id;
    id.mID = -1;
    return id;
}

AudRemixSequencer::AudRemixSequencer()
{
    _200 = 120.0f;
    _204 = 0.0f;
    initNoteOnBuff();
    setTempo(120.0f);
}

void AudRemixSequencer::initNoteOnBuff()
{
    for (int i = 0; i < 0x20; i++)
    {
        _0[i].initData();
    }
}

void AudRemixSequencer::update()
{
    for (int i = 0; i < 0x20; i++)
    {
        bool isUpdate = (_0[i]._4 > 0.0f);
        if (isUpdate)
        {
            _0[i].update(_204);
        }
    }
}

void AudRemixSequencer::setTempo(f32 f1)
{
    _200 = f1;
    f32 f2 = f1 + 0.0;
    f32 f3 = 48.0f;
    _204 = f3 * f2 / 3600.0f;
}

AudRmxSeqNoteOnTimer *AudRemixSequencer::newNoteOnTimer()
{
    for (int i = 0; i < 0x20; i++)
    {
        bool isUsed = (_0[i]._4 > 0.0f);
        if (!isUsed)
        {
            return &_0[i];
        }
    }
    return nullptr;
}

void AudRemixSequencer::addNoteData(const RemixNoteTrackData *trackData, const RemixNoteData *data)
{
    if (data->_0 >= 0)
    {
        AudRmxSeqNoteOnTimer *pTimer = newNoteOnTimer();
        if (pTimer)
        {
            pTimer->setData(trackData, data);
        }
    }
}
