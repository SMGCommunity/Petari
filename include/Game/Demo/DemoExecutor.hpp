#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"

class DemoActionKeeper;
class DemoTimeKeeper;
class DemoTalkAnimCtrl;
class DemoSubPartKeeper;

class DemoExecutor : public DemoCastGroup {
public:
    DemoExecutor(const char*);

    bool tryStartProperDemoSystem();
    bool tryStartDemoSystemPart(const char*, s32);
    bool tryStartProperDemoSystemPart(const char*);

    void pause();
    void resume();

    void addTalkAnimCtrl(DemoTalkAnimCtrl*);
    void addTalkMessageCtrl(LiveActor*, TalkMessageCtrl*);
    TalkMessageCtrl* findTalkMessageCtrl(const LiveActor*) const;
    void setTalkMessageCtrl(const LiveActor*, TalkMessageCtrl*);

    inline s32 getSubPartStep(const char* pName) {
        DemoSubPartInfo* subpart;
        DemoSubPartKeeper* subpartkeeper = mSubPartKeeper;
        for (int i = 0; i < subpartkeeper->mNumSubPartInfos; i++) {
            subpart = &subpartkeeper->mSubPartInfos[i];
            if (MR::isEqualString(pName, subpart->mMainPartName) && MR::isEqualSubString(subpart->mSubPartName, "会話アニメループ")) {
                return subpart->mMainPartStep;
            }
        }
        return 0;
    }

    const char* mSheetName;             // 0x14
    DemoTimeKeeper* mTimeKeeper;        // 0x18
    DemoSubPartKeeper* mSubPartKeeper;  // 0x1C
    void* _20;
    void* _24;
    DemoActionKeeper* mActionKeeper;    // 0x28
    u8 _2C[0x32C];                      // padding to 0x358
    DemoTalkAnimCtrl* mTalkAnimCtrls[8];  // 0x358 (inline array)
    s32 mNumTalkAnimCtrls;              // 0x378
};
