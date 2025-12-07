#pragma once

#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/group.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/LinkList.h"
#include <cstddef>
#include <new>
#include <revolution/mem.h>

namespace nw4r {
    namespace ut {
        struct Rect;

        template < typename C >
        class TagProcessorBase;
    };  // namespace ut
    namespace lyt {
        namespace res {
            struct AnimShareInfoList;
        };

        class ResourceAccessor;
        struct ResBlockSet;
        class Pane;
        class GroupContainer;
        class DrawInfo;

        class Layout;

        enum OriginType {
            ORIGINTYPE_TOPLEFT,
            ORIGINTYPE_CENTER,

            ORIGINTYPE_MAX
        };

#ifdef __MWERKS__
        typedef ut::LinkList< AnimTransform, offsetof(AnimTransform, mLink) > AnimTransformList;
#else
        typedef ut::LinkList< AnimTransform, 0 > AnimTransformList;
#endif

        class Layout {
        public:
            Layout();

            static void* AllocMemory(u32 size) { return MEMAllocFromAllocator(mspAllocator, size); }

            static void FreeMemory(void* mem) { MEMFreeToAllocator(mspAllocator, mem); }

            virtual ~Layout();
            virtual bool Build(const void*, ResourceAccessor*);
            virtual AnimTransform* CreateAnimTransform();
            virtual AnimTransform* CreateAnimTransform(const void*, ResourceAccessor*);
            virtual AnimTransform* CreateAnimTransform(const AnimResource&, ResourceAccessor*);
            virtual void BindAnimation(AnimTransform*);
            virtual void UnbindAnimation(AnimTransform*);
            virtual void UnbindAllAnimation();
            virtual void SetAnimationEnable(AnimTransform*, bool);
            virtual void CalculateMtx(const DrawInfo&);
            virtual void Draw(const DrawInfo&);
            virtual void Animate(u32);
            virtual void SetTagProcessor(ut::TagProcessorBase< wchar_t >*);

            const ut::Rect GetLayoutRect() const;

            static Pane* BuildPaneObj(s32, const void*, const ResBlockSet&);

            GroupContainer* GetGroupContainer() const { return mpGroupContainer; }

            static MEMAllocator* mspAllocator;

            template < typename T >
            static T* NewObj();

            template < typename T, typename Param1 >
            static T* NewObj(Param1 param1);

            template < typename T, typename Param1, typename Param2 >
            static T* NewObj(Param1 param1, Param2 param2);

            template < typename T >
            static T* NewArray(u32 num);

            template < typename T >
            static void DeleteObj(T*);

            template < typename T >
            static void DeleteArray(T objAry[], u32 num);

            template < typename T >
            static void DeletePrimArray(T objAry[]);

            AnimTransformList mAnimTransList;
            Pane* mpRootPane;
            GroupContainer* mpGroupContainer;
            Size mLayoutSize;
            u8 _20;
        };

        template < typename T >
        inline void Layout::DeleteObj(T* pObj) {
            if (pObj) {
                pObj->~T();
                FreeMemory(pObj);
            }
        }

        template < typename T >
        inline T* Layout::NewObj() {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T();
            }

            return 0;
        }

        template < typename T, typename Param1 >
        inline T* Layout::NewObj(Param1 param1) {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T(param1);
            }
            return 0;
        }

        template < typename T, typename Param1, typename Param2 >
        inline T* Layout::NewObj(Param1 param1, Param2 param2) {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T(param1, param2);
            }
            return 0;
        }

        template < typename T >
        T* Layout::NewArray(u32 num) {
            void* pMem = AllocMemory(sizeof(T) * num);
            T* const objAry = static_cast< T* >(pMem);

            for (u32 i = 0; i < num; ++i) {
                new (&objAry[i]) T();
            }

            return objAry;
        }

        template < typename T >
        inline void Layout::DeleteArray(T objAry[], u32 num) {
            if (objAry) {
                for (u32 i = 0; i < num; ++i) {
                    objAry[i].~T();
                }
                FreeMemory(objAry);
            }
        }

        template < typename T >
        inline void Layout::DeletePrimArray(T objAry[]) {
            if (objAry) {
                FreeMemory(objAry);
            }
        }
    };  // namespace lyt
};  // namespace nw4r
