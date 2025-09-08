#include "Game/Animation/MaterialAnmBuffer.hpp"
#include "Game/Util.hpp"


extern "C" void searchUpdateMaterialID__11J3DAnmColor(const J3DAnmColor* a, J3DModelData* m) {
    const_cast<J3DAnmColor*>(a)->searchUpdateMaterialID(m);
}
extern "C" void searchUpdateMaterialID__16J3DAnmTexPattern(const J3DAnmTexPattern* a, J3DModelData* m) {
    const_cast<J3DAnmTexPattern*>(a)->searchUpdateMaterialID(m);
}
extern "C" void searchUpdateMaterialID__19J3DAnmTextureSRTKey(const J3DAnmTextureSRTKey* a, J3DModelData* m) {
    const_cast<J3DAnmTextureSRTKey*>(a)->searchUpdateMaterialID(m);
}
extern "C" void searchUpdateMaterialID__15J3DAnmTevRegKey(const J3DAnmTevRegKey* a, J3DModelData* m) {
    const_cast<J3DAnmTevRegKey*>(a)->searchUpdateMaterialID(m);
}


extern const char lbl_806B1640[];
extern const char lbl_806B1644[];
extern const char lbl_806B1648[];

static const char* sTagColorKey   = lbl_806B1640;
static const char* sTagTexPattern = lbl_806B1644;
static const char* sTagTexSrt     = lbl_806B1648;

typedef unsigned long J3DDiffFlag;

void modifyDiffFlag(unsigned long*,
                    const J3DAnmColorKey*,
                    unsigned long,
                    bool,
                    const char*,
                    const char*);

void modifyDiffFlag(unsigned long*,
                    const J3DAnmTexPattern*,
                    unsigned long,
                    bool,
                    const char*,
                    const char*);

void modifyDiffFlag(unsigned long*,
                    const J3DAnmTextureSRTKey*,
                    unsigned long,
                    bool,
                    const char*,
                    const char*);

void modifyDiffFlagBrk(unsigned long*,
                       const J3DAnmTevRegKey*,
                       bool,
                       const char*);

extern "C" void searchUpdateMaterialID__11J3DAnmColor(const J3DAnmColor*, J3DModelData*);
extern "C" void searchUpdateMaterialID__16J3DAnmTexPattern(const J3DAnmTexPattern*, J3DModelData*);
extern "C" void searchUpdateMaterialID__19J3DAnmTextureSRTKey(const J3DAnmTextureSRTKey*, J3DModelData*);
extern "C" void searchUpdateMaterialID__15J3DAnmTevRegKey(const J3DAnmTevRegKey*, J3DModelData*);

extern "C" void* __nwa__FUl(unsigned long);
extern "C" void* __construct_new_array(void* mem,
                                       void (*ctor)(void*),
                                       void (*dtor)(void*),
                                       unsigned long elemSize,
                                       unsigned long count);

struct J3DMaterialAnm;
extern "C" void __ct__14J3DMaterialAnmFv(void* self);
extern "C" void __dt__14J3DMaterialAnmFv(void* self, int);


#ifdef MAB_CONSTRUCTOR_NON_MATCHING
// NON_MATCHING: temporary stub to keep build going, working on matching constructor (https://decomp.me/scratch/8INdY)
MaterialAnmBuffer::MaterialAnmBuffer(const ResourceHolder* /*pRes*/,
                                     J3DModelData* /*pModel*/,
                                     bool /*makeDiff*/)
{
    *reinterpret_cast<void**>(this) = 0;
    *reinterpret_cast<void**>(reinterpret_cast<char*>(this) + 0x04) = 0;
}
#endif

int MaterialAnmBuffer::getDiffFlag(long index) const {
    unsigned long* arr = *reinterpret_cast<unsigned long* const*>(
        reinterpret_cast<const char*>(this) + 0x04);
    return arr[index];
}

int MaterialAnmBuffer::getAllocMaterialAnmNum(J3DModelData* pModel, bool makeDiff) const {
    if (makeDiff) {
        return getDifferedMaterialNum(pModel);
    } else {
        return *reinterpret_cast<unsigned short*>(
            reinterpret_cast<char*>(pModel) + 0x5C);
    }
}

int MaterialAnmBuffer::searchUpdateMaterialID(const ResourceHolder* pRes, J3DModelData* pModel) {
    {
        ResTable* table = *reinterpret_cast<ResTable* const*>(
            reinterpret_cast<const char*>(pRes) + 0x0C);
        unsigned int idx = 0;
        while (idx < *reinterpret_cast<unsigned int*>(
                           reinterpret_cast<char*>(table) + 0x04)) {
            const void* res = table->getRes(idx);
            searchUpdateMaterialID__11J3DAnmColor(
                static_cast<const J3DAnmColor*>(res), pModel);
            idx++;
        }
    }

    {
        ResTable* table = *reinterpret_cast<ResTable* const*>(
            reinterpret_cast<const char*>(pRes) + 0x10);
        unsigned int idx = 0;
        while (idx < *reinterpret_cast<unsigned int*>(
                           reinterpret_cast<char*>(table) + 0x04)) {
            const void* res = table->getRes(idx);
            searchUpdateMaterialID__16J3DAnmTexPattern(
                static_cast<const J3DAnmTexPattern*>(res), pModel);
            idx++;
        }
    }

    {
        ResTable* table = *reinterpret_cast<ResTable* const*>(
            reinterpret_cast<const char*>(pRes) + 0x08);
        unsigned int idx = 0;
        while (idx < *reinterpret_cast<unsigned int*>(
                           reinterpret_cast<char*>(table) + 0x04)) {
            const void* res = table->getRes(idx);
            searchUpdateMaterialID__19J3DAnmTextureSRTKey(
                static_cast<const J3DAnmTextureSRTKey*>(res), pModel);
            idx++;
        }
    }

    {
        ResTable* table = *reinterpret_cast<ResTable* const*>(
            reinterpret_cast<const char*>(pRes) + 0x18);
        unsigned int idx = 0;
        while (idx < *reinterpret_cast<unsigned int*>(
                           reinterpret_cast<char*>(table) + 0x04)) {
            const void* res = table->getRes(idx);
            searchUpdateMaterialID__15J3DAnmTevRegKey(
                static_cast<const J3DAnmTevRegKey*>(res), pModel);
            idx++;
        }
    }
}

// MaterialAnmBuffer::setDiffFlag()

int MaterialAnmBuffer::getDifferedMaterialNum(const J3DModelData* pModel) const {
    const unsigned short total =
        *reinterpret_cast<const unsigned short*>(
            reinterpret_cast<const char*>(pModel) + 0x5C);

    unsigned short acc = 0;   // r6
    unsigned short idx = 0;   // r7

    goto check;

loop: {
        const unsigned long* base =
            *reinterpret_cast<unsigned long* const*>(
                reinterpret_cast<const char*>(this) + 0x04);
        const unsigned long val = base[(unsigned int)idx];
        if (val != 0) {
            acc = static_cast<unsigned short>(acc + 1);
        }
    }

    ++idx; 

check: {
        const unsigned int cmp = static_cast<unsigned int>(idx);
        if (cmp < static_cast<unsigned int>(total)) {
            goto loop;  
        }
    }

    return acc;
}
