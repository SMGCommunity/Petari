#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>

/* there used to be class here used for debug drawing...but it got stripped away and only left the static data */
namespace {
    static Color8 sAreaCol = Color8(128, 0, 0, 128);
    static Color8 sLineCol = Color8(255, 255, 255, 255);
};  // namespace

void DUMMY() {
    // this is here *purely* to emit TVec3f::operator+
    TVec3f a;
    TVec3f b;
    TVec3f c = a + b;
    TVec3f d = c + b;
    TVec3f e(d);
}
