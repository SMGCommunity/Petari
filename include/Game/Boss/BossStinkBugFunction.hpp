#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class BossStinkBug;
class HitSensor;

namespace BossStinkBugFunction {
    void addHeightOffset(TVec3f*, const BossStinkBug*, f32);
    void addLocalHeightOffset(TVec3f*, BossStinkBug*, f32) NO_INLINE;
    void calcDiffCurrentRailPosition(TVec3f*, BossStinkBug*, f32, f32);
    void moveRail(BossStinkBug*, f32, f32, f32);
    void turnRailGround(BossStinkBug*, const TVec3f&, f32, bool);
    void turnRail(BossStinkBug*, const TVec3f&, f32, f32, f32, f32, f32, bool);
    void regainToRail(BossStinkBug*, const TVec3f&, f32, f32, f32, f32);
    void setFallVelocity(BossStinkBug*, const TVec3f&, f32, f32, f32);
    bool throwBomb(BossStinkBug*);
    void invalidateAttack(BossStinkBug*);
    bool isHipDropableSensor(const BossStinkBug*, const HitSensor*);
    bool isExistPlayerBack(const BossStinkBug*, f32);
    void validateAttack(BossStinkBug*);
}
