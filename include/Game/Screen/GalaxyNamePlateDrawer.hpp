#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

class LayoutActor;

struct GalaxyNamePlatePrioritied {
    /* 0x0 */ const LayoutActor* mHostActor;
    /* 0x4 */ u32 mZ;

    GalaxyNamePlatePrioritied(const LayoutActor* pActor) : mHostActor(pActor), mZ() {
    }
};

class GalaxyNamePlateDrawer : public NameObj {
public:
    /// @brief Creates a new `GalaxyNamePlateDrawer`.
    GalaxyNamePlateDrawer();

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;

    GalaxyNamePlatePrioritied* registerPlate(const LayoutActor*);

private:
    /* 0xC */ MR::Vector< MR::FixedArray< GalaxyNamePlatePrioritied*, 16 > > mPlateArray;
};

namespace MR {
    GalaxyNamePlatePrioritied* registerToGalaxyNamePlateDrawer(const LayoutActor*);
};  // namespace MR
