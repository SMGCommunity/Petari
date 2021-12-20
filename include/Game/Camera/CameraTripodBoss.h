#include "Game/Camera/CameraTower.h"

class CameraTripodBoss : public CameraTower {
public:
    CameraTripodBoss(const char *);

    virtual CameraTargetObj *calc();

    f32 _8C;
};