#pragma once

#include "JSystem/JAudio2/JASWaveArcLoader.hpp"
#include "JSystem/JAudio2/JASWaveInfo.hpp"

struct JASBasicWaveBank : public JASWaveBank {
    struct TWaveInfo;

    struct TWaveGroup : JASWaveArc {
        TWaveGroup(JASBasicWaveBank*);
        virtual ~TWaveGroup();
        void setWaveCount(u32, JKRHeap*);
        virtual void onLoadDone();
        virtual void onEraseDone();
        u32 getWaveID(int) const;
        void setWaveInfo(int param_0, u32 param_1, const JASWaveInfo& waveInfo);

        /* 0x74 */ JASBasicWaveBank* mBank;
        /* 0x78 */ TWaveInfo* mCtrlWaveArray;
        /* 0x7C */ u32 mWaveCount;

        u32 getWaveCount() const {
            return mWaveCount;
        }
    };

    struct TWaveHandle : public JASWaveHandle {
        TWaveHandle() {
            mHeap = nullptr;
        }
        virtual ~TWaveHandle(){};
        virtual const JASWaveInfo* getWaveInfo() const {
            return &mWaveInfo;
        }
        virtual int getWavePtr() const;

        bool compareHeap(JASHeap* heap) const {
            return mHeap == heap;
        }

        /* 0x04 */ JASWaveInfo mWaveInfo;
        /* 0x28 */ JASHeap* mHeap;
    };

    struct TWaveInfo {
        TWaveInfo() {
            mWaveId = 0;
            mNext = nullptr;
            mPrev = nullptr;
        }

        /* 0x00 */ TWaveHandle mWaveHandle;
        /* 0x30 */ u32 mWaveId;
        /* 0x34 */ TWaveInfo* mNext;
        /* 0x38 */ TWaveInfo* mPrev;
    };

    JASBasicWaveBank();
    ~JASBasicWaveBank();
    TWaveGroup* getWaveGroup(u32);
    void setGroupCount(u32, JKRHeap*);
    void setWaveTableSize(u32, JKRHeap*);
    void incWaveTable(JASBasicWaveBank::TWaveGroup const*);
    void decWaveTable(JASBasicWaveBank::TWaveGroup const*);
    JASWaveHandle* getWaveHandle(u32) const;

    void setWaveInfo(JASBasicWaveBank::TWaveGroup* waveGroup, int index, u16 waveId, JASWaveInfo const& waveInfo) {
        waveGroup->setWaveInfo(index, waveId, waveInfo);
    }
    JASWaveArc* getWaveArc(u32 param_0) {
        return getWaveGroup(param_0);
    }
    u32 getArcCount() const {
        return mGroupCount;
    }

    /* 0x04 */ OSMutex mMutex;
    /* 0x1C */ TWaveInfo** mWaveTable;
    /* 0x20 */ u32 mHandleCount;
    /* 0x24 */ TWaveGroup** mWaveGroupArray;
    /* 0x28 */ u32 mGroupCount;

    static u32 mNoLoad;
};
