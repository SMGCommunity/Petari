#include "Game/NPC/MiiFacePartsHolder.hpp"

MiiFacePartsHolder::MiiFacePartsHolder(int numParts) : LiveActorGroup("Mii顔モデル保持", numParts), JKRDisposer() {
    _30 = 0;
    _34 = 0;
}