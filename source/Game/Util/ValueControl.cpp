#include "Game/Util/ValueControl.hpp"

ValueControl::ValueControl(int a1) {
    _0 = 0;
    _4 = a1;
    _8 = 1;
}

void ValueControl::setDirToOne() {
    _8 = 1;
}

void ValueControl::setDirToOneResetFrame() {
    _8 = 1;
    resetFrame();
}

void ValueControl::setDirToZero() {
    _8 = -1;
}

void ValueControl::setDirToZeroResetFrame() {
    _8 = -1;
    resetFrame();
}

/*
void ValueControl::update() {
    if (_8 > 0 && _0 != _4 || ((-_8 & ~_8) >= 0) && _0) {
        _0 += _8;
    }
}
*/

void ValueControl::setZero() {
    _8 = -1;
    resetFrame();
    _0 = 0;
}

void ValueControl::setOne() {
    _8 = 1;
    resetFrame();
    _0 = _4;
}

void ValueControl::resetFrame() {
    if (_8 > 0) {
        _0 = 0;
    }
    else {
       _0 = _4;
    }
}