#include "Game/System/ConfigDataMii.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <RFL_Types.h>

#define FLAG_UNK1 0x1
#define FLAG_UNK2 0x2

#define ICON_ID_MII 0
#define ICON_ID_MARIO 1
#define ICON_ID_LUIGI 2
#define ICON_ID_YOSHI 3
#define ICON_ID_KINOPIO 4
#define ICON_ID_PEACH 5

ConfigDataMii::ConfigDataMii() : mFlag(0), mIconId(ICON_ID_MARIO), mMiiId(nullptr) {
    mMiiId = new u8[sizeof(RFLCreateID)];

    initializeData();
}

u32 ConfigDataMii::makeHeaderHashCode() const {
    return 0x2836E9;
}

u32 ConfigDataMii::getSignature() const {
    return 'MII ';
}

s32 ConfigDataMii::serialize(u8* pBuffer, u32 size) const {
    JSUMemoryOutputStream stream = JSUMemoryOutputStream(pBuffer, size);

    u8 flag = mFlag;
    stream.write(&flag, sizeof(mFlag));

    stream.write(mMiiId, sizeof(RFLCreateID));

    u8 iconId = mIconId;
    stream.write(&iconId, sizeof(mIconId));

    return stream.mPosition;
}

s32 ConfigDataMii::deserialize(const u8* pBuffer, u32 size) {
    initializeData();

    JSUMemoryInputStream stream = JSUMemoryInputStream(pBuffer, size);

    u8 flag;
    stream.read(&flag, sizeof(mFlag));
    mFlag = flag;

    stream.read(mMiiId, sizeof(RFLCreateID));

    if (stream.getAvailable() == 0) {
        if ((mFlag & FLAG_UNK1) != 0) {
            mIconId = ICON_ID_MII;
        }
    } else {
        stream.read(&mIconId, sizeof(mIconId));
    }

    return 0;
}

void ConfigDataMii::initializeData() {
    mFlag = 0;
    mIconId = ICON_ID_MARIO;

    MR::zeroMemory(mMiiId, sizeof(RFLCreateID));
}

void ConfigDataMii::setMiiOrIconId(const void* pMiiId, const u32* pIconId) {
    if (pMiiId != nullptr) {
        MR::copyMemory(mMiiId, pMiiId, sizeof(RFLCreateID));

        mFlag |= FLAG_UNK2;
        mIconId = ICON_ID_MII;
    } else {
        MR::fillMemory(mMiiId, 0, sizeof(RFLCreateID));

        mIconId = *pIconId;
    }
}

bool ConfigDataMii::getIconId(u32* pIconId) const {
    *pIconId = mIconId;

    return mIconId != ICON_ID_MII;
}

bool ConfigDataMii::getMiiId(void* pMiiId) const {
    MR::copyMemory(pMiiId, mMiiId, sizeof(RFLCreateID));

    return mIconId == ICON_ID_MII;
}
