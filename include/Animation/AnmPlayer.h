#ifndef ANMPLAYER_H
#define ANMPLAYER_H

#include "System/ResourceInfo.h"
#include "Model/J3D/J3DAnimation.h"

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

    const static f32 zero;
    const static f32 one;
};

const f32 AnmPlayerBase::zero = 0.0f;
const f32 AnmPlayerBase::one = 1.0f;

#endif // ANMPLAYER_H