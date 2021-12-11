#include "Game/Camera/CameraPoseParam.h"

void CameraPoseParam::copyFrom(const CameraPoseParam &rOther) {
    _0.set(rOther._0); 
    _C.set(rOther._C);
    _18.set(rOther._18);
    _24.set(rOther._24);
    _30 = rOther._30; 
    _34.set(rOther._34); 
    _40.set(rOther._40);
    _4C = rOther._4C;
    _50 = rOther._50; 
    _54 = rOther._54;
}