#include "Model/J3D/J3DModel.h"
#include "os/mtx.h"

void J3DModel::initialize()
{
    f32 defaultScale = J3DDefaultScale;
    this->_4 = 0;
    this->_8 = 0;
    this->_C = 0;
    this->_10 = 0;
    this->_14 = 0;
    this->mBaseScale.x = defaultScale;
    this->mBaseScale.y = defaultScale;
    this->mBaseScale.z = defaultScale;
    PSMTXIdentity(this->_24);
    PSMTXIdentity(this->_54);
    this->_84 = 0;
    this->_C0 = 0;
    this->_C4 = 0;
    this->_C8 = 0;
    this->_CC = 0;
    this->_D0 = 0;
    this->_D4 = 0;
    this->_D8 = 0;
}