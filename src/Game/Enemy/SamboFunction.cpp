#include "Game/Enemy/SamboFunction.hpp"
#include "Game/Enemy/AnimScaleController.hpp"

namespace SamboFunction {
    AnimScaleController* createAnimScaleController(AnimScaleParam * pParam) {
        pParam->_10 = 4.0f;
        pParam->_14 = 1.0f;
        pParam->_18 = 0.06f;
        pParam->_1C = 2.0f;
        pParam->_20 = 0.06f;
        pParam->_24 = 0.4f;
        pParam->_28 = 0.8f;
        return new AnimScaleController(pParam);
    }
}
