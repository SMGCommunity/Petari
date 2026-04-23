
#include <revolution.h>

class J3DMaterial;
class J3DShapePacket;
class J3DModel;

class ShapePacketUserData {
public:
    ShapePacketUserData();

    void init(J3DMaterial*);
    void callDL() const;
    void loadTexMtx(J3DMaterial*, int, u16) const;

    //
};

namespace MR {
    ShapePacketUserData* getJ3DShapePacketUserData(const J3DShapePacket*);
    void initJ3DShapePacketUserData(J3DModel*);
}  // namespace MR
