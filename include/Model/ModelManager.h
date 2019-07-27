#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "types.h"
#include "Animation/BpkPlayer.h"
#include "Animation/BrkPlayer.h"
#include "Animation/BtkPlayer.h"
#include "Animation/BtpPlayer.h"
#include "Animation/BvaPlayer.h"
#include "Model/DisplayList/DisplayListMaker.h"
#include "Model/J3D/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class ModelManager
{
public:
    ModelManager();

    void init(const char *, const char *, bool);
    void initVisibilityAnim();
    void calc();
    void update();
    void calcAnim();
    void calcView();
    void entry();
    void newDifferedDLBuffer();
    void updateDL();

    void initMaterialAnm();
    void calc();
    ResourceHolder* getResourceHolder() const;
    J3DModel* getJ3DModel() const;

    BtkPlayer* mBTKPlayer; // _0
    BrkPlayer* mBRKPlayer; // _4
    BtpPlayer* mBTPPlayer; // _8
    BpkPlayer* mBPKPlayer; // _C
    BvaPlayer* mVisiAnim; // _10
    u32* _14;
    u32* _18; // XanimePlayer*
    J3DModel* mModel; // _1C
    ResourceHolder* mResourceHolder; // _20
    DisplayListMaker* mDLMaker; // _24
};

#endif // MODELMANAGER_H