// Petari-side test code. Compiled with the Petari/MSL include paths and deliberately does NOT include
// <3ds.h> or <stdio.h>: the MSL headers conflict with devkitARM's libc headers. main.cpp prints the results.
#include "JSystem/JMath/JMath.hpp"

extern "C" int petari_run_tests(float* out) {
    int n = 0;
    out[n++] = JMAFastSqrt(2.0f);
    out[n++] = JMAFastSqrt(-1.0f);

    Vec a = {0.0f, 0.0f, 0.0f}, b = {10.0f, 20.0f, 30.0f}, v;
    JMAVECLerp(&a, &b, &v, 0.25f);
    out[n++] = v.x; out[n++] = v.y; out[n++] = v.z;

    JMAVECScaleAdd(&b, &a, &v, 2.0f);
    out[n++] = v.x; out[n++] = v.y; out[n++] = v.z;

    Quaternion p = {0.0f, 0.0f, 0.0f, 1.0f}, q = {0.0f, 1.0f, 0.0f, 0.0f}, r;
    JMAQuatLerp(&p, &q, 0.5f, &r);
    out[n++] = r.x; out[n++] = r.y; out[n++] = r.z; out[n++] = r.w;

    out[n++] = JMAHermiteInterpolation(0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);

    Mtx m, d;
    PSMTXScale(m, 5.0f, 5.0f, 5.0f);
    JMAMTXApplyScale(m, d, 2.0f, 3.0f, 4.0f);
    out[n++] = d[0][0]; out[n++] = d[1][1]; out[n++] = d[2][2];
    return n;
}
