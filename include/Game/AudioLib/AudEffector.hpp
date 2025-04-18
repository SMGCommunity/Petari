#include "JSystem/JKernel/JKRHeap.hpp"
#include "revolution/types.h"

class AudEffector {
public:
    AudEffector();

    void initFxLine(JKRHeap *);
    void setFxSend(f32);
    void setCutoff(s32);
    void setSpecialFx(s32);
    void initParams(s32);

    // TODO
};
