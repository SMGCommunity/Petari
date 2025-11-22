#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Util/Array.hpp"

struct GalaxyNamePlateDrawerEntry {
    /* 0x0 */ LayoutActor* mHostActor;
    /* 0x4 */ u32          mZ;
};

class GalaxyNamePlateDrawer : public NameObj {
public:
    /// @brief Creates a new `GalaxyNamePlateDrawer`.
    GalaxyNamePlateDrawer();

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;

    GalaxyNamePlateDrawerEntry* registerPlate(const LayoutActor*);

private:
    /* 0xC */ MR::Vector<MR::FixedArray<GalaxyNamePlateDrawerEntry*, 16>> mPlateArray;
};

namespace MR {
    GalaxyNamePlateDrawerEntry* registerToGalaxyNamePlateDrawer(const LayoutActor*);
};
