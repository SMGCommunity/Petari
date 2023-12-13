#include "Game/Camera/CameraShakePatternImpl.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"

CameraShakePatternSingly::CameraShakePatternSingly(float a1)  : CameraShakePattern() {
    _8 = a1;
    mDirection.x = 0.0f;
    mDirection.y = 1.0f;
}

CameraShakePatternSingly::~CameraShakePatternSingly() {

}

bool CameraShakePatternSingly::isEnd() const {
    return _4 >= 25;
}

void CameraShakePatternSingly::getOffset(TVec2f *pOffset) const {
    pOffset->set(mOffset);
}

void CameraShakePatternSingly::start() {
    mOffset.y = 0.0f;
    mOffset.x = 0.0f;
}

#ifdef NON_MATCHING
// Stack issue, fmuls operations order
void CameraShakePatternSingly::update() {
    s32 uStack44 = 25 - static_cast<s32>(_4);

    if (uStack44 < 0) {
        uStack44 = 0;
    }

    f32 dVar2 = MR::sin((12.566371f * static_cast<f32>(uStack44)) / 25.0f);
    f32 dVar3 = _8 * dVar2;
 
    f32 dVar2_2 = MR::sin((1.5707964f * static_cast<f32>(uStack44)) / 25.0f);

    dVar3 *= dVar2_2;
    
    f32 offsetX = mDirection.x * dVar3;
    f32 offsetY = mDirection.y * dVar3;

    TVec2f newOffset;
    newOffset.x = offsetX;
    newOffset.y = offsetY;

    TVec2f newOffset2 = newOffset;

    mOffset.set(newOffset2);
}
#endif

void CameraShakePatternSingly::setDirection(const TVec2f &rDir) {
    mDirection.set(rDir);
    MR::normalize(&mDirection);
}

CameraShakePatternVerticalSin::CameraShakePatternVerticalSin(float a1, float a2) {
    _8 = a1;
    _C = a2;
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
}

CameraShakePatternVerticalSin::~CameraShakePatternVerticalSin() {
    
}

bool CameraShakePatternVerticalSin::isEnd() const {
    f32 var = static_cast<f32>(_4);

    return _C <= var;
}

void CameraShakePatternVerticalSin::getOffset(TVec2f *pOffset) const {
    pOffset->set(mOffset);
}

void CameraShakePatternVerticalSin::start() {
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
}

void CameraShakePatternVerticalSin::update() {
    if (_C < 0.01) {
        mOffset.y = 0.0f;
        mOffset.x = 0.0f;
    }
    else {
        f32 dVar2 = MR::sin((6.2831855f * static_cast<f32>(_4)) / _C);

        f32 nVar2 = -dVar2;
        f32 var = _8 * nVar2;

        mOffset.x = 0.0f;
        mOffset.y = var;
    }
}