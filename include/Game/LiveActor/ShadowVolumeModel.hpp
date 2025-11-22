#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class J3DModelData;

class ShadowVolumeModel : public ShadowVolumeDrawer {
public:
    ShadowVolumeModel(const char*);

    inline virtual ~ShadowVolumeModel();
    virtual void drawShape() const;

    void initVolumeModel(const char*);

    J3DModelData* mModelData;  // 0x1C
};
