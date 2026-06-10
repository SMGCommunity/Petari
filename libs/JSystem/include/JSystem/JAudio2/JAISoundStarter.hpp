#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASGlobal.hpp"
#include "JSystem/JGeometry/TVec.hpp"


struct JAISoundStarter : public JASGlobalInstance< JAISoundStarter > {
    JAISoundStarter(bool setInstance);

    virtual ~JAISoundStarter();
    virtual bool startSound(JAISoundID id, JAISoundHandle* handlePtr, const TVec3f* posPtr) = 0;
    bool startLevelSound(JAISoundID id, JAISoundHandle* handlePtr, const TVec3f* posPtr);
};
