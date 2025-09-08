#pragma once

#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"
#include "JSystem/J3DGraphAnimator/J3DModel.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"

class ResourceHolder;
class J3DModelData;
class J3DMaterialAnm;
class J3DAnmBase;
class ResTable;

struct J3DModelData;
struct J3DAnmColor       { void searchUpdateMaterialID(J3DModelData*); };
struct J3DAnmTexPattern  { void searchUpdateMaterialID(J3DModelData*); };
struct J3DAnmTextureSRTKey { void searchUpdateMaterialID(J3DModelData*); };
struct J3DAnmTevRegKey   { void searchUpdateMaterialID(J3DModelData*); };
struct J3DAnmColorKey;
struct J3DModelData; 

struct ResTable {
    const char* getResName(unsigned long index) const;
    const void* getRes(unsigned long index) const;
    /* 0x04 */ unsigned int mCount; 
};


class MaterialAnmBuffer {
public:
    MaterialAnmBuffer(const ResourceHolder* pRes, J3DModelData* pModel, bool makeDiff);
    int  searchUpdateMaterialID(const ResourceHolder* pRes, J3DModelData* pModel);
    void setDiffFlag(const ResourceHolder* pRes);
    int  getAllocMaterialAnmNum(J3DModelData* pModel, bool makeDiff) const;
    void attachMaterialAnmBuffer(J3DModelData* pModel, bool makeDiff);
    int getDiffFlag(long index) const;
    int getDifferedMaterialNum(const J3DModelData* pModel) const;

private:

};
