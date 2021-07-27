#pragma once

#include <revolution.h>

enum J3DAnmKind
{
    J3DAnmKind_Trans = 0x0,
    J3DAnmKind_Color = 0x1,
    J3DAnmKind_TextPattern = 0x2,
    J3DAnmKind_SRTKey = 0x4,
    J3DAnmKind_TevRegKey = 0x5,
    J3DAnmKind_VertexColor = 0x7,

    J3DAnmKind_TransFull = 0x9,
    J3DAnmKind_ColorFull = 0xA,
    J3dAnmKind_ColorKey = 0xB,
    J3DAnmKind_ClusterFull = 0xC,
    J3DAnmKind_ClusterKey = 0xD,
    J3DAnmKind_VertexColorFull = 0xE,
    J3DAnmKind_VertexColorKey = 0xF,
    J3DAnmKind_TransFullWithLerp = 0x10
};

class J3DAnmBase
{
public:
    J3DAnmBase();
    J3DAnmBase(s16);

    virtual ~J3DAnmBase();
    virtual J3DAnmKind getKind() const = 0;

    u8 _4;
    u8 _5;
    u16 _6;
    f32 _8;
};

class J3DAnmColor : public J3DAnmBase
{
public:
    J3DAnmColor();
};

class J3DAnmVtxColor : public J3DAnmBase
{
public:
    J3DAnmVtxColor();
};

class J3DFrameCtrl
{
public:
    virtual ~J3DFrameCtrl();

    void init(s16);
    bool checkPass(f32);
    void update();

    u8 _4;
    u8 _5;
    u16 _6;
    u16 _8;
    u16 _A;
    f32 _C;
    f32 _10;
};

static f32 J3DHermiteInterpolation(f32, const f32 *, const f32 *, const f32 *, const f32 *, const f32 *, const f32 *);