#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"
#include "Game/System/ConfigDataMii.hpp"
#include "Game/System/ConfigDataMisc.hpp"

class ConfigDataCreateChunk : public BinaryDataChunkBase {
public:
    ConfigDataCreateChunk();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8 *, u32) const;
    virtual s32 deserialize(const u8 *, u32);
    virtual void initializeData();

    bool mData;    // 0x4
};

class ConfigDataHolder {
public:
    ConfigDataHolder();

    void setIsCreated(bool);
    bool isCreated() const;
    void setLastLoadedMario(bool);
    bool isLastLoadedMario() const;
    void onCompleteEndingMario();
    void onCompleteEndingLuigi();
    bool isOnCompleteEndingMario();
    bool isOnCompleteEndingLuigi();
    void updateLastModified();
    OSTime getLastModified() const;
    void setMiiOrIconId(const void *, const u32 *);
    bool getMiiId(void *) const;
    bool getIconId(u32 *) const;
    void resetAllData();
    void makeFileBinary(u8 *, u32);
    void loadFromFileBinary(const char *, const u8 *, u32);

    BinaryDataChunkHolder* mChunkHolder;    // 0x0
    ConfigDataCreateChunk* mCreateChunk;    // 0x4
    ConfigDataMii* mMiiCreateChunk;         // 0x8
    ConfigDataMisc* mMiscCreateChunk;       // 0xC
    char mName[0x10];                       // 0x10
};