#pragma once

#include <revolution.h>
#include "Game/Animation/XanimeCore.h"
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
    
    void changeTrackAnimation(unsigned char, const char *);

    const char* getCurrentAnimationName() const;

    J3DModel *mModel;            // _0
    J3DModelData* mModelData;   // _4
    u8 _8[0x6C-0x8];
    XanimeCore* mCore;          // _6C
};