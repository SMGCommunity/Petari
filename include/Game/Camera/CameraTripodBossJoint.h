#include "Game/Camera/Camera.h"

class CameraTripodBossJoint : public Camera {
public:
    CameraTripodBossJoint(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58[16];
    u8 _68;
    u8 _69[3];
    f32 _6C;
    f32 _70;
};