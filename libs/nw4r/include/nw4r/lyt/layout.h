#pragma once

#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/group.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/LinkList.h"
#include <revolution/mem.h>
#include <cstddef>
#include <new>

namespace nw4r {
    namespace ut {
        struct Rect;

        template<typename C>
        class TagProcessorBase;
    };
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

    #ifdef __MWERKS__
    typedef ut::LinkList<AnimTransform, offsetof(AnimTransform, mLink)> AnimTransformList;
    #else
    typedef ut::LinkList<AnimTransform, 0> AnimTransformList;
    #endif

    class Layout {
        public:
            Layout();

            static void* AllocMemory(u32 size) {
                return MEMAllocFromAllocator(mspAllocator, size);                
            }

            static void FreeMemory(void* mem) {
                MEMFreeToAllocator(mspAllocator, mem);
            }

            virtual ~Layout();
            virtual bool Build(const void *, ResourceAccessor *);
            virtual AnimTransform* CreateAnimTransform();
            virtual AnimTransform* CreateAnimTransform(const void *, ResourceAccessor *);
            virtual AnimTransform* CreateAnimTransform(const AnimResource &, ResourceAccessor *);
            virtual void BindAnimation(AnimTransform *);
            virtual void UnbindAnimation(AnimTransform *);
            virtual void UnbindAllAnimation();
            virtual bool BindAnimationAuto(const AnimResource &, ResourceAccessor *);
            virtual void SetAnimationEnable(AnimTransform *, bool);
            virtual void CalculateMtx(const DrawInfo &);
            virtual void Draw(const DrawInfo &);
            virtual void Animate(u32);
            virtual void SetTagProcessor(ut::TagProcessorBase<wchar_t> *);

            const ut::Rect GetLayoutRect() const;

            static Pane* BuildPaneObj(s32, const void *, const ResBlockSet &);

            GroupContainer* GetGroupContainer() const { 
                return mpGroupContainer; 
            }

            static MEMAllocator* mspAllocator;

            template<typename T>
            static T* NewObj();

            template<typename T, typename Param1>
            static T* NewObj(Param1 param1);

            template<typename T, typename Param1, typename Param2>
            static T* NewObj(Param1 param1, Param2 param2);

            template<typename T>
            static void DeleteObj(T *);

            AnimTransformList mAnimTransList;
            Pane* mpRootPane;
            GroupContainer* mpGroupContainer;
            Size mLayoutSize;
        };

        template<typename T>
        inline void Layout::DeleteObj(T* pObj) {
            if (pObj) {
                pObj->~T();
                FreeMemory(pObj);
            }
        }

        template<typename T> 
        inline T* Layout::NewObj() {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T();
            }

            return 0;
        }

        template<typename T, typename Param1>
        inline T* Layout::NewObj(Param1 param1) {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T(param1);
            }
            return 0;
        }

        template<typename T, typename Param1, typename Param2>
        inline T* Layout::NewObj(Param1 param1, Param2 param2) {
            if (void* pMem = Layout::AllocMemory(sizeof(T))) {
                return new (pMem) T(param1, param2);
            }
            return 0;
        }
    };
};
