#include "Game/MapObj/LavaBreakColumn.hpp"
#include "Game/MapObj/AnmModelObj.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

void LavaBreakColumn_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    static const f32 sEffectPosOffsetY = 5000.0f;
};  // namespace

LavaBreakColumn::LavaBreakColumn(const char* pName) : AnmModelSwitchMoveEndKill(pName), mEffectPos(gZeroVec), mIsPillarFallen() {
}

void LavaBreakColumn::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    mIsPillarFallen = false;
}

void LavaBreakColumn::init(const JMapInfoIter& rIter) {
    AnmModelSwitchMove::init(rIter);
    MR::onCalcGravity(this);
    MR::setEffectHostSRT(this, "LavaColumn", &mEffectPos, nullptr, nullptr);
}

void LavaBreakColumn::control() {
    if (mIsPillarFallen) {
        return;
    }

    TVec3f pos;
    MR::copyJointPos(this, "Move", &pos);
    MR::calcMapGround(pos, &mEffectPos, ::sEffectPosOffsetY);
    TPos3f mtx;
    mtx.set(getBaseMtx());
    TVec3f up;
    mtx.getYDir(up);
    MR::normalize(&up);
    TVec3f offset = pos + up * -1000.0f;
    TVec3f diff = mEffectPos - offset;
    MR::normalize(&diff);

    if (diff.dot(mGravity) < 0.0f) {
        MR::emitEffect(this, "LavaColumn");
        MR::startSound(this, "SE_OJ_LAVA_COLUMN_FALL_ED");
        mIsPillarFallen = true;
    }
}

bool AnmModelSwitchMoveEndKill::isKilledAtMoveDone() const {
    return true;
}
