#pragma once

#include <revolution.h>

class LiveActor;

namespace MR {
    MtxPtr getJointMtx(const LiveActor *, const char *);
};