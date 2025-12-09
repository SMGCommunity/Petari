#pragma once

#include <revolution/types.h>

namespace nw4r {
    namespace ut {
#define NW4R_UT_RUNTIME_TYPEINFO                                                                                                                     \
    virtual const nw4r::ut::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                                                                    \
        return &typeInfo;                                                                                                                            \
    }                                                                                                                                                \
    static const nw4r::ut::detail::RuntimeTypeInfo typeInfo

#define NW4R_UT_RUNTIME_TYPEINFO_DEFINITION(derived, base) const nw4r::ut::detail::RuntimeTypeInfo derived::typeInfo(&base::typeInfo)

#define NW4R_UT_RUNTIME_TYPEINFO_ROOT_DEFINITION(root) const nw4r::ut::detail::RuntimeTypeInfo root::typeInfo(NULL)

        namespace detail {
            struct RuntimeTypeInfo {
                const RuntimeTypeInfo* mParentTypeInfo;
                explicit RuntimeTypeInfo(const RuntimeTypeInfo* parent) : mParentTypeInfo(parent) {}

                bool IsDerivedFrom(const RuntimeTypeInfo* typeInfo) const {
                    const RuntimeTypeInfo* self = this;
                    while (self != nullptr) {
                        if (self == typeInfo) {
                            return true;
                        }

                        self = self->mParentTypeInfo;
                    }

                    return false;
                }
            };
        };  // namespace detail
    };  // namespace ut
};  // namespace nw4r

#define NW4R_UT_RTTI_DECL(T)                                                                                                                         \
    virtual const nw4r::ut::detail::RuntimeTypeInfo* GetRuntimeTypeInfo() const {                                                                    \
        return &typeInfo;                                                                                                                            \
    }                                                                                                                                                \
                                                                                                                                                     \
    static nw4r::ut::detail::RuntimeTypeInfo typeInfo;

#define NW4R_UT_RTTI_DEF_DERIVED(T, BASE) nw4r::ut::detail::RuntimeTypeInfo T::typeInfo(&BASE::typeInfo)
