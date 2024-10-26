#include "Game/AreaObj/BigBubbleGoalArea.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/ObjUtil.hpp"

BigBubbleGoalArea::BigBubbleGoalArea(int type, const char *pName) : AreaObj(type, pName) {
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;
    _48 = 1.0f;
    _4C = 0.0f;
    _50 = 0.0f;
}

BigBubbleGoalArea::~BigBubbleGoalArea() {

}

void BigBubbleGoalArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    _48 = mObjArg0 <= 0 ? 1.0f : (f32)mObjArg0;

    MR::getJMapInfoTrans(rIter, &mTranslation);
    MR::connectToScene(this, 34, -1, -1, 16);
}

void BigBubbleGoalArea::movement() {
    if (!isValidSwitchA() || isOnSwitchA()) {
        return;
    }

    if (!(_4C < _50)) {
        return;
    }

    _4C += 0.05f;

    if (_4C >= _48) {
        _4C = _48;
        onSwitchA();
        mValid = false;
    }
    else if (_50 < _4C) {
        _4C = _50;
    }
}

void BigBubbleGoalArea::draw() const {
    if (!isValidSwitchA() || isOnSwitchA()) {
        return;
    }
    
    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetCullMode(GX_CULL_BACK);

    f32 dVar3 = _4C / _48;
    f32 dVar4 = 6.2831855f * dVar3;

    TDDraw::drawFillFan(mTranslation, MR::getCamZDir(), MR::getCamYDir() * 150.0f, 0xFFFF00FF, 0.0f, dVar4, 16);
    TDDraw::drawFillFan(mTranslation, MR::getCamZDir(), MR::getCamYDir() * 150.0f, 0x80FF, dVar4, 150.0f, 16);
}

const char *BigBubbleGoalArea::getManagerName() const {
    return "BigBubbleGoalArea";
}
