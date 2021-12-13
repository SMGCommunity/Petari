#pragma once

#include "Game/System/BinaryDataChunkHolder.h"

class ConfigDataMisc : public BinaryDataChunkBase {
public:
    ConfigDataMisc();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual void* serialize();
    virtual bool deserialize(const u8 *, u32);
    virtual void initializeData();

    bool isLastLoadedMario() const;
    void setLastLoadedMario(bool);
    void onCompleteEndingMario();
    void onCompleteEndingLuigi();
    bool isOnCompleteEndingMario();
    bool isOnCompleteEndingLuigi();
    OSTime getLastModified() const;
    void updateLastModified();

    u8 mData;               // _4
    OSTime mLastModified;   // _8
};
