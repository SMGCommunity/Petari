#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class TakoHeiInk;

class TakoHeiInkHolder : public DeriveActorGroup< TakoHeiInk > {
public:
    /// @brief Creates a new `TakoHeiInkHolder`.
    TakoHeiInkHolder();
};

namespace MR {
    void createTakoHeiInkHolder();
    bool spurtTakoHeiInk(const TVec3f&, const TVec3f&);
};  // namespace MR
