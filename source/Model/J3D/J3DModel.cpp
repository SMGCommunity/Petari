#include "Model/J3D/J3DModel.h"
#include "os/mtx.h"

void J3DModel::initialize()
{
    f32 defaultScale = J3DDefaultScale;
    _4 = 0;
    _8 = 0;
    _C = 0;
    _10 = 0;
    _14 = 0;
    mBaseScale.x = defaultScale;
    mBaseScale.y = defaultScale;
    mBaseScale.z = defaultScale;
    PSMTXIdentity(_24);
    PSMTXIdentity(_54);
    _84 = 0;
    _C0 = 0;
    _C4 = 0;
    _C8 = 0;
    _CC = 0;
    _D0 = 0;
    _D4 = 0;
    _D8 = 0;
}