#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Kamek.hpp"
#include "Macros.hpp"

#define AREA_FORM_CUBE 0 // Center origin
#define AREA_FORM_BOX 1  // Bottom origin
#define AREA_FORM_SPHERE 2
#define AREA_FORM_CYLINDER 3
#define AREA_FORM_BOWN 4

namespace {
    template <typename T>
    static NameObj* createExtNameObj(const char* pName) {
        return new T(pName);
    }

    template <typename T, u32 Form>
    static NameObj* createExtAreaObj(const char* pName) {
        return new T(Form, pName);
    }

    static const NameObjFactory::Name2CreateFunc cCreateFuncTable[] = {
        {nullptr, nullptr, nullptr},
    };

    static const NameObjFactory::Name2MakeArchiveListFunc cArchiveListFuncTable[] = {
        {nullptr, nullptr},
    };

    static const NameObjFactory::Name2CreateFunc* getName2CreateFunc() {
        register const char* pName;
        GET_REGISTER(pName, r29);

        for (const NameObjFactory::Name2CreateFunc* pEntry = &cCreateFuncTable[1]; pEntry < ARRAY_END(cCreateFuncTable); pEntry++) {
            if (MR::isEqualStringCase(pEntry->mName, pName)) {
                return pEntry;
            }
        }

        return nullptr;
    }

    static void getMountObjectArchiveList(NameObjArchiveListCollector* pCollector, const char* pName, const JMapInfoIter& rIter) {
        for (const NameObjFactory::Name2MakeArchiveListFunc* pEntry = &cArchiveListFuncTable[1]; pEntry < ARRAY_END(cArchiveListFuncTable);
             pEntry++) {
            if (MR::isEqualString(pEntry->mName, pName)) {
                pEntry->mArchiveFunc(pCollector, rIter);
            }
        }
    }

    // clang-format off
#ifdef __MWERKS__
    static asm void saveRegister() {
        nofralloc
        addi r11, r1, 0x20
        mr   r3, r27 // NameObjArchiveListCollector* pCollector
        mr   r4, r28 // const char* pName
        mr   r5, r29 // const JMapInfoIter& rIter
        blr
    }
#else
    static void saveRegister() {}
#endif
    // clang-format on
} // namespace

extern kmSymbol getName2CreateFunc__14NameObjFactoryFPCcPCQ214NameObjFactory15Name2CreateFunc;
kmCall(&getName2CreateFunc__14NameObjFactoryFPCcPCQ214NameObjFactory15Name2CreateFunc + 0x6C, getName2CreateFunc);

extern kmSymbol getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter;
kmCall(&getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter + 0xEC, saveRegister);
kmBranch(&getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter + 0x100, getMountObjectArchiveList);