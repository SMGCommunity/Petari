#pragma once

#include <revolution/mem.h>
#include <new>

namespace nw4r {
    namespace lyt {
        class Layout {
        public:
            static MEMAllocator* mspAllocator;

            static void* AllocMemory(u32 size) {
                return MEMAllocFromAllocator(mspAllocator, size);
            }

            static void FreeMemory(void *mem) {
                MEMFreeToAllocator(mspAllocator, mem);
            }

            template<typename T>
            static void DeleteObj(T *);

            template<typename T>
            static T* NewArray(u32);

            template<typename T>
            static void DeleteArray(T [], u32);

        };

        MEMAllocator* Layout::mspAllocator;

        template<typename T>
        T* Layout::NewArray(u32 num) {
            void* mem = AllocMemory(sizeof(T) * num);

            if (mem == nullptr) {
                return nullptr;
            }

            T* const objAry = static_cast<T*>(mem);

            for (u32 i = 0; i < num; i++) {
                new (&objAry[i]) T();
            }

            return objAry;
        }

        template<typename T>
        void Layout::DeleteObj(T *pObj) {
            if (pObj != nullptr) {
                pObj->~T();
                FreeMemory(pObj);
            }
        }

        template<typename T>
        void Layout::DeleteArray(T objAry[], u32 num) {
            if (objAry) {
                for (u32 i = 0; i < num; i++) {
                    objAry[i].~T();
                }

                FreeMemory(objAry);
            }
        }
    };
};