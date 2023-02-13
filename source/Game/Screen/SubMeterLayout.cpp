#include "Game/Screen/SubMeterLayout.h"

SubMeterLayout::SubMeterLayout(const char *pName, const char *pLayoutName) : LayoutActor(pName, true) {
    MR::connectToSceneLayout(this);
    initLayoutManager(pLayoutName, 1);
    MR::createAndAddPaneCtrl(this, "Count", 1);
}

void SubMeterLayout::init(const JMapInfoIter &rIter) {
    MR::startPaneAnim(this, "Count", "Count", 0);
    MR::setPaneAnimFrameAndStop(this, "Count", 0.0f, 0);
    appear();
    MR::hideLayout(this);
    _20 = 0;
    _26 = 0;
}

void SubMeterLayout::setLifeRatio(f32 ratio) {
    f32 mult = 128.0f * ratio;
    _22 = mult;
    MR::setPaneAnimFrameAndStop(this, "Count", 0x80 - _22, 0);
}

void SubMeterLayout::requestDeactivate() {
    MR::hideLayout(this);
    _20 = 0;
}

void SubMeterLayout::requestAppear() {
    MR::showLayout(this);
    MR::startAnim(this, "Appear", nullptr);
    _20 = 1;
    control();
}

void SubMeterLayout::requestFrameIn() {
    MR::showLayout(this);
    MR::startAnim(this, "Appear", 0);
    _20 = 1;
    control();
}

void SubMeterLayout::requestFrameOut() {
    MR::startAnim(this, "End", 0);
    _20 = 4;
}

void SubMeterLayout::control() {
    switch(_20) {
        case 0:
            break;
        case 1:
            if (MR::isAnimStopped(this, 0)) {
                MR::startAnim(this, "Wait", 0);
                _26 = 0;
                _20 += 1;
            }
            break;
        case 2:
            if (_22 < 0x40) {
                MR::startAnim(this, "Danger", 0);
                _20 = 3;
            }
            break;
        case 3:
            if (_22 > 0x40) {
                MR::startAnim(this, "Wait", 0);
                MR::setAnimRate(this, 1.0f, 0);
                _20 = 2;
            }
            else {
                MR::setAnimRate(this, ((0x40 - _22) * 1.0f / 32.0f) + 1.0f, 0); 
            }
            break;
        case 4:
            if (MR::isAnimStopped(this, 0)) {
                MR::hideLayout(this);
                _20 = 0;
            }
            break;
    }

    bool flag = true;

    if (!MR::isPlayerInWaterMode()) {
        if (!MR::isOnGroundPlayer()) {
            flag = false;
        }
    }
    else {
        if (!MR::isPlayerOnWaterSurface()) {
            flag = false;
        }
    }

    if (flag) {
        if (((_20 + 0xFE) & 0xFF) <= 1u && _22 == 0x80) { 
            if (++_26 == 60) {
                MR::startAnim(this, "End", 0);
                _20 = 5;
            }
        }
    }
    else if (_20 == 5) {
        _20 = 1;
        _26 = 0;
        MR::startAnim(this, "Appear", 0);
    }

    if (!MR::isHiddenLayout(this)) {
        TVec2f screenPos;
        MR::calcScreenPosition(&screenPos, *MR::getPlayerPos());
        screenPos.x += sScreenOffsetX;
        screenPos.y += sScreenOffsetY;
        setTrans(screenPos);
    }
}

SubMeterLayout::~SubMeterLayout() {

}