#pragma once

#include "Game/AreaObj/AreaObj.h"

class CubeCameraArea : public AreaObj {
public:
    enum ECategory {

    };
    
    CubeCameraArea(int, const char *);

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &) const;
    virtual s32 getCategoryArg() const;

    bool isValidCameraID() const;
    u16 getCameraID() const;

    static void setCurrentCategory(s32);

    static s32 sCubeCategory;

    u32 _3C;
    u32 mZoneID; // _40
};
