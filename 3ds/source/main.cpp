// Petari 3DS bring-up: runs a few decomp math routines on-device and prints the results next to the expected values.
// Only <3ds.h>/<stdio.h> here; all Petari code lives in petari_test.cpp (see the note there).
#include <3ds.h>
#include <stdio.h>

extern "C" int petari_run_tests(float* out);

static void row(const char* name, const float* v, int count, const char* expect) {
    printf("%-14s", name);
    for (int i = 0; i < count; i++) {
        printf(" %.3f", v[i]);
    }
    printf("\n   expect: %s\n", expect);
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);

    float r[32];
    petari_run_tests(r);

    printf("Petari 3DS bring-up\n\n");
    row("FastSqrt(2)", r + 0, 1, "~1.414 (exact 1.4142)");
    row("FastSqrt(-1)", r + 1, 1, "-1.000 (passthrough)");
    row("VECLerp .25", r + 2, 3, "2.500 5.000 7.500");
    row("VECScaleAdd 2", r + 5, 3, "20.000 40.000 60.000");
    row("QuatLerp .5", r + 8, 4, "0.000 0.500 0.000 0.500");
    row("Hermite", r + 12, 1, "0.625 (my decode of the PPC asm)");
    row("ApplyScale", r + 13, 3, "10.000 15.000 20.000");

    printf("\nPress START to exit.\n");

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
