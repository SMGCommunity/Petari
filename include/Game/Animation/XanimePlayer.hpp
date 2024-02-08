#pragma once

#include <revolution.h>
#include "Game/Animation/XanimeCore.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"

class XanimeResourceTable;

class XanimePlayer {
public:
    XanimePlayer(J3DModel *, XanimeResourceTable *, XanimePlayer *);
    XanimePlayer(J3DModel *, XanimeResourceTable *);

    void stopAnimation();
    void stopAnimation(const char *);

    void setModel(J3DModel *);

    void updateBeforeMovement();
    void updateAfterMovement();

    void changeAnimationBck(const char *);

    void changeInterpoleFrame(s32);
    
    void changeTrackAnimation(unsigned char, const char *);

    const char* getCurrentAnimationName() const;

    void setDefaultAnimation(const char *);

    void changeAnimation(const char *);

    inline XanimeCore* getCore() {
        return mCore;
    }

    J3DModel *mModel;            // _0
    J3DModelData* mModelData;   // _4
    u8 _8[0x6C-0x8];
    XanimeCore* mCore;          // _6C
    u8 _70[0x1C];
};