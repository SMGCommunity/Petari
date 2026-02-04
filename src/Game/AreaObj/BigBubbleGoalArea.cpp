#include "Game/AreaObj/BigBubbleGoalArea.hpp"
#include "Game/Ride/BigBubble.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const f32 sMeterUpSpeed = 150.0f;
}  // namespace

BigBubbleGoalArea::BigBubbleGoalArea(int type, const char* pName) : AreaObj(type, pName), mTranslation(0.0f, 0.0f, 0.0f) {
    mMeterCapacity = 1.0f;
    mMeterDisplayAmount = 0.0f;
    mMeterAmount = 0.0f;
}

void BigBubbleGoalArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    mMeterCapacity = mObjArg0 <= 0 ? 1.0f : static_cast< f32 >(mObjArg0);

    MR::getJMapInfoTrans(rIter, &mTranslation);
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_BigBubbleGoal);
}

void BigBubbleGoalArea::addBubble(BigBubble* pBubble) {
    if (isValidSwitchA() && !isOnSwitchA()) {
        f32 bubble21C = pBubble->mVolume;
        mMeterAmount += bubble21C;
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

    mMeterDisplayAmount += 0.05f;

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

    f32 dVar3 = mMeterDisplayAmount / mMeterCapacity;
    f32 dVar4 = TWO_PI * dVar3;

    TDDraw::drawFillFan(mTranslation, MR::getCamZdir(), MR::getCamYdir() * 150.0f, 0xFFFF00FF, 0.0f, dVar4, 16);
    TDDraw::drawFillFan(mTranslation, MR::getCamZdir(), MR::getCamYdir() * 150.0f, 0x80FF, dVar4, TWO_PI, 16);
}

namespace MR {
    bool checkBigBubbleGoal(BigBubble* pBubble) {
        BigBubbleGoalArea* goalArea = reinterpret_cast< BigBubbleGoalArea* >(MR::getAreaObj("BigBubbleGoalArea", pBubble->mPosition));
        if (goalArea != nullptr) {
            goalArea->addBubble(pBubble);
            return true;
        }
        return false;
    }
}  // namespace MR
