#pragma once

#include "Game/LiveActor/ShadowVolumeDrawer.hpp"

class J3DModelData;

class ShadowVolumeModel : public ShadowVolumeDrawer {
public:
    ShadowVolumeModel(const char *);

    virtual ~ShadowVolumeModel();
    virtual void drawShape() const;

    void initVolumeModel(const char *);

    J3DModelData* mModelData;   // _1C
};