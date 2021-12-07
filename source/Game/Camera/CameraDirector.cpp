#include "Game/Camera/CameraPoseParam.h"

void CameraPoseParam::copyFrom(const CameraPoseParam &rOther) {
    _00.set(rOther._00); 
    _0C.set(rOther._0C);
    _18.set(rOther._18);
    _24.set(rOther._24);
    _30 = rOther._30; 
    _34.set(rOther._34); 
    _0C.set(rOther._40);
    _4C = rOther._4C;
    _50 = rOther._50; 
    _54 = rOther._54;
}