#pragma once

#include "Game/LiveActor/MaterialCtrl.hpp"
#include <revolution.h>
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>

class J3DAnmBase;
class ResourceHolder;

class DisplayListMaker {
public:
    void onCurFlagBpk(const J3DAnmBase *);
    void offCurFlagBpk(const J3DAnmBase *);
    void onCurFlagBtp(const J3DAnmBase *);
    void offCurFlagBtp(const J3DAnmBase *);
    void onCurFlagBtk(const J3DAnmBase *);
    void offCurFlagBtk(const J3DAnmBase *);
    void onCurFlagBrk(const J3DAnmBase *);
    void offCurFlagBrk(const J3DAnmBase *);

    void newDifferedDisplayList();

    void update();

    bool isValidDiff();

    MaterialCtrl** mMaterialCtrls;      // 0x0
    u32 mNumMaterialCtrls;              // 0x4
    u32 _8;
    u32 mPrgFlag;                       // 0xC
    u32 mCurFlag;                       // 0x10
    J3DModel* mModel;                   // 0x14
    FogCtrl* mFogCtrl;                  // 0x18
    ResourceHolder* mResHolder;         // 0x1C
};