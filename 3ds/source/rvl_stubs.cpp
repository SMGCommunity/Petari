// Portable stand-ins for the RVL_SDK pieces the ported files link against.
// The real ones (src/RVL_SDK/mtx) are PPC paired-single asm and cannot be built for ARM.
// Same signatures and semantics as the originals; grow this file as more of Petari is added.
#include <revolution/mtx.h>

extern "C" {

void PSMTXScale(Mtx m, f32 x, f32 y, f32 z) {
    m[0][0] = x;    m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = y;    m[1][2] = 0.0f; m[1][3] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = z;    m[2][3] = 0.0f;
}

// ab = a * b for 3x4 affine matrices (implicit 4th row 0 0 0 1). Safe if ab aliases a or b.
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab) {
    Mtx t;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            t[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];
        }
        t[i][3] = a[i][0] * b[0][3] + a[i][1] * b[1][3] + a[i][2] * b[2][3] + a[i][3];
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            ab[i][j] = t[i][j];
        }
    }
}

}  // extern "C"
