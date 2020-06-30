#pragma once

#include "System/Resource/ResourceInfo.h"
#include "J3DGraphBase/J3DAnimation.h"
#include "J3DGraphBase/J3DModelData.h"

class AnmPlayerBase
{
public:
    AnmPlayerBase(const ResTable *);

    virtual void changeAnimation(J3DAnmBase *);
    virtual void stopAnimation();

    void update();
    void reflectFrame();
    void start(const char *);
    void stop();
    bool isPlaying(const char *) const;

    J3DAnmBase* mJ3DAnmBase; // _4
    const ResTable* mResourceTable; // _8
    J3DFrameCtrl mFrameCtrl; // _C
};

class MaterialAnmPlayerBase : public AnmPlayerBase
{
public:
    MaterialAnmPlayerBase(const ResTable *, J3DModelData *);

    void beginDiff();
    void endDiff();

    J3DModelData* mModelData; // _20
};