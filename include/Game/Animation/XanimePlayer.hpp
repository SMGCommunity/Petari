#pragma once

#include <revolution.h>
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimeResource.hpp"
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

    const char* getCurrentBckName() const;

    void setDefaultAnimation(const char *);

    void changeAnimation(const char *);
    
    void changeAnimation(const XanimeGroupInfo *);

    inline XanimeCore* getCore() {
        return mCore;
    }

    J3DModel *mModel;            // _0
    J3DModelData *mModelData;   // _4
    u8 _8[0x5C-8];
    const XanimeGroupInfo *mDefaultAnimation; // _5C
    const XanimeGroupInfo *mStopAnimation; // _60
    u8 _64[8];
    XanimeCore *mCore;          // _6C
    XanimeResourceTable *mResourceTable; // _70
    u8 _74[0x8];
    bool _7C;
    bool _7D;
    u8 _7E[0x8C - 0x7E];
};