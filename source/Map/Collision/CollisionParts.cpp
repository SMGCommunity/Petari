#include "Map/Collision/CollisionParts.h"
#include "Map/Collision/KCollisionServer.h"
#include "JSystem/JGeometry/TMatrix34.h"

CollisionParts::CollisionParts()
{
    _0 = 0;
    _C8 = 0;
    _CC = 0;
    _CD = 1;
    _CE = 0;
    _CF = 0;
    _D0 = 0;
    _D4 = 0;
    _D8 = -1.0f;
    _DC = 1.0f;
    _E0 = -1;
    _E4 = 0;

    mCollisionServer = new KCollisionServer();
    
    // todo -- finish me
}