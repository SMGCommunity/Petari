#pragma once

#include "Game/Demo/DemoCastGroup.hpp"
#include "Game/Demo/DemoSubPartKeeper.hpp"

class DemoActionKeeper;
class DemoTimeKeeper;
class DemoTalkAnimCtrl;
class DemoSubPartKeeper;

class DemoExecutor : public DemoCastGroup {
public:
    DemoExecutor(const char *);

    bool tryStartProperDemoSystem();
    bool tryStartDemoSystemPart(const char *, s32);
    bool tryStartProperDemoSystemPart(const char *);

    void pause();
    void resume();

    void addTalkAnimCtrl(DemoTalkAnimCtrl *);
    void addTalkMessageCtrl(LiveActor *, TalkMessageCtrl *);
    void findTalkMessageCtrl(const LiveActor *) const;
    void setTalkMessageCtrl(const LiveActor *, TalkMessageCtrl *);

    inline s32 getSubPartStep(const char *name) {
        DemoSubPartInfo *part;
        DemoSubPartKeeper *subpartkeeper = mSubPartKeeper;
        for (int i = 0; i < subpartkeeper->mNumSubPartInfos; i++) {
            part = &subpartkeeper->mSubPartInfos[i];
            if (MR::isEqualString(name, part->mMainPartName) && MR::isEqualSubString(part->mSubPartName, "会話アニメループ")) {
                return part->mMainPartStep;
            }
        }
        return 0;
    }

    const char *mSheetName;            // 0x14
    DemoTimeKeeper *mTimeKeeper;       // 0x18
    DemoSubPartKeeper *mSubPartKeeper; // 0x1C
    void *_20;
    void *_24;
    DemoActionKeeper *mActionKeeper;   // 0x28
};
