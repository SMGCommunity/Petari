#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphAnimator/J3DModel.h"

class XanimeResourceTable;

class XanimePlayer {
public:
    XanimePlayer(J3DModel *, XanimeResourceTable *, XanimePlayer *);

    void stopAnimation();
    void stopAnimation(const char *);

    void setModel(J3DModel *);

    void updateBeforeMovement();
    void updateAfterMovement();

    void changeAnimationBck(const char *);

    void changeInterpoleFrame(s32);

    J3DModel mModel;            // _0
    J3DModelData* mModelData;   // _4
};