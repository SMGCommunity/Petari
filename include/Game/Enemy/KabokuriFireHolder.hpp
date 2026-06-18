#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class KabokuriFire;

class KabokuriFireHolder : public DeriveActorGroup< KabokuriFire > {
public:
    /// @brief Creates a new `KabokuriFireHolder`.
    KabokuriFireHolder();
};

namespace KabokuriFireHolderFunc {
    void createHolder();
    KabokuriFire* getDeadMember();
    bool generateFire(const TVec3f&, const TVec3f&);
};  // namespace KabokuriFireHolderFunc
