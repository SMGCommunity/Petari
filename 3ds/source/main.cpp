// Petari 3DS bring-up: runs a few decomp math routines on-device and prints the results.
// Compare the output against the expected values to confirm the C fallbacks match the PPC asm.
#include <3ds.h>
#include <cstdio>

#include "JSystem/JMath/JMath.hpp"

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);

    std::printf("Petari 3DS bring-up\n\n");

    std::printf("JMAFastSqrt(2.0)      = %f (exact 1.414214)\n", JMAFastSqrt(2.0f));
    std::printf("JMAFastSqrt(-1.0)     = %f (expect -1.0, passthrough)\n", JMAFastSqrt(-1.0f));

    Vec a = {0.0f, 0.0f, 0.0f}, b = {10.0f, 20.0f, 30.0f}, out;
    JMAVECLerp(&a, &b, &out, 0.25f);
    std::printf("VECLerp t=.25         = %.2f %.2f %.2f (expect 2.50 5.00 7.50)\n", out.x, out.y, out.z);

    JMAVECScaleAdd(&b, &a, &out, 2.0f);
    std::printf("VECScaleAdd s=2       = %.2f %.2f %.2f (expect 20 40 60)\n", out.x, out.y, out.z);

    Quaternion p = {0.0f, 0.0f, 0.0f, 1.0f}, q = {0.0f, 1.0f, 0.0f, 0.0f}, r;
    JMAQuatLerp(&p, &q, 0.5f, &r);
    std::printf("QuatLerp t=.5         = %.2f %.2f %.2f %.2f\n", r.x, r.y, r.z, r.w);

    std::printf("Hermite(0..1 @0.5)    = %f\n", JMAHermiteInterpolation(0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f));

    std::printf("\nPress START to exit.\n");

    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        }
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
