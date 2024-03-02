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

    MaterialCtrl** mMaterialCtrls;      // _0
    u32 mNumMaterialCtrls;              // _4
    u32 _8;
    u32 mPrgFlag;                       // _C
    u32 mCurFlag;                       // _10
    J3DModel* mModel;                   // _14
    FogCtrl* mFogCtrl;                  // _18
    ResourceHolder* mResHolder;         // _1C
};