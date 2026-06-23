#include "Game/AreaObj/BigBubbleGoalArea.hpp"
#include "Game/Ride/BigBubble.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const f32 sMeterUpSpeed = 0.05f;
};  // namespace

void BigBubbleGoalArea_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

BigBubbleGoalArea::BigBubbleGoalArea(int formType, const char* pName)
    : AreaObj(formType, pName), mTrans(0.0f, 0.0f, 0.0f), mMeterCapacity(1.0f), mMeterDisplayAmount(), mMeterAmount() {
}

void BigBubbleGoalArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    mMeterCapacity = mObjArg0 <= 0 ? 1.0f : mObjArg0;

    MR::getJMapInfoTrans(rIter, &mTrans);
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_BigBubbleGoal);
}

void BigBubbleGoalArea::addBubble(BigBubble* pBubble) {
    if (isValidSwitchA() && !isOnSwitchA()) {
        f32 volume = pBubble->mVolume;
        mMeterAmount += volume;

        if (mMeterAmount >= mMeterCapacity) {
            mMeterAmount = mMeterCapacity;
        }
    }
}

void BigBubbleGoalArea::movement() {
    if (!isValidSwitchA() || isOnSwitchA()) {
        return;
    }

    if (!(mMeterDisplayAmount < mMeterAmount)) {
        return;
    }

    mMeterDisplayAmount += ::sMeterUpSpeed;

    if (mMeterDisplayAmount >= mMeterCapacity) {
        mMeterDisplayAmount = mMeterCapacity;
        onSwitchA();
        mIsValid = false;
        return;
    }

    if (mMeterDisplayAmount > mMeterAmount) {
        mMeterDisplayAmount = mMeterAmount;
    }
}

void BigBubbleGoalArea::draw() const {
    if (!isValidSwitchA() || isOnSwitchA()) {
        return;
    }

    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetCullMode(GX_CULL_BACK);

    f32 f = TWO_PI * (mMeterDisplayAmount / mMeterCapacity);

    TDDraw::drawFillFan(mTrans, MR::getCamZdir(), MR::getCamYdir() * 150.0f, 0xFFFF00FF, 0.0f, f, 16);
    TDDraw::drawFillFan(mTrans, MR::getCamZdir(), MR::getCamYdir() * 150.0f, 0x80FF, f, TWO_PI, 16);
}

namespace MR {
    bool checkBigBubbleGoal(BigBubble* pBubble) {
        BigBubbleGoalArea* goalArea = static_cast< BigBubbleGoalArea* >(MR::getAreaObj("BigBubbleGoalArea", pBubble->mPosition));

        if (goalArea != nullptr) {
            goalArea->addBubble(pBubble);

            return true;
        }

        return false;
    }
};  // namespace MR
