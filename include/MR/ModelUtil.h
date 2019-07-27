#ifndef MODELUTIL_H
#define MODELUTIL_H

#include "Actor/LiveActor/LiveActor.h"
#include "Model/J3D/J3DModel.h"

namespace MR
{
    J3DModel* getJ3DModel(const LiveActor *);
}

#endif // MODELUTIL_H