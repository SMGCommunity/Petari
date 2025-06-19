#include "Game/Player/MarioActor.hpp"

// regswap
void MarioActor::bodyClap() {
    u32 i = 0;
    for (u32 i = 0; i < _7DC; i++) {
            TVec3f stack_14(_6DC[i]->mPosition);
            TVec3f stack_8(stack_14);
            JMathInlineVEC::PSVECSubtract2(&stack_8, &_2A0, &stack_8);
            if (PSVECMag(&stack_8) < 100.0f) {
                _6DC[i]->receiveMessage(0x87, getSensor("body"));
            }
    }
}
